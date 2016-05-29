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
		�������зֽ�Ϊ���غͲ��������ؿ��صĸ�����
		*/
		size_t			Parse(int argc, char **argv);

		/**
		����������Ƿ����ָ���Ŀ��ء�
		����:
		������: test.exe --a p1 p2 p3 --b p4 -c -d p5
		HasSwitch("--a")      ����true
		HasSwitch("-z")       ����false
		*/
		bool			HasSwitch(const char *pSwitch);

		/**
		��ȡһ�����صĲ�������������ڣ�����ָ����ȱʡֵ��
		����:
		������: test.exe --a p1 p2 p3 --b p4 -c -d p5
		GetSafeArgument("--a", 0, "default")		����p1
		GetSafeArgument("--a", 1, "default")		����p2
		GetSafeArgument("--b", 0, "default")		����p4
		GetSafeArgument("--b", 1, "default")		����default
		*/

		Cactus::String	GetSafeArgument(const char *pSwitch, const char *pDefault = 0, int iIdx = 0);

		/**
		��ȡһ�����صĲ�������������ڣ��׳��쳣��
		����:
		������: test.exe --a p1 p2 p3 --b p4 -c -d p5
		GetArgument("--a", 0)     ����p1
		GetArgument("--b", 1)     �׳��쳣std::exception("Exceed the size!"), ���ؿմ���
		GetArgument("--e", 0)     �׳��쳣std::exception("No such switch!"), ���ؿմ���
		*/
		Cactus::String	GetArgument(const char *pSwitch, int iIdx = 0); 

		/**
		����һ�����صĲ������������û�ҵ�����-1��
		*/
		int				GetArgumentCount(const char *pSwitch);

	protected:

		/**
		�Ƿ��ǿ��أ���������-��ʼ�ģ����治�������֡�
		*/
		bool        _IsSwitch(const char *pParam);

		typedef Cactus::map<Cactus::String, Cactus::StringVector >::type	SwitchValueMapType;
		SwitchValueMapType	_Switches;
	};
}