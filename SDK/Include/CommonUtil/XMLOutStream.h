/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			XMLOutStream.h
*	Description:
*			A simple way to read xml document(From CEGUI.)
*	Revision history:
*			2008-1-3	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	class IDataOutStream;

	class COMMONUTIL_API XMLOutStream
	{
	public:

		// Output to file stream
		XMLOutStream(IDataOutStream* pStream)
			: _iIdent(0)
			, _pOutStream(pStream)
		{
		}

		// Output to memory
		XMLOutStream()
			: _iIdent(0)
		{
		}

		Cactus::String	GetBuffer(){ return _Stream.str(); }
		void			ClearBuffer();
		void			Flush();

		void			NodeBegin(const Cactus::String& strName);
		void			NodeEnd(const Cactus::String& strName);

		template<class vType>
		inline void		AddAttribute(const Cactus::String& strKey, const vType& val)
		{
			_Stream << strKey << "=\"" << val << "\" ";
		}

	protected:

		Cactus::ostringstream	_Stream;
		int						_iIdent;
		Cactus::String			_strCurNode;
		IDataOutStream*			_pOutStream;
	};

    template<>
    inline void XMLOutStream::AddAttribute(const Cactus::String& strKey, const bool& val)
    {
        if (val)
            _Stream << strKey << "=\"true\" ";
        else
            _Stream << strKey << "=\"false\" ";
    }

}