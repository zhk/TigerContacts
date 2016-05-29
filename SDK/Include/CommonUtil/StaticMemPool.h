/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			StaticMemPool.h
*	Description:
*			
*	Revision history:
*			2010-7-13	* Created by Zhang Kun
*				Refactor from Nvwa
===============================================================================*/
/*
* Copyright (C) 2004-2008 Wu Yongwei <adah at users dot sourceforge dot net>
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any
* damages arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must
*    not claim that you wrote the original software.  If you use this
*    software in a product, an acknowledgement in the product
*    documentation would be appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must
*    not be misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* This file is part of Stones of Nvwa:
*      http://sourceforge.net/projects/nvwa
*
*/

#ifndef _STATIC_MEM_POOL_H
#define _STATIC_MEM_POOL_H

#include "PreInclude.h"
#include "Mutex.h"

namespace Cactus
{

	/**
	* Base class for memory pools.
	*/
	class COMMONUTIL_API MemPoolBase
	{
	public:
		virtual ~MemPoolBase();
		virtual void recycle() = 0;
		static void* alloc_sys(size_t __size);
		static void dealloc_sys(void* __ptr);

		/** Structure to store the next available memory block. */
		struct _Block_list { _Block_list* _M_next; };
	};

	/* Defines the macro for debugging output */
# ifdef _STATIC_MEM_POOL_DEBUG
#   include <iostream>
#   define _STATIC_MEM_POOL_TRACE(_Lck, _Msg) \
	{ \
		if (_Lck) \
		{ \
			StaticMemPoolSet::lock __guard; \
			std::cerr << "StaticMemPool: " << _Msg << std::endl; \
		} \
		else \
		{ \
			std::cerr << "StaticMemPool: " << _Msg << std::endl; \
		} \
	}
# else
#   define _STATIC_MEM_POOL_TRACE(_Lck, _Msg) ((void)0)
# endif

	/**
	* Singleton class to maintain a set of existing instantiations of
	* StaticMemPool.
	*/
	class COMMONUTIL_API StaticMemPoolSet
	{
	public:
		typedef ClassLevelLock<StaticMemPoolSet>::lock lock;
		static StaticMemPoolSet& instance();
		void recycle();
		void add(MemPoolBase* __memory_pool_p);

	private:
		StaticMemPoolSet();
		~StaticMemPoolSet();

		typedef std::vector<MemPoolBase*> container_type;
		container_type _M_memory_pool_set;

		/* Forbid their use */
		StaticMemPoolSet(const StaticMemPoolSet&);
		const StaticMemPoolSet& operator=(const StaticMemPoolSet&);
	};

	/**
	* Singleton class template to manage the allocation/deallocation of
	* memory blocks of one specific size.
	*
	* @param _Sz   size of elements in the StaticMemPool
	* @param _Gid  group id of a StaticMemPool: if it is negative,
	*              simultaneous accesses to this StaticMemPool will be
	*              protected from each other; otherwise no protection is
	*              given.
	*
	* Usage: 
	*	class TestStaticMemPool
	*	{
	*		DECLARE_STATIC_MEM_POOL(TestStaticMemPool)
	*	public:
	*		// ...
	*	};
	*
	*/
	template <size_t _Sz, int _Gid = -1>
	class StaticMemPool : public MemPoolBase
	{
		typedef typename ClassLevelLock<StaticMemPool<_Sz, _Gid>, (_Gid < 0)>
			::lock lock;
	public:

		/**
		* Gets the known instance of the static memory pool.  The instance
		* must already exist.  Generally the static initializer of the
		* template guarantees it.
		*
		* @return  reference to the instance of the static memory pool
		*/
		static StaticMemPool& instance()
		{
			assert(_S_instance_p != 0);
			return *_S_instance_p;
		}

		/**
		* Allocates memory and returns its pointer.  The template will try
		* to get it from the memory pool first, and request memory from the
		* system if there is no free memory in the pool.
		*
		* @return  pointer to allocated memory if successful; \c 0
		*          otherwise
		*/
		void* allocate()
		{
			{
				lock __guard;
				if (_S_memory_block_p)
				{
					void* __result = _S_memory_block_p;
					_S_memory_block_p = _S_memory_block_p->_M_next;
					return __result;
				}
			}
			return _S_alloc_sys(_S_align(_Sz));
		}

		/**
		* Deallocates memory by putting the memory block into the pool.
		*
		* @param __ptr pointer to memory to be deallocated
		*/
		void deallocate(void* __ptr)
		{
			assert(__ptr != 0);
			lock __guard;
			_Block_list* __block = reinterpret_cast<_Block_list*>(__ptr);
			__block->_M_next = _S_memory_block_p;
			_S_memory_block_p = __block;
		}

		virtual void recycle();

	private:
		StaticMemPool()
		{
			_STATIC_MEM_POOL_TRACE(true, "StaticMemPool<" << _Sz << ','
				<< _Gid << "> is created");
		}

		~StaticMemPool()
		{
#   ifdef _DEBUG
			// Empty the pool to avoid false memory leakage alarms.  This is
			// generally not necessary for release binaries.
			_Block_list* __block = _S_memory_block_p;
			while (__block)
			{
				_Block_list* __next = __block->_M_next;
				dealloc_sys(__block);
				__block = __next;
			}
			_S_memory_block_p = 0;
#   endif
			_S_instance_p = 0;
			_S_destroyed = true;
			_STATIC_MEM_POOL_TRACE(false, "StaticMemPool<" << _Sz << ','
				<< _Gid << "> is destroyed");
		}

		static size_t _S_align(size_t __size)
		{
			return __size >= sizeof(_Block_list) ? __size : sizeof(_Block_list);
		}

		static void* _S_alloc_sys(size_t __size);
		static StaticMemPool* _S_create_instance();

		static bool _S_destroyed;
		static StaticMemPool* _S_instance_p;
		static MemPoolBase::_Block_list* _S_memory_block_p;

		/* Forbid their use */
		StaticMemPool(const StaticMemPool&);
		const StaticMemPool& operator=(const StaticMemPool&);
	};

	template <size_t _Sz, int _Gid> bool
		StaticMemPool<_Sz, _Gid>::_S_destroyed = false;

	template <size_t _Sz, int _Gid> MemPoolBase::_Block_list*
		StaticMemPool<_Sz, _Gid>::_S_memory_block_p = 0;

	template <size_t _Sz, int _Gid> StaticMemPool<_Sz, _Gid>*
		StaticMemPool<_Sz, _Gid>::_S_instance_p = _S_create_instance();

	/**
	* Recycles half of the free memory blocks in the memory pool to the
	* system.  It is called when a memory request to the system (in other
	* instances of the static memory pool) fails.
	*/
	template <size_t _Sz, int _Gid>
	void StaticMemPool<_Sz, _Gid>::recycle()
	{
		// Only here the global lock in StaticMemPoolSet is obtained
		// before the pool-specific lock.  However, no race conditions are
		// found so far.
		lock __guard;
		_Block_list* __block = _S_memory_block_p;
		while (__block)
		{
			if (_Block_list* __temp = __block->_M_next)
			{
				_Block_list* __next = __temp->_M_next;
				__block->_M_next = __next;
				dealloc_sys(__temp);
				__block = __next;
			}
			else
			{
				break;
			}
		}
		_STATIC_MEM_POOL_TRACE(false, "StaticMemPool<" << _Sz << ','
			<< _Gid << "> is recycled");
	}

	template <size_t _Sz, int _Gid>
	void* StaticMemPool<_Sz, _Gid>::_S_alloc_sys(size_t __size)
	{
		StaticMemPoolSet::lock __guard;
		void* __result = MemPoolBase::alloc_sys(__size);
		if (!__result)
		{
			StaticMemPoolSet::instance().recycle();
			__result = MemPoolBase::alloc_sys(__size);
		}
		return __result;
	}

	template <size_t _Sz, int _Gid>
	StaticMemPool<_Sz, _Gid>* StaticMemPool<_Sz, _Gid>::_S_create_instance()
	{
		if (_S_destroyed)
			throw std::runtime_error("dead reference detected");

		StaticMemPoolSet::instance();  // Force its creation
		StaticMemPool* __inst_p = new StaticMemPool();
		try
		{
			StaticMemPoolSet::instance().add(__inst_p);
		}
		catch (...)
		{
			_STATIC_MEM_POOL_TRACE(true,
				"Exception occurs in StaticMemPoolSet::add");
			// The strange cast below is to work around a bug in GCC 2.95.3
			delete static_cast<MemPoolBase*>(__inst_p);
			throw;
		}
		return __inst_p;
	}
}	//Cactus




#define DECLARE_STATIC_MEM_POOL(_Cls) \
public: \
	static void* operator new(size_t __size) \
	{ \
		assert(__size == sizeof(_Cls)); \
		void* __ptr = Cactus::StaticMemPool<sizeof(_Cls)>::instance().allocate(); \
		if (__ptr == 0) \
			throw std::bad_alloc(); \
		return __ptr; \
	} \
	static void operator delete(void* __ptr) \
	{ \
		if (__ptr) \
			Cactus::StaticMemPool<sizeof(_Cls)>::instance().deallocate(__ptr); \
	}



#define DECLARE_STATIC_MEM_POOL__NOTHROW(_Cls) \
public: \
	static void* operator new(size_t __size) throw() \
	{ \
		assert(__size == sizeof(_Cls)); \
		return Cactus::StaticMemPool<sizeof(_Cls)>::instance().allocate(); \
	} \
	static void operator delete(void* __ptr) \
	{ \
		if (__ptr) \
			Cactus::StaticMemPool<sizeof(_Cls)>::instance().deallocate(__ptr); \
	}

#define DECLARE_STATIC_MEM_POOL_GROUPED(_Cls, _Gid) \
public: \
	static void* operator new(size_t __size) \
	{ \
		assert(__size == sizeof(_Cls)); \
		void* __ptr = Cactus::StaticMemPool<sizeof(_Cls), (_Gid)>::instance().allocate(); \
		if (__ptr == 0) \
			throw std::bad_alloc(); \
		return __ptr; \
	} \
	static void operator delete(void* __ptr) \
	{ \
		if (__ptr) \
			Cactus::StaticMemPool<sizeof(_Cls), (_Gid)>::instance().deallocate(__ptr); \
	}

#define DECLARE_STATIC_MEM_POOL_GROUPED__NOTHROW(_Cls, _Gid) \
public: \
	static void* operator new(size_t __size) throw() \
	{ \
		assert(__size == sizeof(_Cls)); \
		return Cactus::StaticMemPool<sizeof(_Cls), (_Gid)>::instance().allocate(); \
	} \
	static void operator delete(void* __ptr) \
	{ \
		if (__ptr) \
			Cactus::StaticMemPool<sizeof(_Cls), (_Gid)>::instance().deallocate(__ptr); \
	}


#endif // _STATIC_MEM_POOL_H
