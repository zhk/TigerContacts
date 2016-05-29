/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonUtilBasicType.h
*	Description:
*
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once


#define Cactus_PLATFORM_WIN32 1
#define Cactus_PLATFORM_LINUX 2

#define Cactus_COMPILER_MSVC 1
#define Cactus_COMPILER_GNUC 2

#define Cactus_ARCHITECTURE_32 1
#define Cactus_ARCHITECTURE_64 2


// Finds the compiler type and version.
#if defined( _MSC_VER )

#   define Cactus_COMPILER Cactus_COMPILER_MSVC
#   define Cactus_COMP_VER _MSC_VER

#elif defined( __GNUC__ )

#   define Cactus_COMPILER Cactus_COMPILER_GNUC
#   define Cactus_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)

#else

#   pragma error "Not known compiler. Abort!"

#endif


/* See if we can use __forceinline or if we need to use __inline instead */
#if Cactus_COMPILER == Cactus_COMPILER_MSVC
#   if Cactus_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#else
#   define FORCEINLINE __inline
#endif


// Finds the current platform
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define Cactus_PLATFORM Cactus_PLATFORM_WIN32
#else
#   define Cactus_PLATFORM Cactus_PLATFORM_LINUX
#endif


// Find the arch type
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define Cactus_ARCH_TYPE Cactus_ARCHITECTURE_64
#else
#   define Cactus_ARCH_TYPE Cactus_ARCHITECTURE_32
#endif



//Standard headers
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdarg>
#include <cmath>
#include <stdexcept>

// STL containers
#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include <deque>
#include <queue>
#include <bitset>
#include <stack>

// Note - not in the original STL, but exists in SGI STL and STLport
// For gcc 4.3 see http://gcc.gnu.org/gcc-4.3/changes.html
#if (Cactus_COMPILER == Cactus_COMPILER_GNUC) && !defined(STLPORT)
#   if Cactus_COMP_VER >= 430
#       include <tr1/unordered_map>
#       include <tr1/unordered_set>
#   else
#       include <ext/hash_map>
#       include <ext/hash_set>
#   endif
#else
#   if (Cactus_COMPILER == Cactus_COMPILER_MSVC) && !defined(STLPORT) && Cactus_COMP_VER >= 1600 // VC++ 10.0
#    	include <unordered_map>
#    	include <unordered_set>
#	else
#		include <hash_set>
#		include <hash_map>
#	endif
#endif


// STL algorithms & functions
#include <algorithm>
#include <functional>
#include <limits>

// C++ Stream stuff
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <iomanip>


#if Cactus_COMPILER == Cactus_COMPILER_GNUC && Cactus_COMP_VER >= 310 && !defined(STLPORT)

#	if Cactus_COMP_VER >= 430
#		define __HashMap	::std::tr1::unordered_map
#		define __HashSet	::std::tr1::unordered_set
#		define __HashCmp(x)	::std::tr1::hash< x >
#	else
#		define __HashMap	::__gnu_cxx::hash_map
#		define __HashSet	::__gnu_cxx::hash_set
#		define __HashCmp(x)	::__gnu_cxx::hash< x >
#	endif

#else

#	if Cactus_COMPILER == Cactus_COMPILER_MSVC

#		if Cactus_COMP_VER >= 1600 // VC++ 10.0
#			define __HashMap	::std::tr1::unordered_map
#			define __HashSet	::std::tr1::unordered_set
#			define __HashCmp(x)	::std::tr1::hash< x >
#		elif Cactus_COMP_VER > 1300 && !defined(_STLP_MSVC)
#			define __HashMap	::stdext::hash_map
#			define __HashSet	::stdext::hash_set
#			define __HashCmp(x)	::stdext::hash_compare<x, std::less<x> >
#		else
#			define __HashMap	::std::hash_map
#			define __HashSet	::std::hash_set
#			define __HashCmp(x)	::std::hash< x >
#		endif

#	else

#		define __HashMap	::std::hash_map
#		define __HashSet	::std::hash_set
#		define __HashCmp(x)	::std::hash< x >

#	endif

#endif


extern "C"
{
#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32
#	include <io.h>
#	include <direct.h>
#else
#	include <sys/time.h>
#   include <wchar.h>
#   include <wctype.h>
#	include <ctype.h>
#	include <unistd.h>
#	include <dlfcn.h>
#   include <dirent.h>
#	include <semaphore.h>
#	include <pthread.h>
#endif

#   include <limits.h>
#	include <signal.h>
#	include <process.h>

#   include <sys/types.h>
#   include <sys/stat.h>
#	include <sys/timeb.h>
}

namespace Cactus
{
	typedef unsigned char		uint8;
	typedef unsigned short		uint16;
	typedef unsigned short		ushort;
	typedef unsigned int		uint32;
	typedef unsigned int		uint;

	typedef signed char			int8;
	typedef signed short		int16;
	typedef signed int			int32;

// define uint64 type
#if Cactus_COMPILER == Cactus_COMPILER_MSVC
	typedef unsigned __int64		uint64;
	typedef __int64					int64;
#else
	typedef unsigned long long	uint64;
	typedef long long			int64;
#endif

}


#if Cactus_COMPILER == Cactus_COMPILER_MSVC

#	define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#	ifndef _WIN32_WINNT
#		define _WIN32_WINNT 0x403
#	endif
#	include <windows.h>

#	pragma warning(disable : 4251 4275 4355 4661 4996)

#	undef max
#	undef min
#   define vsnwprintf	_vsnwprintf
#   define snprintf		_snprintf

#else

// A quick define to overcome different names for the same function
#   define strnicmp		strncasecmp
#   define stricmp		strcasecmp
#   define wcsicmp		wcscasecmp
#   define vsnwprintf	vswprintf

#endif

