/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			MemoryAllocators.h
*	Description:
*			
*	Revision history:
*			2010-7-13	* Created by Zhang Kun
*				Refactor from OGRE1.71
===============================================================================*/
/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

You may alternatively use this source under the terms of a specific version of
the OGRE Unrestricted License provided you have obtained such a license from
Torus Knot Software Ltd.
-----------------------------------------------------------------------------
*/
#pragma once

#include "CommonUtilConfig.h"
#include "CommonUtilBasicType.h"
#include "CommonUtilExports.h"

namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/
	/** Superclass for all objects that wish to use custom memory allocators
		when their new / delete operators are called.
		Requires a template parameter identifying the memory allocator policy 
		to use (e.g. see StdAllocPolicy). 
	*/
	template <class Alloc>
	class COMMONUTIL_API AllocatedObject
	{
	public:
		explicit AllocatedObject()
		{ }

		~AllocatedObject()
		{ }

		/// operator new, with debug line info
		void* operator new(size_t sz, const char* file, int line, const char* func)
		{
			return Alloc::allocateBytes(sz, file, line, func);
		}

		void* operator new(size_t sz)
		{
			return Alloc::allocateBytes(sz);
		}

		/// placement operator new
		void* operator new(size_t sz, void* ptr)
		{
			(void) sz;
			return ptr;
		}

		/// array operator new, with debug line info
		void* operator new[] ( size_t sz, const char* file, int line, const char* func )
		{
			return Alloc::allocateBytes(sz, file, line, func);
		}

		void* operator new[] ( size_t sz )
		{
			return Alloc::allocateBytes(sz);
		}

		void operator delete( void* ptr )
		{
			Alloc::deallocateBytes(ptr);
		}

		// Corresponding operator for placement delete (second param same as the first)
		void operator delete( void* ptr, void* )
		{
			Alloc::deallocateBytes(ptr);
		}

		// only called if there is an exception in corresponding 'new'
		void operator delete( void* ptr, const char* , int , const char*  )
		{
			Alloc::deallocateBytes(ptr);
		}

		void operator delete[] ( void* ptr )
		{
			Alloc::deallocateBytes(ptr);
		}


		void operator delete[] ( void* ptr, const char* , int , const char*  )
		{
			Alloc::deallocateBytes(ptr);
		}
	};


	/** @} */
	/** @} */

}


namespace Cactus
{


	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/
	/**
	Wrapper class for operating as an STL container allocator.
	This class acts as the host for a configured allocation policy.

	It takes the following template parameters:-
	<ul>
	<li>T			: the type we will be allocating (see rebind later)</li>
	<li>AllocPolicy : a memory allocator policy </li>
	</ul>

	See StdAllocPolicy for an example of implementing a Policy. The reason for
	implementing the allocation in a policy is that it makes implementing
	a custom allocator easier -  you just have to provide an alternative
	policy with just allocate/deallocate implementations; this class does all the
	housekeeping associated with keeping the STL happy.
	*/

	// Base STL allocator class.
	template<typename T>
	struct STLAllocatorBase
	{	// base class for generic allocators
		typedef T value_type;
	};

	// Base STL allocator class. (const T version).
	template<typename T>
	struct STLAllocatorBase<const T>
	{	// base class for generic allocators for const T
		typedef T value_type;
	};

	template
		<
		typename T,
		typename AllocPolicy
		>
	class STLAllocator : public STLAllocatorBase<T>
	{
	public :
		/// define our types, as per ISO C++
		typedef STLAllocatorBase<T>			Base;
		typedef typename Base::value_type	value_type;
		typedef value_type*					pointer;
		typedef const value_type*			const_pointer;
		typedef value_type&					reference;
		typedef const value_type&			const_reference;
		typedef std::size_t					size_type;
		typedef std::ptrdiff_t				difference_type;


		/// the standard rebind mechanism
		template<typename U>
		struct rebind
		{
			typedef STLAllocator<U, AllocPolicy> other;
		};

		/// ctor
		inline explicit STLAllocator()
		{ }

		/// dtor
		virtual ~STLAllocator()
		{ }

		/// copy ctor - done component wise
		inline STLAllocator( STLAllocator const& )
		{ }

		/// cast
		template <typename U>
		inline STLAllocator( STLAllocator<U, AllocPolicy> const& )
		{ }

		/// cast
		template <typename U, typename P>
		inline STLAllocator( STLAllocator<U, P> const& )
		{ }

		/// memory allocation (elements, used by STL)
		inline pointer allocate( size_type count,
			typename std::allocator<void>::const_pointer ptr = 0 )
		{
                        (void)ptr;
			// convert request to bytes
			register size_type sz = count*sizeof( T );
			pointer p  = static_cast<pointer>(AllocPolicy::allocateBytes(sz));
			return p;
		}

		/// memory deallocation (elements, used by STL)
		inline void deallocate( pointer ptr, size_type )
		{
			// convert request to bytes, but we can't use this?
			// register size_type sz = count*sizeof( T );
			AllocPolicy::deallocateBytes(ptr);
		}

		pointer address(reference x) const
		{
			return &x;
		}

		const_pointer address(const_reference x) const
		{
			return &x;
		}

		size_type max_size() const throw()
		{
			// maximum size this can handle, delegate
			return AllocPolicy::getMaxAllocationSize();
		}

		void construct(pointer p, const T& val)
		{
			// call placement new
			new(static_cast<void*>(p)) T(val);
		}

		void destroy(pointer p)
		{
			// do we have to protect against non-classes here?
			// some articles suggest yes, some no
			p->~T();
		}

	};

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename T2, typename P>
	inline bool operator==(STLAllocator<T,P> const&,
		STLAllocator<T2,P> const&)
	{
		// same alloc policy (P), memory can be freed
		return true;
	}

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename P, typename OtherAllocator>
	inline bool operator==(STLAllocator<T,P> const&,
		OtherAllocator const&)
	{
		return false;
	}
	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename T2, typename P>
	inline bool operator!=(STLAllocator<T,P> const&,
		STLAllocator<T2,P> const&)
	{
		// same alloc policy (P), memory can be freed
		return false;
	}

	/// determine equality, can memory from another allocator
	/// be released by this allocator, (ISO C++)
	template<typename T, typename P, typename OtherAllocator>
	inline bool operator!=(STLAllocator<T,P> const&,
		OtherAllocator const&)
	{
		return true;
	}


	/** @} */
	/** @} */

}// namespace Cactus


namespace Cactus 
{
    /** Class to provide aligned memory allocate functionality.
    @remarks
        All SIMD processing are friendly with aligned memory, and some SIMD routines
        are designed for working with aligned memory only. If the data are intended to
        use SIMD processing, it's need to be aligned for better performance boost.
        In additional, most time cache boundary aligned data also lead to better
        performance even if didn't used SIMD processing. So this class provides a couple
        of functions for allocate aligned memory.
    @par
        Anyways, in general, you don't need to use this class directly, Ogre internally
        will take care with most SIMD and cache friendly optimisation if possible.
    @par
        This isn't a "one-step" optimisation, there are a lot of underlying work to
        achieve performance boost. If you didn't know what are you doing or what there
        are going, just ignore this class.
    @note
        This class intended to use by advanced user only.
    */
	class COMMONUTIL_API AlignedMemory
	{
	public:
        /** Allocate memory with given alignment.
            @param
                size The size of memory need to allocate.
            @param
                alignment The alignment of result pointer, must be power of two
                and in range [1, 128].
            @returns
                The allocated memory pointer.
            @par
                On failure, exception will be throw.
        */
        static void* allocate(size_t size, size_t alignment);

        /** Allocate memory with default platform dependent alignment.
            @remarks
                The default alignment depend on target machine, this function
                guarantee aligned memory according with SIMD processing and
                cache boundary friendly.
            @param
                size The size of memory need to allocate.
            @returns
                The allocated memory pointer.
            @par
                On failure, exception will be throw.
        */
        static void* allocate(size_t size);

        /** Deallocate memory that allocated by this class.
            @param
                p Pointer to the memory allocated by this class or <b>NULL</b> pointer.
            @par
                On <b>NULL</b> pointer, nothing happen.
        */
        static void deallocate(void* p);
	};

}

#if Ogre_Memory_Allocator == Ogre_MemAlloc_std
namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/
	/**	A "standard" allocation policy for use with AllocatedObject and 
		STLAllocator. This is the class that actually does the allocation
		and deallocation of physical memory, and is what you will want to 
		provide a custom version of if you wish to change how memory is allocated.
		@par
		This class just delegates to the global malloc/free.
	*/
	class COMMONUTIL_API StdAllocPolicy
	{
	public:
		static inline void* allocateBytes(size_t count, 
#if Ogre_MemAlloc_Use_Tracker
			const char* file = 0, int line = 0, const char* func = 0
#else
			const char*  = 0, int  = 0, const char* = 0
#endif
            )
		{
			void* ptr = malloc(count);
#if Ogre_MemAlloc_Use_Tracker
			// this alloc policy doesn't do pools
			MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
			return ptr;
		}

		static inline void deallocateBytes(void* ptr)
		{
#if Ogre_MemAlloc_Use_Tracker
			MemoryTracker::get()._recordDealloc(ptr);
#endif
			free(ptr);
		}

		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		// no instantiation
		StdAllocPolicy()
		{ }
	};

	/**	A "standard" allocation policy for use with AllocatedObject and 
		STLAllocator, which aligns memory at a given boundary (which should be
		a power of 2). This is the class that actually does the allocation
		and deallocation of physical memory, and is what you will want to 
		provide a custom version of if you wish to change how memory is allocated.
		@par
		This class just delegates to the global malloc/free, via AlignedMemory.
		@note
		template parameter Alignment equal to zero means use default
		platform dependent alignment.

	*/
	template <size_t Alignment = 0>
	class StdAlignedAllocPolicy
	{
	public:
		// compile-time check alignment is available.
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment-1)) == 0) ? +1 : -1];

		static inline void* allocateBytes(size_t count, 
#if Ogre_MemAlloc_Use_Tracker
			const char* file = 0, int line = 0, const char* func = 0
#else
			const char*  = 0, int  = 0, const char* = 0
#endif
            )
		{
			void* ptr = Alignment ? AlignedMemory::allocate(count, Alignment)
				: AlignedMemory::allocate(count);
#if Ogre_MemAlloc_Use_Tracker
			// this alloc policy doesn't do pools
			MemoryTracker::get()._recordAlloc(ptr, count, 0, file, line, func);
#endif
			return ptr;
		}

		static inline void deallocateBytes(void* ptr)
		{
#if Ogre_MemAlloc_Use_Tracker
			MemoryTracker::get()._recordDealloc(ptr);
#endif
			AlignedMemory::deallocate(ptr);
		}

		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		// No instantiation
		StdAlignedAllocPolicy()
		{ }
	};
	/** @} */
	/** @} */

}// namespace Cactus

#elif Ogre_Memory_Allocator == Ogre_MemAlloc_Ned

namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/
	/** Non-templated utility class just to hide nedmalloc.
	*/
	class COMMONUTIL_API NedAllocImpl
	{
	public:
		static void* allocBytes(size_t count, 
			const char* file, int line, const char* func);
		static void deallocBytes(void* ptr);
		static void* allocBytesAligned(size_t align, size_t count, 
			const char* file, int line, const char* func);
		static void deallocBytesAligned(size_t align, void* ptr);

	};

	/**	An allocation policy for use with AllocatedObject and 
	STLAllocator. This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This allocation policy uses nedmalloc 
		(http://nedprod.com/programs/portable/nedmalloc/index.html). 
	*/
	class COMMONUTIL_API NedAllocPolicy
	{
	public:
		static inline void* allocateBytes(size_t count, 
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedAllocImpl::allocBytes(count, file, line, func);
		}
		static inline void deallocateBytes(void* ptr)
		{
			NedAllocImpl::deallocBytes(ptr);
		}
		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		// No instantiation
		NedAllocPolicy()
		{ }
	};


	/**	An allocation policy for use with AllocatedObject and 
	STLAllocator, which aligns memory at a given boundary (which should be
	a power of 2). This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This allocation policy uses nedmalloc 
		(http://nedprod.com/programs/portable/nedmalloc/index.html). 
	@note
		template parameter Alignment equal to zero means use default
		platform dependent alignment.
	*/
	template <size_t Alignment = 0>
	class NedAlignedAllocPolicy
	{
	public:
		// compile-time check alignment is available.
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment-1)) == 0) ? +1 : -1];

		static inline void* allocateBytes(size_t count, 
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedAllocImpl::allocBytesAligned(Alignment, count, file, line, func);
		}

		static inline void deallocateBytes(void* ptr)
		{
			NedAllocImpl::deallocBytesAligned(Alignment, ptr);
		}

		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		// no instantiation allowed
		NedAlignedAllocPolicy()
		{ }
	};



	/** @} */
	/** @} */

}// namespace Cactus

#elif Ogre_Memory_Allocator == Ogre_MemAlloc_NedPooling

namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/
	/** Non-templated utility class just to hide nedmalloc.
	*/
	class COMMONUTIL_API NedPoolingImpl
	{
	public:
		static void* allocBytes(size_t count, 
			const char* file, int line, const char* func);
		static void deallocBytes(void* ptr);
		static void* allocBytesAligned(size_t align, size_t count, 
			const char* file, int line, const char* func);
		static void deallocBytesAligned(size_t align, void* ptr);

	};

	/**	An allocation policy for use with AllocatedObject and 
	STLAllocator. This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This allocation policy uses nedmalloc 
		(http://nedprod.com/programs/portable/nedmalloc/index.html). 
	*/
	class COMMONUTIL_API NedPoolingPolicy
	{
	public:
		static inline void* allocateBytes(size_t count, 
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedPoolingImpl::allocBytes(count, file, line, func);
		}
		static inline void deallocateBytes(void* ptr)
		{
			NedPoolingImpl::deallocBytes(ptr);
		}
		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}

	private:
		// No instantiation
		NedPoolingPolicy()
		{ }
	};


	/**	An allocation policy for use with AllocatedObject and 
	STLAllocator, which aligns memory at a given boundary (which should be
	a power of 2). This is the class that actually does the allocation
	and deallocation of physical memory, and is what you will want to 
	provide a custom version of if you wish to change how memory is allocated.
	@par
	This allocation policy uses nedmalloc 
		(http://nedprod.com/programs/portable/nedmalloc/index.html). 
	@note
		template parameter Alignment equal to zero means use default
		platform dependent alignment.
	*/
	template <size_t Alignment = 0>
	class COMMONUTIL_API NedPoolingAlignedPolicy
	{
	public:
		// compile-time check alignment is available.
		typedef int IsValidAlignment
			[Alignment <= 128 && ((Alignment & (Alignment-1)) == 0) ? +1 : -1];

		static inline void* allocateBytes(size_t count, 
			const char* file = 0, int line = 0, const char* func = 0)
		{
			return NedPoolingImpl::allocBytesAligned(Alignment, count, file, line, func);
		}

		static inline void deallocateBytes(void* ptr)
		{
			NedPoolingImpl::deallocBytesAligned(Alignment, ptr);
		}

		/// Get the maximum size of a single allocation
		static inline size_t getMaxAllocationSize()
		{
			return std::numeric_limits<size_t>::max();
		}
	private:
		// no instantiation allowed
		NedPoolingAlignedPolicy()
		{ }
	};




	/** @} */
	/** @} */

}// namespace Cactus

#endif 