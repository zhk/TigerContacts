/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			FileConfig.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "IConfig.h"

namespace Cactus
{
	class COMMONUTIL_API FileConfig : public IConfig, public Object
	{
	public:
		struct SPair
		{
			SPair(Cactus::String f, Cactus::String s)
				: first(f)
				, second(s)
			{
			}
			Cactus::String	first;
			Cactus::String	second;
		};

		// Key, Value
		typedef Cactus::list<SPair >::type SectionListType;
		// SectionName, SectionValue
		typedef Cactus::map<Cactus::String, SectionListType >::type	ConfigMapType;

	public:
		FileConfig();
		virtual ~FileConfig();

		virtual bool GetBool( const char* section, const char* key, bool& value);
		virtual bool GetInt( const char* section, const char* key, int& value);
		virtual bool GetFloat( const char* section, const char* key, float& value);
		virtual bool GetString( const char* section, const char* key, Cactus::String& value);

		virtual void SetBool( const char* section, const char* key, bool value);
		virtual void SetInt( const char* section, const char* key, int value);
		virtual void SetFloat( const char* section, const char* key, float value);
		virtual void SetString( const char* section, const char* key, const Cactus::String& value);

		virtual bool SetSource(const Cactus::String& path);
		virtual void SaveConfig();

		bool		 IsValid() const { return _valid; }

		SectionListType		GetSection(const Cactus::String& section) const;
		SectionListType&	GetSection(const Cactus::String& section);

	protected:
		bool _valid;
		bool _saveChanges;
		bool _inUTF8;

		Cactus::String _strPathName;

		enum ELineType
		{
			eLineTypeEmpty,
			eLineTypeNote,
			eLineTypeSection,
			eLineTypeValue
		};

		ELineType	ParseString(Cactus::String& str, Cactus::String& p1, Cactus::String& p2);

		ConfigMapType _configs;
	};
}
