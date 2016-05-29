/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			IndexedObjectPool.h
*	Description:
*			
*			
*	Revision history:
*			2010-8-3	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "Mutex.h"
#include "CommonTemplate.h"
#include "OgreMemAlloc.h"

namespace Cactus
{

	class COMMONUTIL_API IndexedObject
	{
	public:
		IndexedObject(){}
		virtual ~IndexedObject(){}

		void			SetIndex(Cactus::uint32 u) { _uIndex = u; }
		Cactus::uint32	GetIndex(){ return _uIndex; }

	protected:
		Cactus::uint32	_uIndex;
	};

	template<typename T, size_t szInitCount, size_t szIncreasement, class LockType = Cactus::ThreadMutex>
	class ObjectPoolT
	{
	public:

		typedef typename T*		PointerTypeT;

		ObjectPoolT()
		{
			M_Static_Check(szInitCount > 0 && szIncreasement > 0, IndexObjectPool_size_or_increasement_can_not_be_zero);

			_szCount = 0;

			for (size_t t = 0; t < szInitCount; ++t)
			{
				PointerTypeT pObj = Mem_New T;
				pObj->SetIndex(t + _szCount);
				_Objects.push_back(pObj);
				_FreeObjectIndexes.push_back(t);
			}
			_szCount = szInitCount;
		}

		~ObjectPoolT()
		{
			for (size_t t = 0; t < _Objects.size(); ++t)
			{
				Mem_Delete _Objects[t];
			}
		}

		PointerTypeT	GetFreeObject()
		{
			MutexAutoLock<LockType> g(_lock);

			if (_FreeObjectIndexes.size() == 0)
			{
				for (size_t t = 0; t < szIncreasement; ++t)
				{
					PointerTypeT pObj = Mem_New T;
					pObj->SetIndex(Cactus::uint32(t + _szCount));
					_Objects.push_back(pObj);

					_FreeObjectIndexes.push_back(Cactus::uint32(t));
				}
				_szCount += szIncreasement;
			}

			int iIndex = _FreeObjectIndexes.front();
			_FreeObjectIndexes.pop_front();

			return _Objects[iIndex];
		}

		void	ReturnBackObject(PointerTypeT pT)
		{
			MutexAutoLock<LockType> g(_lock);

			_FreeObjectIndexes.push_back( pT->GetIndex() );
		}

		size_t	GetObjectCount(){ return _Objects.size(); }

		PointerTypeT	GetObjectByIndex(int iIndex)
		{
			MutexAutoLock<LockType> g(_lock);

			if (iIndex < _Objects.size())
			{
				return _Objects[iIndex];
			}

			return 0;
		}

	protected:
		size_t					_szCount;
		LockType				_lock;

		typedef typename Cactus::vector< PointerTypeT >::type	ObjectVectorType;
		ObjectVectorType		_Objects;

		typedef Cactus::list<Cactus::uint32>::type	FreeObjectListType;
		FreeObjectListType		_FreeObjectIndexes;
	};

}
