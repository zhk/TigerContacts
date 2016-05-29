/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Singleston.h
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
	/* Copyright (C) Scott Bilas, 2000. 
	* All rights reserved worldwide.
	*
	* This software is provided "as is" without express or implied
	* warranties. You may freely copy and compile this source into
	* applications you distribute provided that the copyright text
	* below is included in the resulting source code, for example:
	* "Portions Copyright (C) Scott Bilas, 2000"
	*/

	template <typename T>
	class Singleton
	{
	protected:
		static T* s_pSingleton;

	public:
		Singleton( void )
		{
			assert( !s_pSingleton );
			s_pSingleton = static_cast<T*>(this);
		}

		~Singleton( void )
		{
			assert( s_pSingleton );
			s_pSingleton = 0;
		}

		static T& getSingleton( void )
		{
			assert( s_pSingleton ); 
			return ( *s_pSingleton );
		}

		static T* getSingletonPtr( void )
		{
			return s_pSingleton;
		}
	};

}

