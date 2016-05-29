/*===============================================================================
*	Copyright (C) 2006-2009, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			UIStringTableT.h
*	Description:
*			
*	Revision history:
*			2009-7-6	* Created by Zhang Kun
*				Created
===============================================================================*/

#pragma once

#include "PreInclude.h"
#include "XMLDocHandler.h"
#include "Logger.h"

namespace Cactus
{

	template<int ColumnCount>
	class UIStringTableT : public Cactus::XMLHandler
	{
	public:
		UIStringTableT(const Cactus::String& strColWanted, bool bLoadAll = false)
		{
			_strColWanted	= strColWanted;
			_bLoadAll		= bLoadAll;
			_iColCursor		= 0;
		}

		void		PushColumnName(const Cactus::String& strName)
		{
			assert(_iColCursor < ColumnCount);

			_ColumnNames[strName] = _iColCursor++;
		}

		bool		Load(const Cactus::String& strPathName)
		{
			if(_iColCursor != ColumnCount)
			{
				Log_Error("UIStringTableT::Load - ColumnNames are not enough, push more column name!");
				return false;
			}

			NameColumnMapType::iterator it = _ColumnNames.find(_strColWanted);
			if (it == _ColumnNames.end())
			{
				Log_Error("UIStringTableT::Load - wanted column not exist " << _strColWanted);
				return false;
			}
			_iColWanted = it->second;

			// do parse (which uses handler to create actual data)
			try
			{
				Cactus::XMLParser xmlParser(*this, strPathName, "");
			}
			catch (exception e)
			{
				Log_Error("UIStringTableT::Load - loading of StringTable from file '" << strPathName << "' failed!");
				return false;
			}

			return true;
		}

		Cactus::String	GetString(const Cactus::String& strKey)
		{
			if ( _StringTable.find(strKey) != _StringTable.end() )
			{
				return _StringTable[strKey]._strValues[_iColWanted];
			}
			else
			{
				Log_Error("UIStringTableT::GetString, Local string not found for : " << strKey);
				return "NotFound!";
			}
		}

	private:

		virtual void elementStart(const Cactus::String& element, const Cactus::XMLAttributes& attributes)
		{
			if (element == "Item")
			{
				Cactus::String	strKey = attributes.getValueAsString("Key");

				if(_bLoadAll)
				{
					SLocaleValues vals;

					NameColumnMapType::iterator it = _ColumnNames.begin();
					for (; it != _ColumnNames.end(); it++)
					{
						vals._strValues[it->second] = attributes.getValueAsString(it->first);
					}

					_StringTable[strKey] = vals;
				}
				else
				{
					Cactus::String strTmp = attributes.getValueAsString(_strColWanted);

					if (strKey.size() != 0 && strTmp.size() != 0 )
					{
						SLocaleValues vals;
						vals._strValues[_iColWanted] = strTmp;

						_StringTable[strKey] = vals;
					}
				}
			}
		}

		virtual void elementEnd(const Cactus::String& element)
		{
		}

		virtual void text(const Cactus::String& element, const Cactus::String& content)
		{
		}


		struct SLocaleValues
		{
			Cactus::String	_strValues[ColumnCount];
		};

		typedef	typename Cactus::hashmap<Cactus::String, SLocaleValues>::type	StringTableType;
		StringTableType		_StringTable;

		typedef Cactus::hashmap<Cactus::String, int>::type				NameColumnMapType;
		NameColumnMapType	_ColumnNames;

		Cactus::String		_strColWanted;
		int					_iColWanted;
		bool				_bLoadAll;
		int					_iColCursor;
	};

}