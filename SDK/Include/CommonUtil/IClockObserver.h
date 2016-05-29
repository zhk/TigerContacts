/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			IClockObserver.h
*	Description:
*			A clock used for game update(From Game Programming Gems.)
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	class COMMONUTIL_API IClockObserver
	{
	public:
		virtual ~IClockObserver() {}
		virtual void Notify(double detaTime) = 0;
	};
}

