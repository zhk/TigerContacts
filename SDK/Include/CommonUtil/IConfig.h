/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			IConfig.h
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
	class COMMONUTIL_API IConfig
	{
	public:
		virtual ~IConfig(){}

		virtual bool GetBool( const char* section, const char* key, bool& value) = 0;
		virtual bool GetInt( const char* section, const char* key, int& value) = 0;
		virtual bool GetFloat( const char* section, const char* key, float& value) = 0;
		virtual bool GetString( const char* section, const char* key, Cactus::String& value) = 0;

		virtual void SetBool( const char* section, const char* key, bool value) = 0;
		virtual void SetInt( const char* section, const char* key, int value) = 0;
		virtual void SetFloat( const char* section, const char* key, float value) = 0;
		virtual void SetString( const char* section, const char* key, const Cactus::String& value) = 0;

		virtual bool SetSource(const Cactus::String& path) = 0;
		virtual void SaveConfig() = 0;
	};
}