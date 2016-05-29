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

		//�����̶߳����ȴ���ʼ�����봫��һ��IThreadStarter
		bool	Create(IThreadStarter* starter);

		//�̺߳�����ʼ����
		void	Start();

		//ʹ�����̹߳���һ��ʱ�䣬-1��һֱ�ȴ���ֱ���߳̽���
		bool	Join(int time);

		//�˳��̣߳��̺߳��������н���
		void	Exit();

		//ǿ����ֹ�߳�
		void	Abort();

		//�õ��̶߳����ID��ע�ⴴ�������Ч
		uint32	GetID();

		//�õ������̵߳�ID
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