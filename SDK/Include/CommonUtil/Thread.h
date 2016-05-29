/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Thread.h
*	Description:
*
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"


namespace Cactus
{
	class COMMONUTIL_API IThreadStarter
	{
	public:
		virtual void Run() = 0;
	};
}

namespace Cactus
{

#if (Cactus_PLATFORM == Cactus_PLATFORM_WIN32)

#	define	threadID_t	uint32

	class COMMONUTIL_API Thread : public Object
	{
	public:
		Thread();
		~Thread();

		//创建线程对象后立即开始运行，必须传入一个IThreadStarter
		bool	Create(IThreadStarter* starter);

		//使调用线程挂起一段时间，-1会一直等待，直到线程结束
		bool	Join(int time);

		//强行终止线程
		void	Abort();

		//得到线程对象的ID，注意创建后才有效
		threadID_t	GetID();

		//得到调用线程的ID
		static threadID_t	GetCurrentThreadID();

	protected:
		static void ThreadRun(LPVOID lpParameter);

		void		_CloseHandles();

		HANDLE		_thread;
		HANDLE		_endEvent;		//结束信号
		threadID_t	_ID;

		IThreadStarter* _starter;
	};

#else

#	define	threadID_t	pthread_t

	class Thread : public Object
	{
	public:
		Thread();
		~Thread();

		//创建线程对象后立即开始运行，必须传入一个IThreadStarter
		bool	Create(IThreadStarter* starter);

		//使调用线程挂起一段时间，-1会一直等待，直到线程结束。如果是等待时间结束，返回false。
		bool	Join(int time);

		//强行终止线程
		void	Abort();

		//得到线程对象的ID，注意创建后才有效
		threadID_t	GetID();

		//得到调用线程的ID
		static threadID_t	GetCurrentThreadID();

	protected:
		static void* ThreadRun(void* lpParameter);

		void		_CloseHandles();


		pthread_attr_t  _thread;
		sem_t			_endEvent;

		threadID_t		_ID;

		IThreadStarter* _starter;
	};

#endif

}
