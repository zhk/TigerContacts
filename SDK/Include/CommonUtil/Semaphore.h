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

		//��ǰֵ��1
		void	Wait();

		//��ǰֵ��1
		void	Post();

		//�ɹ���ǰֵ��1
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

		//��ǰֵ��1
		void	Lock();

		//��ǰֵ��1
		void	Unlock();

		//�ɹ���ǰֵ��1
		bool	TryLock(int iMS = 0);

		String	GetName() const { return _strName; }

	protected:

		sem_t	*_sem;
		String	_strName;
	};

#endif

}
