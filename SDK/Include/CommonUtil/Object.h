/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Object.h
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
#include "OgreMemAlloc.h"

#include "Mutex.h"


namespace Cactus
{
	class COMMONUTIL_API Object : public AllocatedObject<GeneralAllocPolicy>
	{
	public:
		Object(){}
		virtual ~Object(){}
	};

	class COMMONUTIL_API IRefObject
	{
	public:
		virtual void AddRef() = 0;
		virtual void Release() = 0;
	};


	class COMMONUTIL_API RefObject : public IRefObject, public Object
	{
	protected:
		ThreadMutex _objMutex;

	public:
		RefObject();
		virtual ~RefObject() {}

		void AddRef()
		{
			MutexAutoLock<> lock(_objMutex);
			++_lRefCount;
		}

		void Release()
		{
			long result = 0;
			{
				MutexAutoLock<> lock(_objMutex);
				result = --_lRefCount;
			}

			if(result == 0)
			{
				Mem_Delete this;
			}
		}

		Cactus::String GetObjectName() const
		{
			return _strName;
		}

		long GetReferenceCount() const
		{
			return _lRefCount; 
		}

	protected:
		long			_lRefCount;
		Cactus::String	_strName;
	};

}

#define ImplementIObject \
	public: \
		virtual void AddRef() \
		{ \
			Cactus::RefObject::AddRef(); \
		} \
		virtual void Release() \
		{ \
			Cactus::RefObject::Release(); \
		}


