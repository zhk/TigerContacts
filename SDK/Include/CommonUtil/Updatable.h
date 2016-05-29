/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Updatable.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "NatureTime.h"
#include "Mutex.h"

namespace Cactus
{
	//可更新接口类
	class COMMONUTIL_API IUpdatable
	{
	public:
		virtual bool	OnUpdate(int iDeltaTime) = 0;
	};

	class COMMONUTIL_API SimpleTimer
	{
	public:
		SimpleTimer(){ _bEnabled = false; }
		~SimpleTimer(){}

		//毫秒
		void	SetTimer(int iLen, bool bRepeat = false){ _bEnabled = true; _iTimeLength = iLen; _iTimeCur = 0; _bRepeat = bRepeat; }
		void	ResetTimer(){ _iTimeCur = 0; }
		bool	OnTimer(int iDelta)
		{
			if (!_bEnabled)	
				return false;

			_iTimeCur += iDelta;
			if (_iTimeCur >= _iTimeLength)
			{
				if (_bRepeat)
					_iTimeCur = 0;
				else
					_bEnabled = false;

				return true;
			}
			return false;
		}
		bool	IsTimerSet(){ return _bEnabled; }
		void	CancelTimer(){ _bEnabled = false; }
		void	Postpond(int iDelta){ _iTimeLength += iDelta; }
		int		GetLeftTime()
		{
			if (!_bEnabled)
				return 0;
			else
				return (_iTimeLength - _iTimeCur);
		}

	protected:
		int		_iTimeLength;
		int		_iTimeCur;
		bool	_bEnabled;
		bool	_bRepeat;
	};

	//延时任务接口类
	class COMMONUTIL_API DelayedTask : public Object
	{
	public:
		//in ms
		DelayedTask(int iDelayTime);
		virtual ~DelayedTask(){}

		//如果返回false不需要底层删除操作
		virtual bool	Run() = 0;

		void			PauseTask(bool b = true){ _bPaused = b; }
		void			SetDelayTime(int iDelayTime){ _iTimeInMS = iDelayTime; _bPaused = false; }
		bool			TimePassed(int iDeltaTime);

	protected:
		int		_iTimeInMS;
		bool	_bPaused;
	};

	// 内嵌定时器和延时任务支持的可更新类
	class COMMONUTIL_API DelayTaskUpdatable : public IUpdatable, public Object
	{
	public:
		DelayTaskUpdatable();
		virtual ~DelayTaskUpdatable();

		virtual bool		OnUpdate(int iDeltaTime);

		//DelayTask, these functions are thread safe
		void				AddDelayTask(DelayedTask* pTask);
		void				CancelDelayTask(DelayedTask* pTask);
		void				CancelAllDelayTask();

	protected:

		typedef Cactus::list<DelayedTask*>::type	TasksListType;
		TasksListType		_TimerDelayQueues;
		Cactus::ThreadMutex	_delayQueueMutex;
		bool				_bDelayTasksUpdating;
	};

}