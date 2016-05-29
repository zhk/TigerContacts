/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			TimeSource.h
*	Description:
*			This file provides several high performance time sources.
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{

#if 0
	inline unsigned __int64 GetCycleCount(void)
	{
		_asm    _emit 0x0F
		_asm    _emit 0x31
	}

	inline void DoRDTSC(unsigned int &high, unsigned int &low)
	{
		unsigned int _high,_low;
		__asm
		{
			RDTSC
				mov _high, edx
				mov _low, eax
		}
		high = _high;
		low = _low;
	}

	inline unsigned int GetCycleCountLE(void)
	{
		unsigned int low, high;
		DoRDTSC( high, low );
		return ( (high << (32 - 8)) | (low >> 8) );
	}
#endif

	//-------------------------------------------------------------------------------------
#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32

	//单位：秒
	inline double	TimeGetTime()
	{
		static LARGE_INTEGER __pcount, __pcfreq;
		static int __initflag = 0;

		if (!__initflag)
		{
			QueryPerformanceFrequency(&__pcfreq);
			__initflag++;
		}

		QueryPerformanceCounter(&__pcount);
		return (double)__pcount.QuadPart / (double)__pcfreq.QuadPart;
	}

	inline int     TimeGetMilliTime()
	{
        struct __timeb64 tstruct;
        _ftime64( &tstruct );
        return tstruct.millitm;
	}

	inline void		Sleep(int iMilliseconds)
	{
		::Sleep(iMilliseconds);
	}

#else

#	include <sys/time.h>

	inline double	TimeGetTime()
	{
		struct timeval __tv;
		gettimeofday(&__tv, NULL);
		return (double)__tv.tv_sec + (double)__tv.tv_usec / 1000000;
	}

	inline int     TimeGetMilliTime()
	{
        struct timeval tstruct;
        gettimeofday(&tstruct, 0);
        return tstruct.tv_usec / 1000;
	}

	inline void		Sleep(int iMilliseconds)
	{
		usleep(iMilliseconds * 1000);
	}

#endif


	//单位：毫秒
	inline Cactus::int64	_timeGetTime()
	{
		return Cactus::int64(1000 * TimeGetTime());
	}


	//-------------------------------------------------------------------------------------
	/**
	该类目的是返回当前系统时间，单位：秒。
	*/
	class COMMONUTIL_API ITimeSource : public Object
	{
	public:
		virtual double		GetTime() const = 0;
	};


	/**
	缺省的时间资源。
	*/
	class COMMONUTIL_API TimeSource : public ITimeSource
	{
	public:
		virtual double		GetTime() const
		{
			return TimeGetTime();
		}
	};


	//-------------------------------------------------------------------------------------
	/**
	可以自定义一些时间序列。
	*/
	class COMMONUTIL_API TimeSourceCustom : public ITimeSource
	{
	public:
		TimeSourceCustom();
		virtual double		GetTime() const;

		void				QueueTime (double time);
		void				QueueStep (double time);

	private:
		mutable Cactus::deque<double>::type _TimeQueue;
	};

}
