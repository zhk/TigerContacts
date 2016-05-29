/*====================================================================
	Created Date  :		2006-5-22
	Created By	  :		Zhang Kun
	
	Specification :		From internet
====================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	// Base64
	COMMONUTIL_API Cactus::String base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	COMMONUTIL_API Cactus::String base64_decode(Cactus::String const& encoded_string);

	// Quoted-Printable
	COMMONUTIL_API Cactus::String QP_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
	COMMONUTIL_API Cactus::String QP_decode(Cactus::String const& encoded_string);
}