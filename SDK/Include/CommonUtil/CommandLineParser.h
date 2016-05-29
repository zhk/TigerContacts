/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommandLineParser.h
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
	class COMMONUTIL_API CommandLineParser
	{
	public:

		CommandLineParser();

		/**
		将命令行分解为开关和参数，返回开关的个数。
		*/
		size_t			Parse(int argc, char **argv);

		/**
		检查命令行是否包含指定的开关。
		例如:
		命令行: test.exe --a p1 p2 p3 --b p4 -c -d p5
		HasSwitch("--a")      返回true
		HasSwitch("-z")       返回false
		*/
		bool			HasSwitch(const char *pSwitch);

		/**
		获取一个开关的参数，如果不存在，返回指定的缺省值。
		例如:
		命令行: test.exe --a p1 p2 p3 --b p4 -c -d p5
		GetSafeArgument("--a", 0, "default")		返回p1
		GetSafeArgument("--a", 1, "default")		返回p2
		GetSafeArgument("--b", 0, "default")		返回p4
		GetSafeArgument("--b", 1, "default")		返回default
		*/

		Cactus::String	GetSafeArgument(const char *pSwitch, const char *pDefault = 0, int iIdx = 0);

		/**
		获取一个开关的参数，如果不存在，抛出异常。
		例如:
		命令行: test.exe --a p1 p2 p3 --b p4 -c -d p5
		GetArgument("--a", 0)     返回p1
		GetArgument("--b", 1)     抛出异常std::exception("Exceed the size!"), 返回空串。
		GetArgument("--e", 0)     抛出异常std::exception("No such switch!"), 返回空串。
		*/
		Cactus::String	GetArgument(const char *pSwitch, int iIdx = 0); 

		/**
		返回一个开关的参数数量，如果没找到返回-1。
		*/
		int				GetArgumentCount(const char *pSwitch);

	protected:

		/**
		是否是开关，开关是以-开始的，后面不能是数字。
		*/
		bool        _IsSwitch(const char *pParam);

		typedef Cactus::map<Cactus::String, Cactus::StringVector >::type	SwitchValueMapType;
		SwitchValueMapType	_Switches;
	};
}