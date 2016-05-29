/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Profile.h
*	Description:
*			To statics time cost for functions.(From Game Programming Gems.)
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "StopWatch.h"

#if !M_RETAIL_VERSION

/**
简单方便的宏，可以统计一段操作执行了一定次数后总时间花费。时间单位：秒。
*/
#define M_StatsBegin(Key) \
	static int s_iCallTimes##Key = 0; \
	static double s_fTimeCost##Key = 0; \
	double fTimeBegin##Key = Cactus::GetCurProcessTime();

#define M_StatsEnd(Key, Times) \
	s_iCallTimes##Key++; \
	s_fTimeCost##Key += Cactus::GetCurProcessTime() - fTimeBegin##Key; \
	if(s_iCallTimes##Key >= Times) \
	{ \
		Log_Debug("[Profiler] " << #Key << ", called " << s_iCallTimes##Key << " times, costs : " << s_fTimeCost##Key ); \
		s_iCallTimes##Key = 0; \
		s_fTimeCost##Key = 0; \
	}

#else

#define M_StatsBegin(Key)
#define M_StatsEnd(Key, Times)

#endif

namespace Cactus
{

	class COMMONUTIL_API IProfileReceiver
	{
	public:
		virtual ~IProfileReceiver()
		{
		}
		
		virtual void	ReportStart()						= 0;
		virtual void	Report(const Cactus::String& strMsg)	= 0;
		virtual void	ReportEnd()							= 0;
	};

	/**
	*/
	class COMMONUTIL_API Profiler
	{
		// Define buffer size
		const static int MAX_SECTION_NODE	= 1024;	// Section node, use a static array to represent a tree
		const static int MAX_STOPWATCH_NODE	= 1024;
		const static int MAX_COUNTER_NODE	= 1024;
		const static int MAX_STRING_NODE	= 256;	// Extra string
		const static int MAX_STRING_LENGTH	= 1024;	// Bytes per extra string

	public:
		// Reenterable section statistics
		struct SectionNode
		{
			// tree node property
			int	parent;
			int firstChild;
			int nextSibling;

			Cactus::String	id;			// name
			unsigned int	count;		// how many times this section enters
			StopWatch		timer;		// the timer
			bool			open;		// state

			SectionNode(): parent(-1), firstChild(-1), nextSibling(-1), id(0), count(0), open( false )
			{
			}
		};

		// Custom stopwatch statistics
		struct StopwatchNode
		{
			Cactus::String	id;
			unsigned int	count;
			StopWatch		timer;

			StopwatchNode(): id(0), count(0)
			{
			}
		};

		// Custom counter statistics
		struct CounterNode
		{
			Cactus::String	id;
			unsigned int	count;

			CounterNode(): id(0), count(0)
			{
			}
		};

	public:
		virtual ~Profiler();
		static Profiler*	GetInstance();
		static void			KillInstance();

		void	EnableStopwatch( int iIndex, char* pID );
		void	EnableCounter( int iIndex, char* pID );

		//////////////////////////////////////////////////////////////////////////
		// Entrances for profile macros
		void	Start();
		void	End();

		void	EnterSection( char* const id );
		void	LeaveSection( char* const id );

		void	StartStopwatchByName( const char* id );
		void	StopStopwatchByName( const char* id );

		void	StartStopwatchByIndex( int iIndex );
		void	StopStopwatchByIndex( int iIndex );

		void	IncCounterByName( const char* id );
		void	DecCounterByName( const char* id );

		void	IncCounterByIndex( int iIndex );
		void	DecCounterByIndex( int iIndex );

		void	OffsetCounterByName( const char* id, int iOffset);
		void	OffsetCounterByIndex( int iIndex, int iOffset);

		void	AddExtraString( const char* sString );

		void	Report(IProfileReceiver* pReceiver);
		void	SetEnable(bool b){ _enable = b; }

	protected:
		void	CopyDataToDisplayBuffer();
		void	SerializeTree(int nodeIndex, int level, int &lineIndex);

	protected:
		bool			_enable;
		unsigned int	_countFrequency;

		// Local statistics, valid in a frame
		SectionNode		_sectionNodes[MAX_SECTION_NODE];
		int				_sectionNodeSize;
		int				_currentSectionNodePos;

		StopwatchNode	_stopwatchNodes[MAX_STOPWATCH_NODE];
		int				_stopwatchNodeSize;

		CounterNode		_counterNodes[MAX_COUNTER_NODE];
		int				_counterNodeSize;

		char			_stringNodes[MAX_STRING_NODE][MAX_STRING_LENGTH];	// Extra strings
		int				_stringNodeSize;

		// For display, data from the previous frame
		char			_displayStrings[MAX_STRING_NODE * 2][MAX_STRING_LENGTH];	// Extra strings
		int				_displayStringSize;

		//static Profiler* _instance;
		Profiler();
	};
}
