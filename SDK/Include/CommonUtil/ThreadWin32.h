/*===============================================================================
*	Copyright (C) 2006-2008, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			ThreadWin32.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once 

#include "PreInclude.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Cactus
{
	class COMMONUTIL_API IThreadStarter
	{
	public:
		virtual void Run() = 0;
	};

	class COMMONUTIL_API ThreadWin32 : public Object
	{
	public:
		ThreadWin32();
		~ThreadWin32();

		//创建线程对象后等待开始，必须传入一个IThreadStarter
		bool	Create(IThreadStarter* starter);

		//线程函数开始运行
		void	Start();

		//使调用线程挂起一段时间，-1会一直等待，直到线程结束
		bool	Join(int time);

		//退出线程，线程函数会运行结束
		void	Exit();

		//强行终止线程
		void	Abort();

		//得到线程对象的ID，注意创建后才有效
		uint32	GetID();

		//得到调用线程的ID
		static uint32	GetCurrentThreadID();

	protected:
		static void ThreadRun(LPVOID lpParameter);

		void	_WaitStart();

		void	_CloseHandles();

		HANDLE	_thread;
		HANDLE	_beginEvent;
		HANDLE	_exitEvent;
		HANDLE	_endEvent;
		uint32	_ID;

		IThreadStarter* _starter;
	};
}