/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			OgreMemAlloc.h
*	Description:
*
*	Revision history:
*			2010-7-13	* Created by Zhang Kun
*				Refactor from OGRE1.71
===============================================================================*/
/*-------------------------------------------------------------------------
This source file is a part of OGRE
(Object-oriented Graphics Rendering Engine)

For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE
-------------------------------------------------------------------------*/
#ifndef __OgreMemAlloc_H__
#define __OgreMemAlloc_H__

#pragma once

#include "MemoryAllocatorConfig.h"

namespace Cactus
{
#if Ogre_String_Use_MemAlloc

	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy > >			_Std_WStringBase;
	typedef std::basic_string<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > >					_Std_StringBase;

	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy> >		_Std_WStringStreamBase;
	typedef std::basic_stringstream<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy > >				_Std_StringStreamBase;

	typedef std::basic_ostringstream<char, std::char_traits<char>, STLAllocator<char, GeneralAllocPolicy> >				_Std_OStringStream;
	typedef std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, STLAllocator<wchar_t, GeneralAllocPolicy> >	_Std_WOStringStream;


	#define StdStringT(T)			std::basic_string<T, std::char_traits<T>, std::allocator<T> >
	#define CustomMemoryStringT(T)	std::basic_string<T, std::char_traits<T>, STLAllocator<T, GeneralAllocPolicy> >


	template<typename T>
	bool operator <(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<0;
	}
	template<typename T>
	bool operator <(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<0;
	}

	template<typename T>
	bool operator <=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<=0;
	}
	template<typename T>
	bool operator <=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())<=0;
	}

	template<typename T>
	bool operator >(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>0;
	}
	template<typename T>
	bool operator >(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>0;
	}

	template<typename T>
	bool operator >=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>=0;
	}
	template<typename T>
	bool operator >=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())>=0;
	}

	template<typename T>
	bool operator ==(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())==0;
	}
	template<typename T>
	bool operator ==(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())==0;
	}

	template<typename T>
	bool operator !=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())!=0;
	}
	template<typename T>
	bool operator !=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return l.compare(0, l.length(), o.c_str(), o.length())!=0;
	}

	template<typename T>
	CustomMemoryStringT(T) operator +=(const CustomMemoryStringT(T)& l, const StdStringT(T)& o)
	{
		return CustomMemoryStringT(T)(l)+=o.c_str();
	}
	template<typename T>
	CustomMemoryStringT(T) operator +=(const StdStringT(T)& l, const CustomMemoryStringT(T)& o)
	{
		return CustomMemoryStringT(T)(l.c_str())+=o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const CustomMemoryStringT(T)& l,const StdStringT(T)& o)
	{
		return CustomMemoryStringT(T)(l)+=o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const StdStringT(T)& l,const CustomMemoryStringT(T)& o)
	{
		return CustomMemoryStringT(T)(l.c_str())+=o.c_str();
	}

	template<typename T>
	CustomMemoryStringT(T) operator +(const T* l,const CustomMemoryStringT(T)& o)
	{
		return CustomMemoryStringT(T)(l)+=o;
	}

	#undef StdStringT
	#undef CustomMemoryStringT

#else

	//typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >		_Std_WStringBase;
	//typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> >				_Std_StringBase;

	//typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >	_Std_WStringStreamBase;
	//typedef std::basic_stringstream<char, std::char_traits<char>, std::allocator<char> >			_Std_StringStreamBase;

	//typedef std::basic_ostringstream<char, std::char_traits<char>, std::allocator<char> >			_Std_OStringStream;

	typedef std::wstring				_Std_WStringBase;
	typedef std::string					_Std_StringBase;

	typedef std::wstringstream			_Std_WStringStreamBase;
	typedef std::stringstream			_Std_StringStreamBase;

	typedef std::ostringstream			_Std_OStringStream;
	typedef std::wostringstream			_Std_WOStringStream;


#endif

	typedef _Std_WStringBase		WString;
	typedef _Std_StringBase			String;
	typedef _Std_WStringStreamBase	WStringStream;
	typedef _Std_StringStreamBase	StringStream;


	typedef _Std_OStringStream		ostringstream;
	typedef _Std_WOStringStream		wostringstream;
}

//for stl containter
namespace Cactus
{
	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct deque
	{
#if Ogre_Container_Use_MemAlloc
		typedef typename std::deque<T, A> type;
#else
		typedef typename std::deque<T> type;
#endif
	};


#if Ogre_Container_Use_MemAlloc
	template <typename T, typename C = std::deque<T, STLAllocator<T, GeneralAllocPolicy> > >
	struct queue
	{
		typedef typename std::queue<T, C> type;
	};
#else
	template <typename T, typename C = std::deque<T> >
	struct queue
	{
		typedef typename std::queue<T> type;
	};
#endif


#if Ogre_Container_Use_MemAlloc
	template <typename T, typename C = std::deque<T, STLAllocator<T, GeneralAllocPolicy> > >
	struct stack
	{
		typedef typename std::stack<T, C> type;
	};
#else
	template <typename T, typename C = std::deque<T> >
	struct stack
	{
		typedef typename std::stack<T> type;
	};
#endif


	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct vector
	{
#if Ogre_Container_Use_MemAlloc
		typedef typename std::vector<T, A> type;
#else
		typedef typename std::vector<T> type;
#endif
	};


	template <typename T, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct list
	{
#if Ogre_Container_Use_MemAlloc
	   typedef typename std::list<T, A> type;
#else
		typedef typename std::list<T> type;
#endif
	};


	template <typename T, typename P = std::less<T>, typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct set
	{
#if Ogre_Container_Use_MemAlloc
	   typedef typename std::set<T, P, A> type;
#else
		typedef typename std::set<T, P> type;
#endif
	};


	template <typename K, typename V, typename P = std::less<K>, typename A = STLAllocator<std::pair<const K, V>, GeneralAllocPolicy> >
	struct map
	{
#if Ogre_Container_Use_MemAlloc
	   typedef typename std::map<K, V, P, A> type;
#else
		typedef typename std::map<K, V, P> type;
#endif
	};


	template <typename K, typename V, typename P = std::less<K>, typename A = STLAllocator<std::pair<const K, V>, GeneralAllocPolicy> >
	struct multimap
	{
#if Ogre_Container_Use_MemAlloc
		typedef typename std::multimap<K, V, P, A> type;
#else
		typedef typename std::multimap<K, V, P> type;
#endif
	};


	template <typename K, typename V, typename P = __HashCmp(K), typename A = STLAllocator<std::pair<const K, V>, GeneralAllocPolicy> >
	struct hashmap
	{
#if Ogre_Container_Use_MemAlloc
		typedef typename __HashMap<K, V, P, A> type;
#else
		typedef typename __HashMap<K, V, P> type;
#endif
	};


	template <typename T, typename P = __HashCmp(T), typename A = STLAllocator<T, GeneralAllocPolicy> >
	struct hashset
	{
#if Ogre_Container_Use_MemAlloc
		typedef typename __HashSet<T, P, A> type;
#else
		typedef typename __HashSet<T, P> type;
#endif
	};


	typedef Cactus::vector<Cactus::String>::type	StringVector;

} // Cactus

#endif


