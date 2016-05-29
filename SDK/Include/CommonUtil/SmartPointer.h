/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			SmartPointer.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include <assert.h>

namespace Cactus
{
	template<class T> class SmartPointerT
	{
	public:
		SmartPointerT(T* p = 0)
		{
			_pointer = p;
			if (_pointer)
			{
				_pointer->AddRef();
			}
		}

		template<class Y>
			SmartPointerT(const SmartPointerT<Y>& r)
		{
			_pointer = r._pointer;
			if(_pointer)
			{
				_pointer->AddRef();
			}
		}

		//template<>
		//	SmartPointerT(const SmartPointerT<T>& r)
		//{
		//	_pointer = r._pointer;
		//	if (_pointer)
		//	{
		//		_pointer->AddRef();
		//	}
		//}

		SmartPointerT& operator=(T* p)
		{
			if(_pointer != p)
			{
				if(p)
				{
					p->AddRef();
				}
				T* ptr = _pointer;
				_pointer = p;
				if(ptr)
				{
					ptr->Release();
				}
			}
			return *this;
		}

		template<typename Y>
			SmartPointerT& operator=(const SmartPointerT<Y>& r)
		{
			if(_pointer != r._pointer)
			{
				if (r._pointer)
				{
					r._pointer->AddRef();
				}
				T* ptr = _pointer;
				_pointer = r._pointer;
				if(ptr)
				{
					ptr->Release();
				}
			}
			return *this;
		}

		//template<>
		//	SmartPointerT& operator=(const SmartPointerT<T>& r)
		//{
		//	if(_pointer != r._pointer)
		//	{
		//		if(r._pointer)
		//		{
		//			r._pointer->AddRef();
		//		}
		//		T* ptr = _pointer;
		//		_pointer = r._pointer;
		//		if(ptr)
		//		{
		//			ptr->Release();
		//		}
		//	}
		//	return *this;
		//}

		T* operator->() const
		{
			assert(_pointer);
			return _pointer;
		}

		operator bool() const
		{
			return _pointer ? true : false;
		}

		T& operator*() const
		{
			assert(_pointer);
			return *_pointer;
		}

		~SmartPointerT()
		{
			if (_pointer)
			{
				_pointer->Release();
			}
		}

		//--------------------------------------------------------------------------------------

		template<class Y>
			static SmartPointerT DynamicCast(const SmartPointerT<Y>& r)
		{
			return SmartPointerT(dynamic_cast<T*>(r._pointer));
		}

		template<class Y>
			static SmartPointerT DynamicCast(Y* p)
		{
			return SmartPointerT(dynamic_cast<T*>(p));
		}

		T* GetPointer() const
		{
			return _pointer;
		}

		T*	_pointer;
	};

	//--------------------------------------------------------------------------------------

	template<class T, class U>
		inline bool operator==(const SmartPointerT<T>& lhs, const SmartPointerT<U>& rhs)
	{
		T* l = lhs.GetPointer();
		U* r = rhs.GetPointer();
		if(l && r)
		{
			return l == r;
		}
		else
		{
			return !l && !r;
		}	
	}

	template<class T, class U>
		inline bool operator!=(const SmartPointerT<T>& lhs, const SmartPointerT<U>& rhs)
	{
		T* l = lhs.GetPointer();
		U* r = rhs.GetPointer();
		if(l && r)
		{
			return l != r;
		}
		else
		{
			return l || r;
		}	
	}
}

//---------------------------------------------------------------------------

#define SmartPointer(classname) \
class classname; \
	typedef Cactus::SmartPointerT<classname> classname##Ptr;

// Use for casting a smart pointer of one type to a pointer or smart pointer of another type.
#define SmartPointerCast(destPtrType, srcPtrInstance) (destPtrType::DynamicCast(srcPtrInstance))


