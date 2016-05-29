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

		//�����̶߳����������ʼ���У����봫��һ��IThreadStarter
		bool	Create(IThreadStarter* starter);

		//ʹ�����̹߳���һ��ʱ�䣬-1��һֱ�ȴ���ֱ���߳̽���
		bool	Join(int time);

		//ǿ����ֹ�߳�
		void	Abort();

		//�õ��̶߳����ID��ע�ⴴ�������Ч
		threadID_t	GetID();

		//�õ������̵߳�ID
		static threadID_t	GetCurrentThreadID();

	protected:
		static void ThreadRun(LPVOID lpParameter);

		void		_CloseHandles();

		HANDLE		_thread;
		HANDLE		_endEvent;		//�����ź�
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

		//�����̶߳����������ʼ���У����봫��һ��IThreadStarter
		bool	Create(IThreadStarter* starter);

		//ʹ�����̹߳���һ��ʱ�䣬-1��һֱ�ȴ���ֱ���߳̽���������ǵȴ�ʱ�����������false��
		bool	Join(int time);

		//ǿ����ֹ�߳�
		void	Abort();

		//�õ��̶߳����ID��ע�ⴴ�������Ч
		threadID_t	GetID();

		//�õ������̵߳�ID
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
