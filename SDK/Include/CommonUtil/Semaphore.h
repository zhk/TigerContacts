/*===============================================================================
*	Copyright (C) 2006-2012, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Semaphore.h
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

#if (Cactus_PLATFORM == Cactus_PLATFORM_WIN32)

	class COMMONUTIL_API Semaphore
	{
	public:
		Semaphore();
		~Semaphore();

		//strName in such format "Global\\name"
		bool	Create(const String& strName, int iInit, int iMax, bool& bAlreadyExisted);
		bool	Destroy();

		//当前值减1
		void	Wait();

		//当前值加1
		void	Post();

		//成功后当前值减1
		bool	TryWait(int iMS = 0);

		String	GetName() const { return _strName; }

	protected:

		HANDLE	_hSemaphore;
		String	_strName;
	};

#else

#	include <fcntl.h>

	/* For O_* constants */
#	include <sys/stat.h>

	/* For mode constants */
#  include <semaphore.h>

	class Semaphore
	{
	public:
		Semaphore();
		~Semaphore();

		bool	Create(const String& strName, int iInit, int iMax, bool& bAlreadyExisted);
		bool	Destroy();

		//当前值减1
		void	Lock();

		//当前值加1
		void	Unlock();

		//成功后当前值减1
		bool	TryLock(int iMS = 0);

		String	GetName() const { return _strName; }

	protected:

		sem_t	*_sem;
		String	_strName;
	};

#endif

}
