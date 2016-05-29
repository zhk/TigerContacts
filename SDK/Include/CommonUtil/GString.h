/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			GString.h
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
	class GString;
	typedef Cactus::vector<GString>::type			GStringVector;

	/**
	GString means : Game String, string class for game.
	Use UCS-2 as the string code point(Term : code point = character).
	Assume Cactus::String and char* are in MBCS if no encoding is specified.
	Assume Cactus::WString, wchar_t* and unsigned short are in UCS-2.
	*/
	class COMMONUTIL_API GString : public Object
	{
	public:

		enum EGStringEncoding
		{
			eSE_Ucs2,
			eSE_Utf8,
			eSE_Mbcs,
			eSE_Ucs4,
			eSE_Max
		};

		GString();
		~GString();

		GString(const Cactus::String& std_str, EGStringEncoding encode = eSE_Mbcs)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			if(eSE_Mbcs == encode)
			{
				FromMbcs(std_str.c_str(), std_str.size());
			}
			else if(eSE_Utf8 == encode)
			{
				FromUTF8(std_str.c_str(), std_str.size());
			}
		}

		GString(const Cactus::GString& str)
			: _strBuffer(0)
			, _uiOldHashCode(str._uiOldHashCode)
			, _strIN(str._strIN)
		{
		}

		GString(const char* str, EGStringEncoding encode = eSE_Mbcs)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			if(eSE_Mbcs == encode)
			{
				FromMbcs(str);
			}
			else if(eSE_Utf8 == encode)
			{
				FromUTF8(str);
			}
		}

		GString(const Cactus::WString& std_wstr)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN = std_wstr;
		}

		GString(const wchar_t* wstr)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN = wstr;
		}

		GString(wchar_t wch)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN += wch;
		}

		GString(char ch)
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			_strIN += (wchar_t)ch;
		}

		GString(const GStringVector& vects, const GString& strDelimit = L";")
			: _strBuffer(0)
			, _uiOldHashCode(0)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			for (size_t t = 0; t < vects.size(); ++t)
			{
				_strIN += vects[t]._strIN;
				if (t != vects.size() - 1)
					_strIN += strDelimit._strIN;
			}
		}


		GString&	operator=(const Cactus::String& std_str)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			FromMbcs(std_str.c_str(), std_str.size());
			return *this;
		}

		GString&	operator=(const char* str)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			FromMbcs(str);
			return *this;
		}

		GString&	operator=(const Cactus::WString& std_wstr)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN = std_wstr;
			return *this;
		}

		GString&	operator=(const wchar_t* wstr)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN = wstr;
			return *this;
		}

		GString&	operator=(const GString& Str)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN = Str.Str();
			return *this;
		}

		GString&	operator+=(const GString& Str)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN += Str.Str();
			return *this;
		}

		GString&	operator+=(const wchar_t* wstr)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN += wstr;
			return *this;
		}

		GString&	operator+=(const wchar_t wch)
		{
			if (_strBuffer)
			{
				Mem_Free(_strBuffer);
				_strBuffer = 0;
			}
			_strIN.append(1, wch);
			return *this;
		}

		/**
		Get the internal Cactus::WString.
		*/
		Cactus::WString&		Str()
		{
			return _strIN;
		}

		const Cactus::WString&	Str() const
		{
			return _strIN;
		}

		GString			Left(size_t count) const;

		GString			Right(size_t count) const;

		GString			SubStr(size_t pos, size_t count) const;

		/**
		Get the internal wchar_t at position szPos.
		*/
		wchar_t			At(size_t szPos) const
		{
			return ( szPos >= _strIN.length() ) ? 0 : _strIN[szPos];
		}

		/**
		Convert a C style string encoded in UTF8 into another C style string encoded in UCS2.
		*/
		size_t			UTF8_to_UCS2(const char* src, wchar_t* dest, size_t dest_len, size_t src_len = 0) const;

		/**
		Build GString from C style string encoded in UTF8.
		*/
		void			FromUTF8(const char* str, size_t szLen = 0);

		/**
		Build GString from C style string encoded in MBCS.
		*/
		void			FromMbcs(const char* str, size_t szLen = 0);

		/**
		Return a buffer encoded in MBCS of the GString.
		*/
		const char*		ToMbcs() const;

		/**
		Return a buffer encoded in UTF-8 of the GString.
		*/
		const char*		ToUtf8() const;

		/**
		Convert all characters in the GString to uppercase.
		*/
		void			ToUpper();

		/**
		Convert all characters in the GString to lowercase.
		*/
		void			ToLower();

		/**
		Replaces each format item in a specified GString with the text equivalent of
		a corresponding variable's value.
		*/
		void			Format(const wchar_t* fmt, ...);
		
		/**
		Removes all occurrences of white space characters from the beginning and end of this instance.
		Whitespace means any character for which isspace() returns true. This includes the ASCII characters
		'\\t', '\\n', '\\v', '\\f', '\\r', and ' '.		
		*/
		bool			Trim();

		/**
		Removes whitespace from the start and the end, and that has each sequence of internal whitespace
		replaced with a single space.
		Whitespace means any character for which isspace() returns true. This includes the ASCII characters
		'\\t', '\\n', '\\v', '\\f', '\\r', and ' '.		
		*/
		bool			Simplify();

		/**
		Replace %1 %2 %3 ... %n in turn. Like QT.
		*/
		template<typename T>
		bool			Arg(T t)
		{
			Cactus::wostringstream wos;
			wos << t;
			Cactus::WString strValue = wos.str();

			for (int i = 0; i < 16; ++i)
			{
				GString strFmt;
				strFmt.Format(L"%%%d", i);

				size_t pos = _strIN.find(strFmt.Str());
				if( pos != Cactus::WString::npos )
				{
					_strIN.replace( pos, strFmt.Str().length(), strValue );
					return true;
				}
			}

			return false;
		}

		/**
		Returns the hash code for this instance.
		*/
		unsigned int	GetHashCode() const;


		/**
		Is it an integer or float? Not support scientific notation.
		*/
		bool			IsNumber();

		bool			Parse(int& value);
		bool			Parse(bool& value);
		bool			Parse(wchar_t& value);
		bool			Parse(float& value);

		bool			StartWith(const GString& strValue) const;

		bool			EndWith(const GString& strValue) const;

		/**
		Type=Family, Match("Type"), out "Family" 
		*/
		bool			Match(const GString& strValue, GString& strOut);

		/**
		*/
		GStringVector	Split(const GString& strDelimiter = L",;") const;
		GStringVector	SplitByString(const GString& strDelimiter) const;

		/**
		(Speed=125,Mass=99,Position=(1,1,1),Direction=(1,0,1)) has 4 groups. (1,1,1) has 3.
		*/
		bool			SplitGroup(GStringVector& Groups, const GString& strDelimiter = L",;", const GString& strGroupChars = L"()");

		/**
		something like : Data/Config/config.xml
		*/
		bool			ParseFileName(GString& strOutPath		// Data/Config/
							, GString& strOutName				// config
							, GString& strOutExt);				// xml

		/**
		something like : (255, 255, 128)
		*/
		bool			ParseIntGroup3(int& x, int& y, int& z);

		/**
		something like : (12.0,123.0)
		*/
		bool			ParseFloatGroup2(float& x, float& y);


		/**
		something like : (12.0,123.0,1234.0)
		*/
		bool			ParseFloatGroup3(float& x, float& y, float& z);

		/**
		something like : (12.0,123.0,1234.0,12345.0)
		*/
		bool			ParseFloatGroup4(float& x, float& y, float& z, float& w);

		/**
		something like : <string0><strDelimiter><string1>...
		*/
		bool			ParseStr2StrVector(GStringVector& Groups, const GString& strDelimiter);

	private:
		Cactus::WString		_strIN;
		mutable char*		_strBuffer;
		unsigned int		_uiOldHashCode;
	};

	COMMONUTIL_API GString 			operator+(const GString& str1, const GString& str2);
	COMMONUTIL_API GString			operator+(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator==(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator==(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator!=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator!=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator>(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator>(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator>=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator>=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator<(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator<(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API bool				operator<=(const GString& str1, const GString& str2);
	COMMONUTIL_API bool				operator<=(const GString& str, const wchar_t* wstr);

	COMMONUTIL_API std::ostream&	operator<<(std::ostream& s, const GString& str);
	COMMONUTIL_API std::wostream&	operator<<(std::wostream& s, const GString& str);
}

/// convert to MBCS display string, source is utf8
#define _D(str) (Cactus::GString(str, Cactus::GString::eSE_Utf8).ToMbcs())