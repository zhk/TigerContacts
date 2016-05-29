/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			LockedQueue.h
*	Description:
*			
*	Revision history:
*			2010-7-13	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "Mutex.h"
#include "OgreMemAlloc.h"

namespace Cactus
{
#if Ogre_Container_Use_MemAlloc
	template <class T, class LockType = ThreadMutex, typename StorageType = std::deque<T, STLAllocator<T, GeneralAllocPolicy> > >
#else
	template <class T, class LockType = ThreadMutex, typename StorageType = std::deque<T> >
#endif
	class LockedQueue
	{
		//! Lock access to the queue.
		LockType	_lock;

		//! Storage backing the queue.
		StorageType _queue;

	public:

		//! Create a LockedQueue.
		LockedQueue() { }

		//! Destroy a LockedQueue.
		virtual ~LockedQueue() { }

		//! Adds an item to the queue.
		void Add(const T& item)
		{
			MutexAutoLock<LockType> g(_lock);

			_queue.push_back(item);
		}

		//! Gets the next result in the queue, if any.
		bool Next(T& result)
		{
			MutexAutoLock<LockType> g(_lock);

			if (_queue.empty())
				return false;

			result = _queue.front();
			_queue.pop_front();

			return true;
		}

		size_t Size()
		{
			size_t sz;
			{
				MutexAutoLock<LockType> g(_lock);
				sz = _queue.size();
			}
			return sz;
		}
	};
}

