/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Mutex.h
*	Description:
*
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "CommonUtilConfig.h"
#include "CommonUtilBasicType.h"
#include "CommonUtilExports.h"

#ifdef _DEBUG
#   include <stdio.h>
#   include <stdlib.h>
#   define _FAST_MUTEX_ASSERT(_Expr, _Msg) \
	if (!(_Expr)) \
	{ \
		fprintf(stderr, "ThreadMutex::%s\n", _Msg); \
		abort(); \
	}
#else
#   define _FAST_MUTEX_ASSERT(_Expr, _Msg) ((void)0)
#endif

namespace Cactus
{

	class COMMONUTIL_API NullMutex
	{
	public:
		NullMutex(){}
		~NullMutex(){}

		void	Lock(){}
		bool	Trylock(){ return true; }
		void	Unlock(){}
	};

	//------------------------------------------------------------------------------

#if (Cactus_PLATFORM == Cactus_PLATFORM_WIN32)

	class COMMONUTIL_API ThreadMutex
	{
	public:
		ThreadMutex();
		~ThreadMutex();

		void	Lock();
		bool	Trylock();
		void	Unlock();

	private:
		ThreadMutex(const ThreadMutex&);
		ThreadMutex& operator=(const ThreadMutex&);

		CRITICAL_SECTION _cs;
#ifdef _DEBUG
		int _ilocked;
#endif
	};

#else

	class ThreadMutex
	{
	public:
		ThreadMutex();
		~ThreadMutex();

		void	Lock();
		bool	Trylock();
		void	Unlock();


	private:
		ThreadMutex(const ThreadMutex& null);
		ThreadMutex& operator=(const ThreadMutex& null);

		pthread_mutex_t _mutex;
#ifdef _DEBUG
		int _ilocked;
#endif
	};

#endif


	//------------------------------------------------------------------------------
	template<class LockType = ThreadMutex>
	class MutexAutoLock
	{
	public:
		explicit MutexAutoLock(LockType& mutex)
			: _mutex(mutex)
		{
			_mutex.Lock();
		}

		~MutexAutoLock()
		{
			_mutex.Unlock();
		}

	private:
		LockType& _mutex;
		MutexAutoLock(const MutexAutoLock&);
		MutexAutoLock& operator=(const MutexAutoLock&);
	};

	//------------------------------------------------------------------------------
	// Usage:
	// class SomeClass : public ObjectLevelLock<SomeClass>
	// typedef ObjectLevelLock<SomeClass>::lock lock;
	// lock __guard(*this);
	template <class _Host>
	class ObjectLevelLock
	{
		mutable ThreadMutex _M_mtx;
	public:
		class lock;
		friend class lock;

		/** Type that provides locking/unlocking semantics. */
		class lock
		{
			const ObjectLevelLock& _M_host;

			lock(const lock&);
			lock& operator=(const lock&);
		public:
			explicit lock(const ObjectLevelLock& __host) : _M_host(__host)
			{
				_M_host._M_mtx.Lock();
			}
			~lock()
			{
				_M_host._M_mtx.Unlock();
			}
#ifndef NDEBUG
			// The purpose of this method is allow one to write code
			// like "assert(guard.GetLockedObject() == this)" to
			// ensure that the locked object is exactly the object being
			// accessed.
			const ObjectLevelLock* GetLockedObject() const
			{
				return &_M_host;
			}
#   endif
		};
	};


	//------------------------------------------------------------------------------
	// Usage:
	// typedef ObjectLevelLock<Host>::lock lock;
	// lock __guard;
	template <class _Host, bool _bRealLock = true>
	class ClassLevelLock
	{
		static ThreadMutex _S_mtx;
	public:
		class lock;
		friend class lock;

		/** Type that provides locking/unlocking semantics. */
		class lock
		{
			lock(const lock&);
			lock& operator=(const lock&);
		public:
			lock()
			{
				if(_bRealLock)
					_S_mtx.Lock();
			}
			~lock()
			{
				if(_bRealLock)
					_S_mtx.Unlock();
			}
		};
	};

	template <class _Host, bool _bRealLock>
	ThreadMutex ClassLevelLock<_Host, _bRealLock>::_S_mtx;

}

#define CACTUS_GUARD(MUTEX, OBJ, LOCK)					Cactus::MutexAutoLock< MUTEX > OBJ(LOCK)
#define CACTUS_GUARD_RETURN(MUTEX, OBJ, LOCK, RETURN) 	Cactus::MutexAutoLock< MUTEX > OBJ(LOCK)
	
