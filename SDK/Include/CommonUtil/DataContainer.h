/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			DataContainer.h
*	Description:
*			
*	Revision history:
*			2007-7-13	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	/*!
	\brief
	Class used as the databuffer for loading files throughout the library.
	*/
	class COMMONUTIL_API RawDataContainer : public Object
	{
	public:

		RawDataContainer()
			: _pData(0)
			, _szSize(0)
		{
		}

		~RawDataContainer(void) 
		{ 
			release();
		}

		/*!
		\brief
		Set a pointer to the external data.

		\param data
		Pointer to the uint8 data buffer.
		*/
		void setData(unsigned char* data) { _pData = data; }

		/*!
		\brief
		Return a pointer to the external data

		\return
		Pointer to an the uint8 data buffer.
		*/
		unsigned char* getDataPtr(void) { return _pData; }
		const unsigned char* getDataPtr(void) const { return _pData; }

		/*!
		\brief
		Set the size of the external data.

		\param size
		size_t containing the size of the external data
		*/
		void setSize(size_t size) { _szSize = size; }

		/*!
		\brief
		Get the size of the external data.

		\return
		size_t containing the size of the external data
		*/
		size_t getSize(void) const { return _szSize; }

		/*!
		\brief
		Release supplied data.
		*/
		void release(void);

	private:

		unsigned char*	_pData;
		size_t			_szSize;
	};

}