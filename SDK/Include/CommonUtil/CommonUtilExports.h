/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonUtil.cpp
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma	once


//----------------------------------------------------------------------------
// Windows Settings
#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32

// If we're not including this from a client build, specify that the stuff should get exported. Otherwise, import it.
#if defined(USE_COMMONUTIL_STATIC_LIB) || defined(_LIB)
// Linux compilers don't have symbol import/export directives.
#	define COMMONUTIL_API
#else
#   	if defined( CommonUtil_EXPORTS )
#		define COMMONUTIL_API __declspec(dllexport)
#	else
#		define COMMONUTIL_API __declspec(dllimport)
#	endif
#endif

// Win32 compilers use _DEBUG for specifying debug builds.
#   if defined(_DEBUG)
#       define Cactus_DEBUG_MODE 1
#else
#       define Cactus_DEBUG_MODE 0
#   endif

#elif	Cactus_PLATFORM == Cactus_PLATFORM_LINUX
// Linux Settings

#   define COMMONUTIL_API

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless you mark it yourself any way you like it -- zap ???)
#   ifdef DEBUG
#       define Cactus_DEBUG_MODE 1
#   else
#       define Cactus_DEBUG_MODE 0
#   endif

#endif
