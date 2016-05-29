/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Logger.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "Singleton.h"
#include "Mutex.h"

#if 0	//Usage:

Mem_New LogManager(eLogLevelDebug);

ILog* pRollingTest = Mem_New SkeletonLog("Logic", eLogLevelDebug, Mem_New LogFormat);

RollingFileAppender::SRollingParam param;
param._bSkipExist		= false;	//overwrite exist
param._maxFileSize		= 10000;	//bytes
param._maxRollingIndex	= 10;
param._rollingStyle		= RollingFileAppender::eRollingSize | RollingFileAppender::eRollingMinute;
pRollingTest->AddAppender(Mem_New RollingFileAppender("Logic.log", param));

LogManager::getSingleton().AddLog(pRollingTest);

for (int i = 0; i < 2000; ++i)
{
	LogN_Debug(Logic, "Logic log test. " << i);
}

#endif

namespace Cactus
{
	//-----------------------------------------------------------------------
	enum ELogLevel
	{
		eLogLevelAll		= 0x0,		// All
		eLogLevelDebug		= 0x100,	// Debug
		eLogLevelInfo		= 0x200,	// Info
		eLogLevelWarn		= 0x300,	// Warning
		eLogLevelError		= 0x400,	// Error
		eLogLevelFatal		= 0x500,	// Fatal
		eLogLevelOff		= 0x600		// Off
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API IAppender : public Object
	{
	public:
		virtual ~IAppender(){}

		virtual void	Write(Cactus::ELogLevel eLevel, const char* str) = 0;
		virtual void	Flush() = 0;
	};

	//-----------------------------------------------------------------------
	//Console Appender
	class COMMONUTIL_API ConsoleAppender : public IAppender
	{
	public:
		virtual void	Write(Cactus::ELogLevel eLevel, const char* str);
		virtual void	Flush();

	protected:
#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32
		ThreadMutex		_threadMutex;
#endif
	};

	//-----------------------------------------------------------------------
	//IDEAppender Appender
	class COMMONUTIL_API IDEAppender : public IAppender
	{
	public:
		virtual void	Write(Cactus::ELogLevel eLevel, const char* str);
		virtual void	Flush();
	};

	//-----------------------------------------------------------------------
	//Text File Appender
	class COMMONUTIL_API FileAppender : public IAppender
	{
	public:

		enum EFileEncoding
		{
			eMBCS,
			eUTF8,
			eUCS2,
			eUCS2BE,
			eMax
		};

		FileAppender(const Cactus::String& fileName, bool bAppend, EFileEncoding encoding, int iCacheItemCount);
		virtual ~FileAppender();

		virtual void	Write(Cactus::ELogLevel eLevel, const char* str);
		virtual void	Flush();

	protected:

		void		_Write(const char* buffer, size_t len);
		void		_HandleEncoding();

		Cactus::String	_fileName;
		FILE*			_file;
		EFileEncoding	_encoding;
		int				_iCacheItemCount;
		int				_iCurCacheItemCount;
	};

	//-----------------------------------------------------------------------
	//Rolling Text File Appender
	class COMMONUTIL_API RollingFileAppender : public FileAppender
	{
	public:

		enum ERollingPattern
		{
			eRollingNone	= 0x0,
			eRollingSize	= 0x1,
			eRollingDay		= 0x10,
			eRollingHour	= 0x100,
			eRollingMinute	= 0x1000,
		};

		struct SRollingParam 
		{
			SRollingParam()
			{
				_bSkipExist			= false;
				_rollingStyle		= 0;
				_maxFileSize		= 1024 * 1024 * 1024;	// 1G
				_maxRollingIndex	= 1024;
			}

			bool	_bSkipExist;	//If true, will skip exist files(include rolling files).
			int		_rollingStyle;
			size_t	_maxFileSize;
			int		_maxRollingIndex;
		};


		RollingFileAppender(const Cactus::String& fileName, SRollingParam param, FileAppender::EFileEncoding encoding, int iCacheItemCount);
		virtual ~RollingFileAppender();

		virtual void	Write(Cactus::ELogLevel eLevel, const char* str);

		static int		GetRollingType(const Cactus::String& str);

	protected:
		static Cactus::String	FindSuitFileName(const Cactus::String& fileName, SRollingParam param, int& iRetStep, int iSetp);

		Cactus::String		_strBaseFileName;
		SRollingParam		_param;
		size_t				_szCurSize;
		int					_iCurTime;
		int					_iFileNameStep;
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API ILog : public Object
	{
	public:
		virtual ~ILog(){}

		virtual	Cactus::String	GetName() = 0;

		virtual void		LogF(ELogLevel e, const char* strFile, int iLine, const char* fmt, ...) = 0;
		virtual void		Log(ELogLevel e, const char* strFile, int iLine, const char* content) = 0;

		virtual void		SetLogLevel(ELogLevel level) = 0;
		virtual bool		IsLogLevelEnabled(ELogLevel e) = 0;

		virtual void		AddAppender(IAppender* p) = 0;
		virtual void		Flush() = 0;
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API ILogFormat : public Object
	{
	public:
		virtual Cactus::String	FormatLog(const Cactus::String& name, ELogLevel e, const char* strFile, int iLine, const char* content) = 0;
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API LogFormat : public ILogFormat
	{
	public:
		virtual Cactus::String	FormatLog(const Cactus::String& name, ELogLevel e, const char* strFile, int iLine, const char* content);
	};

	//-----------------------------------------------------------------------
	// A log without ILogFormat nor IAppender. It is thread safe.
	// You can instance your own Log from this class with simply give ILogFormat and IAppender.
	class COMMONUTIL_API SkeletonLog : public ILog
	{
	public:
		SkeletonLog(const Cactus::String& name, ELogLevel eDefault = eLogLevelInfo, ILogFormat* pFormat = 0);
		virtual ~SkeletonLog();

		virtual	Cactus::String	GetName();

		virtual void		LogF(ELogLevel e, const char* strFile, int iLine, const char* fmt, ...);
		virtual void		Log(ELogLevel e, const char* strFile, int iLine, const char* content);

		virtual void		SetLogLevel(ELogLevel level);
		virtual bool		IsLogLevelEnabled(ELogLevel e);

		virtual void		AddAppender(IAppender* p);
		virtual void		Flush();

	protected:

		Cactus::vector<IAppender*>::type	_Appenders;

		ILogFormat*			_pLogFormat;

		ThreadMutex			_mutex;
		ELogLevel			_level;
		Cactus::String		_name;
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API ILogListener
	{
	public:
		virtual void	OnLogContent(const char* loggerName, ELogLevel elevel, const char* content) = 0;
	};

	//-----------------------------------------------------------------------
	class COMMONUTIL_API LogManager : public Singleton<LogManager>, public Cactus::Object
	{
	public:

		// Will also create a default log, default log will have ConsoleAppender, FileAppender and IDEAppender.
		// Log format will use the default one, LogFormat.
		// Give the log level and File configurable.
		LogManager(ELogLevel eDefault = eLogLevelInfo
			, const char* logFileName = "default.log"
			, int iCacheItemCountForFileLog = 1024
			, FileAppender::EFileEncoding encoding = FileAppender::eUTF8
			);

		// Constructed with a default log pointer
		LogManager(ILog* pLog);

		~LogManager();

		// LogManager will be in charge of deleting it.
		bool				AddLog(ILog* pLog);

		ILog*				GetLog(const Cactus::String& name);

		ILog*				GetDefaultLog(){ return _pDefaultLog; }

		void				Flush();

		static ELogLevel	GetLogLevelFromString(const Cactus::String& str);

		ILogListener*		GetLogListener(){ return _pLogListener; }
		void				SetLogListener(ILogListener* pListener){ _pLogListener = pListener; }

	protected:
		typedef Cactus::map<Cactus::String, ILog*>::type LogMap;
		LogMap			_logMap;

		ILog*			_pDefaultLog;
		ILogListener*	_pLogListener;
	};
}

//----------------------------------------------------------------------------------------------
#define DefaultLog	(Cactus::LogManager::getSingleton().GetDefaultLog())

#define Log_Fatal_(log, x) \
	{ \
		if(log && log->IsLogLevelEnabled(Cactus::eLogLevelFatal)) \
		{ \
			Cactus::ostringstream __os__; \
			__os__ << x; \
			log->Log(Cactus::eLogLevelFatal, __FILE__, __LINE__, __os__.str().c_str()); \
		} \
	}

#define Log_Error_(log, x) \
	{ \
		if(log && log->IsLogLevelEnabled(Cactus::eLogLevelError)) \
		{ \
			Cactus::ostringstream __os__; \
			__os__ << x; \
			log->Log(Cactus::eLogLevelError, __FILE__, __LINE__, __os__.str().c_str()); \
		} \
	}

#define Log_Warn_(log, x) \
	{ \
		if(log && log->IsLogLevelEnabled(Cactus::eLogLevelWarn)) \
		{ \
			Cactus::ostringstream __os__; \
			__os__ << x; \
			log->Log(Cactus::eLogLevelWarn, __FILE__, __LINE__, __os__.str().c_str()); \
		} \
	}

#define Log_Info_(log, x) \
	{ \
		if(log && log->IsLogLevelEnabled(Cactus::eLogLevelInfo)) \
		{ \
			Cactus::ostringstream __os__; \
			__os__ << x; \
			log->Log(Cactus::eLogLevelInfo, __FILE__, __LINE__, __os__.str().c_str()); \
		} \
	}

#define Log_Debug_(log, x) \
	{ \
		if(log && log->IsLogLevelEnabled(Cactus::eLogLevelDebug)) \
		{ \
			Cactus::ostringstream __os__; \
			__os__ << x; \
			log->Log(Cactus::eLogLevelDebug, __FILE__, __LINE__, __os__.str().c_str()); \
		} \
	}

//----------------------------------------------------------------------------------------------

#define Log_Fatal(x)	Log_Fatal_(DefaultLog, x)
#define Log_Error(x)	Log_Error_(DefaultLog, x)
#define Log_Warn(x)		Log_Warn_(DefaultLog, x)
#define Log_Info(x)		Log_Info_(DefaultLog, x)
#define Log_Debug(x)	Log_Debug_(DefaultLog, x)

#define LogN_Fatal(n, x)	Log_Fatal_(Cactus::LogManager::getSingleton().GetLog(#n), x)
#define LogN_Error(n, x)	Log_Error_(Cactus::LogManager::getSingleton().GetLog(#n), x)
#define LogN_Warn(n, x)		Log_Warn_(Cactus::LogManager::getSingleton().GetLog(#n), x)
#define LogN_Info(n, x)		Log_Info_(Cactus::LogManager::getSingleton().GetLog(#n), x)
#define LogN_Debug(n, x)	Log_Debug_(Cactus::LogManager::getSingleton().GetLog(#n), x)


#ifdef _DEBUG

#	define Safe
#	define UnSafe

#else

#	define Safe	try {

#	define UnSafe } \
		catch (...) \
		{ \
			Log_Fatal("	Crash occured! File : " << __FILE__ << ", function : " << __FUNCTION__ << ", line : " << __LINE__); \
		}

#endif