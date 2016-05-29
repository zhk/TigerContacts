/**
*
* This file is part of the libvcard project.
*
* Copyright (C) 2010, Emanuele Bertoldi (Card Tech srl).
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
* $Revision: 10 $
* $Date: 2010-10-18 10:50:23 +0200 (Mon, 18 Oct 2010) $
*/

#include "stdafx.h"
#include "vcardparam.h"

using namespace Cactus;


#define VC_GROUP_TOKEN		"%1=%2"
#define VC_TYPE_TOKEN		"TYPE"
#define VC_TYPE_SEP_TOKEN	','
#define VC_ENCODING_TOKEN	"ENCODING"
#define VC_CHARSET_TOKEN	"CHARSET"

vCardParam::vCardParam()
: m_group(vCardParam::Undefined)
{
}

vCardParam::vCardParam(const Cactus::GString& value, EvCardParamGroup group)
: m_group(group)
, m_value(value)
{
}

vCardParam::~vCardParam()
{
}

vCardParam::EvCardParamGroup vCardParam::group() const
{
	return m_group;
}

Cactus::GString vCardParam::value() const
{
	return m_value;
}

bool vCardParam::isValid() const
{
	return m_value.Str().length() != 0;
}

bool vCardParam::operator== (const vCardParam& param) const
{
	return ((m_group == param.group()) && (m_value == param.value()));
}

bool vCardParam::operator!= (const vCardParam& param) const
{
	return ((m_group != param.group()) || (m_value != param.value()));
}

Cactus::GString vCardParam::toGString(EvCardVersion version/* = VC_VER_3_0*/, EvCardUsage usage/* = VC_Usage_Android_Apple*/) const
{
	Cactus::GString buffer;

	switch (version)
	{
	case VC_VER_2_1:
		/*
		TEL;WORK;FAX:1-304-060-7827
		TEL;HOME;FAX:337-240-20
		*/
		{
			switch (m_group)
			{
			case vCardParam::Charset:
				{
					GString gStr(VC_GROUP_TOKEN);
					gStr.Arg(VC_CHARSET_TOKEN);
					gStr.Arg(m_value);
				
					buffer += gStr;
				}
				break;

			case vCardParam::Encoding:
				{
					GString gStr(VC_GROUP_TOKEN);
					gStr.Arg(VC_ENCODING_TOKEN);
					gStr.Arg(m_value);

					buffer += gStr;
				}
				break;

			default:
				buffer += m_value;
				break;
			}

		}
		break;

	case VC_VER_3_0:
		/*
		TEL;TYPE=WORK;TYPE=FAX:1-304-060-7827
		TEL;TYPE=HOME;TYPE=FAX:337-240-20
		*/
		{
			switch (m_group)
			{
			case vCardParam::Type:
				{
					GString gStr(VC_GROUP_TOKEN);
					gStr.Arg(VC_TYPE_TOKEN);
					gStr.Arg(m_value);

					buffer += gStr;
				}
				break;

			case vCardParam::Charset:
				{
					GString gStr(VC_GROUP_TOKEN);
					gStr.Arg(VC_CHARSET_TOKEN);
					gStr.Arg(m_value);

					buffer += gStr;
				}
				break;

			case vCardParam::Encoding:
				{
					GString gStr(VC_GROUP_TOKEN);
					gStr.Arg(VC_ENCODING_TOKEN);
					gStr.Arg(m_value);

					buffer += gStr;
				}
				break;

			default:
				buffer += m_value;
				break;
			}
		}
		break;

	default:
		break;
	}

	buffer.ToUpper();

	return buffer;
}

//static
Cactus::GString vCardParam::toGString(const Cactus::GString& name
									  , const vCardParamList& params
									  , EvCardVersion version/* = VC_VER_3_0*/
									  , EvCardUsage usage/* = VC_Usage_Android_Apple*/)
{
	bool bSkipUtf8 = false;
	if (usage == VC_Usage_Android_Apple)
	{
		if (name == VC_NAME || name == VC_FORMATTED_NAME)
			bSkipUtf8 = true;
	}
	else if (usage == VC_Usage_Outlook)
	{
		bSkipUtf8 = true;
	}
	

	Cactus::GString buffer;

	switch (version)
	{
	case VC_VER_2_1:
		{
			Cactus::GStringVector ps;
			M_Foreach_Const (vCardParam, param, vCardParamList, params)
				ps.push_back(param.toGString(VC_VER_2_1, usage));

			buffer += GString(ps, Cactus::GString(VC_SEPARATOR_TOKEN));
		}
		break;

	case VC_VER_3_0:
		{
			Cactus::GStringVector types;
			Cactus::GStringVector encodings;
			Cactus::GStringVector charsets;
			Cactus::GStringVector unknowns;
			M_Foreach_Const (vCardParam, param, vCardParamList, params)
			{
				Cactus::GString param_str = param.toGString(VC_VER_3_0, usage);
				switch (param.group())
				{
				case Type:
					types.push_back(param_str);
					break;

				case Encoding:
					encodings.push_back(param_str);
					break;

				case Charset:
					if (!bSkipUtf8)
						charsets.push_back(param_str);
					break;

				default:
					unknowns.push_back(param_str);
				}
			}

			unknowns.insert( unknowns.end(), charsets.begin(), charsets.end() );
			unknowns.insert( unknowns.end(), encodings.begin(), encodings.end() );

			if (types.size() > 0)
			{
#if 0
				// 内部存储为2.1格式
				GString gStr(VC_GROUP_TOKEN);
				gStr.Arg(VC_TYPE_TOKEN);
				gStr.Arg( GString(types, Cactus::GString(VC_TYPE_SEP_TOKEN)) );		// ,

				unknowns.insert(unknowns.begin(), gStr);
#else
				// 内部存储为3.0格式
				unknowns.insert(unknowns.begin(), types);
#endif
			}

			if (unknowns.size() > 0)
				buffer += GString(unknowns, Cactus::GString(VC_SEPARATOR_TOKEN));
		}
		break;

	default:
		break;
	}

	buffer.ToUpper();
	return buffer;
}

/*
2.1
TEL;CELL:1-381-786-1818
TEL;WORK:646-833-63815
TEL;WORK,FAX:1-304-060-7827
TEL;HOME,FAX:337-240-20

3.0
TEL;TYPE=CELL:1-381-786-1818
TEL;TYPE=WORK:646-833-63815
TEL;TYPE=WORK;TYPE=FAX:1-304-060-7827
TEL;TYPE=HOME;TYPE=FAX:337-240-20
*/

vCardParamList vCardParam::fromGString(const Cactus::GString& gStr)
{
	Cactus::GStringVector tokens = gStr.Split(VC_SEPARATOR_TOKEN);
	vCardParamList params;

	M_Foreach (Cactus::GString, token, Cactus::GStringVector, tokens)
	{
		size_t token_size = token.Str().length();
		token.ToUpper();

		if (token.StartWith(VC_TYPE_TOKEN))
		{
			GStringVector rightParts = token.Right(token_size - 5).Split(VC_TYPE_SEP_TOKEN);	// ,
			M_Foreach (Cactus::GString, type, Cactus::GStringVector, rightParts)
			{
				type.ToUpper();
				params.push_back(vCardParam(type, vCardParam::Type));
			}
		}
		else if (token.StartWith(VC_ENCODING_TOKEN))
		{
			GString gStr = token.Right(token_size - 9);
			gStr.ToUpper();
			params.push_back(vCardParam(gStr, vCardParam::Encoding));
		}
		else if (token.StartWith(VC_CHARSET_TOKEN))
		{
			GString gStr = token.Right(token_size - 8);
			gStr.ToUpper();
			params.push_back(vCardParam(gStr, vCardParam::Charset));
		}
		else
		{
#if 0
			// 内部存储为2.1格式
			params.push_back(vCardParam(token));
#else
			// 内部存储为3.0格式
			GStringVector rightParts = token.Split(VC_TYPE_SEP_TOKEN);		// ,
			M_Foreach (Cactus::GString, type, Cactus::GStringVector, rightParts)
			{
				type.ToUpper();
				params.push_back(vCardParam(type, vCardParam::Type));
			}
#endif
		}
	}

	return params;
}
