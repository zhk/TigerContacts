/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			StopWatch.h
*	Description:
*			High precise time functions.(From Game Programming Gems.)
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "TimeSource.h"

namespace Cactus
{

	class COMMONUTIL_API StopWatch
	{
	public:
		StopWatch() : _count(0), _startTime(0), _hold(false)
		{
		}

		void Start()
		{
			//unsigned int low, high;
			//DoRDTSC( high, low );
			//_startTime = ((low >> 8) | (high << (32 - 8)));

			_startTime = _timeGetTime();

			_count = 1;
		}

		void Stop()
		{
			//		Util_Assert(_count > 0);
			_count--;

			//unsigned int low, high;
			//DoRDTSC( high, low );
			//_count += ((low >> 8) | (high << (32 - 8))) - _startTime;

			_count += _timeGetTime() - _startTime;
		}

		void Hold()
		{
			//unsigned int low, high;
			//DoRDTSC( high, low );
			//_count += ((low >> 8) | (high << (32 - 8))) - _startTime;

			_count += _timeGetTime() - _startTime;
			_hold = true;
		}

		void Unhold()
		{
			if(!_hold)	// do not report error
			{
				return;
			}

			//unsigned int low, high;
			//DoRDTSC( high, low );
			//_startTime = ((low >> 8) | (high << (32 - 8)));

			_startTime = _timeGetTime();
			_hold = false;
		}

		void Reset()
		{
			_count = 0;
			_startTime = 0;
			_hold = false;
		}

		Cactus::int64 GetTime() const
		{
			return _count;
		}

	protected:
		Cactus::int64	_count; 
		Cactus::int64	_startTime;
		bool			_hold;
	};

}