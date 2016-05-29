/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			SharedPtr.h
*	Description:
*			Modified from Ogre.
*	Revision history:
*			2010-9-1	* Created by Zhang Kun
*				Created
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
#pragma once

#include "PreInclude.h"
#include "CommonTemplate.h"
#include "Mutex.h"

namespace Cactus
{
	/*
	This is a standard shared pointer implementation which uses a reference 
    count to work out when to delete the object. 
    */
	template<class T>
	class SharedPtr
	{
	protected:
		T*				_pRep;
		unsigned int*	_pRefCount;

	public:
		ThreadMutex*	_pMutex;

		SharedPtr() : _pRep(0), _pRefCount(0)
        {
            _pMutex = 0;
        }

        template<class Y>
		explicit SharedPtr(Y* rep) 
			: _pRep(rep)
		{
			M_Static_Check(M_SuperSubClass(T, Y), require_supper_sub_class_types);

			if (rep)
			{
				_pRefCount = Mem_Alloc_T(unsigned int, 1);
				*_pRefCount = 1;
			}
			else
			{
				_pRefCount = 0;
			}

			_pMutex = 0;
			if (rep)
			{
				_pMutex = new ThreadMutex;
			}
		}

		SharedPtr(const SharedPtr& r)
            : _pRep(0), _pRefCount(0)
		{
			_pMutex = 0;
            if(r._pMutex)
            {
				MutexAutoLock<> g(*r._pMutex);

				_pMutex		= r._pMutex;
			    _pRep		= r._pRep;
			    _pRefCount	= r._pRefCount; 

				// Handle zero pointer gracefully to manage STL containers
			    if(_pRefCount)
			    {
				    ++(*_pRefCount); 
			    }
            }
		}

		template< class Y>
		SharedPtr(const SharedPtr<Y>& r)
            : _pRep(0), _pRefCount(0)
		{
			M_Static_Check(M_SuperSubClass(T, Y), require_supper_sub_class_types);

			_pMutex = 0;
            if(r._pMutex)
            {
				MutexAutoLock<> g(*r._pMutex);

				_pMutex		= r._pMutex;
				_pRep		= r.getPointer();
			    _pRefCount	= r.refCountPointer();

				// Handle zero pointer gracefully to manage STL containers
			    if(_pRefCount)
			    {
				    ++(*_pRefCount);
			    }
            }
		}

		virtual ~SharedPtr()
		{
			release();
		}


		SharedPtr& operator=(const SharedPtr& r)
		{
			if (_pRep == r._pRep)
				return *this;
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}

		template< class Y>
		SharedPtr& operator=(const SharedPtr<Y>& r)
		{
			M_Static_Check(M_SuperSubClass(T, Y), require_supper_sub_class_types);

			if (_pRep == r.getPointer())
				return *this;
			
			// Swap current data into a local copy
			// this ensures we deal with rhs and this being dependent
			SharedPtr<T> tmp(r);
			swap(tmp);
			return *this;
		}


		inline T& operator*() const { assert(_pRep); return *_pRep; }
		inline T* operator->() const { assert(_pRep); return _pRep; }
		inline T* get() const { return _pRep; }

		inline bool unique() const { MutexAutoLock<> g(*_pMutex); assert(_pRefCount); return *_pRefCount == 1; }
		inline unsigned int refCount() const { MutexAutoLock<> g(*_pMutex); assert(_pRefCount); return *_pRefCount; }
		inline unsigned int* refCountPointer() const { return _pRefCount; }

		inline T* getPointer() const { return _pRep; }

		inline bool isNull(void) const { return _pRep == 0; }

		// Use setNull() to make sure your pointer goes out of scope.
        inline void setNull(void)
		{ 
			if (_pRep)
			{
				release();
				_pRep = 0;
				_pRefCount = 0;
			}
        }

    protected:

        inline void release(void)
        {
			bool destroyThis = false;

            if(_pMutex)
			{
				// lock own mutex in limited scope (must unlock before destroy)
				MutexAutoLock<> g(*_pMutex);
				if ( _pRefCount && --(*_pRefCount) == 0 )
					destroyThis = true;
            }

			if (destroyThis)
			{
				Mem_Delete _pRep;
				Mem_Free(_pRefCount);
				_pRep = 0;
				_pRefCount = 0;
				delete _pMutex;
			}

            _pMutex = 0;
        }

		virtual void swap(SharedPtr<T> &other) 
		{
			std::swap(_pRep, other._pRep);
			std::swap(_pRefCount, other._pRefCount);
			std::swap(_pMutex, other._pMutex);
		}
	};

	template<class T, class U> inline bool operator==(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> inline bool operator!=(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> inline bool operator<(SharedPtr<T> const& a, SharedPtr<U> const& b)
	{
		return std::less<const void*>()(a.get(), b.get());
	}

}
