/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			MemoryTracker.h
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

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

#ifndef _MemoryTracker_H__
#define _MemoryTracker_H__

#pragma once

#include "Mutex.h"

namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/

#if Ogre_MemAlloc_Use_Tracker

	/** This class tracks the allocations and deallocations made, and
		is able to report memory statistics and leaks.
	@note
		This class is only available in debug builds.
	*/
	class COMMONUTIL_API MemoryTracker
	{
	protected:
		//OGRE_AUTO_MUTEX
		ThreadMutex	_mutex;

		// Allocation record
		struct Alloc
		{
			size_t bytes;
			unsigned int pool;
			std::string filename;
			size_t line;
			std::string function;

			Alloc() :bytes(0), line(0) {}
			Alloc(size_t sz, unsigned int p, const char* file, size_t ln, const char* func)
				:bytes(sz), pool(p), line(ln)
			{
				if (file)
					filename = file;
				if (func)
					function = func;
			}
		};

		std::string mLeakFileName;
		bool mDumpToStdOut;
		typedef __HashMap<void*, Alloc> AllocationMap;
		AllocationMap mAllocations;

		size_t mTotalAllocations;
		typedef std::vector<size_t> AllocationsByPool;
		AllocationsByPool mAllocationsByPool;

		void reportLeaks();

		// protected ctor
		MemoryTracker()
			: mLeakFileName("MemLeaks.log"), mDumpToStdOut(true),
			mTotalAllocations(0)
		{
		}
	public:

		/** Set the name of the report file that will be produced on exit. */
		void setReportFileName(const std::string& name)
		{
			mLeakFileName = name;
		}
		/// Return the name of the file which will contain the report at exit
		const std::string& getReportFileName() const
		{
			return mLeakFileName;
		}
		/// Sets whether the memory report should be sent to stdout
		void setReportToStdOut(bool rep)
		{
			mDumpToStdOut = rep;
		}
		/// Gets whether the memory report should be sent to stdout
		bool getReportToStdOut() const
		{
			return mDumpToStdOut;
		}

		/// Get the total amount of memory allocated currently.
		size_t getTotalMemoryAllocated() const;
		/// Get the amount of memory allocated in a given pool
		size_t getMemoryAllocatedForPool(unsigned int pool) const;


		/** Record an allocation that has been made. Only to be called by
			the memory management subsystem.
			@param ptr The pointer to the memory
			@param sz The size of the memory in bytes
			@param pool The memory pool this allocation is occurring from
			@param file The file in which the allocation is being made
			@param ln The line on which the allocation is being made
			@param func The function in which the allocation is being made
		*/
		void _recordAlloc(void* ptr, size_t sz, unsigned int pool = 0,
						  const char* file = 0, size_t ln = 0, const char* func = 0);
		/** Record the deallocation of memory. */
		void _recordDealloc(void* ptr);

		~MemoryTracker()
		{
			reportLeaks();
		}

		/// Static utility method to get the memory tracker instance
		static MemoryTracker& get();


	};



#endif
	/** @} */
	/** @} */

}

#endif

