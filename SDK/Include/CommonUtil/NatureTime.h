/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			NatureTime.h
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

	//----------------------------------------------------------------
	//自然时间类，内部以总秒数表示
	class COMMONUTIL_API NatureTime
	{
	public:
		enum
		{
			eNatureTimeSecond	= 1,
			eNatureTimeMinute	= eNatureTimeSecond * 60,
			eNatureTimeHour		= eNatureTimeMinute * 60,
			eNatureTimeDay		= eNatureTimeHour * 24,
			eNatureTimeMonth	= eNatureTimeDay * 30,
			eNatureTimeYear		= eNatureTimeMonth * 12,
		};

		enum
		{
			eStartYear = 2000
		};

		NatureTime(){ _iTotalSecond = 0; }
		NatureTime(int Y, int Mon, int D, int H, int M, int S)
		{
			_iTotalSecond = Y * eNatureTimeYear
				+ Mon * eNatureTimeMonth
				+ D * eNatureTimeDay
				+ H * eNatureTimeHour
				+ M * eNatureTimeMinute
				+ S * eNatureTimeSecond;
		}
		NatureTime(const NatureTime& r){ _iTotalSecond = r._iTotalSecond; }
		NatureTime(int S){ _iTotalSecond = S; }

		// 2009-10-28 12:32:45
		NatureTime(const Cactus::String& str);

		NatureTime& operator+(const NatureTime& r){ _iTotalSecond += r._iTotalSecond; return *this; }
		NatureTime& operator-(const NatureTime& r){ _iTotalSecond -= r._iTotalSecond; return *this; }

		NatureTime& operator+(int S){ _iTotalSecond += S; return *this; }
		NatureTime& operator-(int S){ _iTotalSecond -= S; return *this; }

		NatureTime& operator+=(int S){ _iTotalSecond += S; return *this; }
		NatureTime& operator-=(int S){ _iTotalSecond -= S; return *this; }

		bool		operator==(const NatureTime& r){ return _iTotalSecond == r._iTotalSecond; }
		bool		operator==(int S){ return _iTotalSecond == S; }

		operator int() const { return _iTotalSecond; }

		int		GetTotalSecond(){ return _iTotalSecond; }

		int		GetYear()	const { return _iTotalSecond/eNatureTimeYear + eStartYear; }
		int		GetMonth()	const { return (_iTotalSecond%eNatureTimeYear)/eNatureTimeMonth; }
		int		GetDay()	const { return ((_iTotalSecond%eNatureTimeYear)%eNatureTimeMonth)/eNatureTimeDay; }
		int		GetHour()	const { return (((_iTotalSecond%eNatureTimeYear)%eNatureTimeMonth)%eNatureTimeDay)/eNatureTimeHour; }
		int		GetMinute()	const { return ((((_iTotalSecond%eNatureTimeYear)%eNatureTimeMonth)%eNatureTimeDay)%eNatureTimeHour)/eNatureTimeMinute; }
		int		GetSecond()	const { return _iTotalSecond%eNatureTimeMinute; }

		Cactus::String	ToString() const;

		static NatureTime	Now();

	protected:
		int		_iTotalSecond;
	};


}
