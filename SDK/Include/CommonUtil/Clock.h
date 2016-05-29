/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Clock.h
*	Description:
*			A clock used for game update(From Game Programming Gems.)
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	class ITimeSource;
	class IClockObserver;

	/*
	平滑的时钟类，用于客户端比较好
	*/
	class COMMONUTIL_API Clock
	{
	public:
		Clock(const ITimeSource* pSource = 0);
		~Clock();

		void	SetTimeSource(const ITimeSource* pSource);

		void	FrameStep();

		double	GetTime() const { return _dCurrentTime; }
		double	GetFrameDuration() const { return _dFrameDuration; }
		int		GetFrameNumber() const { return _iFrameNumber; }
		float	GetFrameRate() const;

		void	AddObserver (IClockObserver * pObserver);
		void	RemoveObserver (IClockObserver * pObserver);

		void	SetFiltering (int frameWindow, double frameDefault = 0.030);

	private:
		double	GetExactLastFrameDuration ();
		void	AddToFrameHistory (double exactFrameDuration);
		double	GetPredictedFrameDuration () const;

		const ITimeSource*	_pTimeSource;
		double				_dCurrentTime;
		double				_dFrameDuration;
		int					_iFrameNumber;

		double				_dSourceStartValue;
		double				_dSourceLastValue;

		int					_iFrameFilteringWindow;
		double				_dFrameDefaultDuration;
		Cactus::deque<double>::type	_FrameDurationHistory;

		typedef Cactus::vector<IClockObserver*>::type ObserverList;
		ObserverList		_Observers;
	};

}

