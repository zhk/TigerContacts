/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			StringHelper.h
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
	template<size_t Size>
	class StrBufferA
	{
	public:
		StrBufferA(const wchar_t *p)
		{
			_snprintf(_strBuffer, Size, "%S", p);
		}
		StrBufferA(const char *p)
		{
			_snprintf(_strBuffer, Size, "%s", p);
		}

		const char* Str(){ return _strBuffer; }

	protected:
		char		_strBuffer[Size+1];
	};

	template<size_t Size>
	class StrBufferW
	{
	public:
		StrBufferW(const wchar_t *p)
		{
			_snwprintf(_strBuffer, Size, "%s", p);
		}
		StrBufferW(const char *p)
		{
			_snwprintf(_strBuffer, Size, "%S", p);
		}

		const wchar_t* Str(){ return _strBuffer; }

	protected:
		wchar_t		_strBuffer[Size+1];
	};

	struct StringNoCaseLess
	{
		bool operator()(const Cactus::String& x, const Cactus::String& y) const
		{
			int iRe = stricmp( x.c_str(), y.c_str() );
			return( iRe < 0 );
		}
	};

	struct FastLessCompare
	{
		bool operator()(const Cactus::String& a, const Cactus::String& b) const
		{
			const size_t la = a.length();
			const size_t lb = b.length();
			if (la == lb)
				return (memcmp(a.c_str(), b.c_str(), la*sizeof(unsigned char)) < 0);

			return (la < lb);
		}
	};

	COMMONUTIL_API	StringVector	Split(const String& strSrc, const String& strSplit);
	COMMONUTIL_API	void			TrimLeft(String& strSrc, const String& s = " \t");
	COMMONUTIL_API	void			TrimRight(String& strSrc, const String& s = " \t");
	COMMONUTIL_API	void			Trim(String& strSrc, const String& s = " \t");
}

// narrow to wide
#define M_S2W(s)	(Cactus::StrBufferW<2048>(s)).Str()

// wide to narrow
#define M_W2S(s)	(Cactus::StrBufferA<2048>(s)).Str()


#ifdef _UNICODE
#	define	M_S2T(s)	M_S2W(s)	//narrow to T
#	define	M_W2T(s)	s			//wide to T
#	define	M_T2S(s)	M_W2S(s)	//T to narrow
#	define	M_T2W(s)	s			//T to wide
#else
#	define	M_S2T(s)	s
#	define	M_W2T(s)	M_W2S(s)
#	define	M_T2S(s)	s
#	define	M_T2W(s)	M_S2W(s)
#endif
