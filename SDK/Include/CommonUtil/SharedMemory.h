/*===============================================================================
*	Copyright (C) 2006-2012, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			SharedMemory.h
*	Description:
*
*	Revision history:
*			2012-8-3	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{

	class COMMONUTIL_API SharedMemory
	{
	public:
		SharedMemory();
		~SharedMemory();

		int		Create(const String& strName, int size);
		//Windows下面，最后一个进程离开时候，SHM会被删除；中间如果在打开状态，不可删除
		bool	Destroy();

		int		Attach();
		void	Detach();
		void*	Data();

		String	Name() const;
		int		Size() const;
		bool	Owned() const { return _Priv != 0 && _bOwned; }
		void	SetDestroyOnDestructor(bool b = true) { _bDestroyOnDestructor = b; }

	protected:
		void*	_Priv;
		bool	_bOwned;
		bool	_bDestroyOnDestructor;
	};

}
