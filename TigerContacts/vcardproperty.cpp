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
 * $Revision: 12 $
 * $Date: 2011-02-20 15:00:41 +0100 (Sun, 20 Feb 2011) $
 */

#include "stdafx.h"
#include "vcardproperty.h"

using namespace Cactus;

int vCardProperty::s_iEditorIndex = 0;

#define VC_ASSIGNMENT_TOKEN ':'

void vCardProperty::_PatchProperty()
{
	//强制进行UTF-8编码
	if (m_name == VC_NAME
		|| m_name == VC_FORMATTED_NAME
		|| m_name == VC_NICKNAME
		|| m_name == VC_NOTE
		|| m_name == VC_ADDRESS
		|| m_name == VC_ORGANIZATION
		|| m_name == VC_TITLE
		|| m_name == VC_CATEGORIES
		)
	{
		bool bFound = false;
		M_Foreach(vCardParam, param, vCardParamList, m_params)
		{
			if (param.group() == vCardParam::Charset)
			{
				if (param.value() != VC_CHARSET_UTF_8)
					param.value(VC_CHARSET_UTF_8);

				bFound = true;
				break;
			}
		}

		if (!bFound)
			m_params.push_back(vCardParam(VC_CHARSET_UTF_8, vCardParam::Charset));


		//去掉VC_ENCODING_QP
		for(vCardParamList::iterator it = m_params.begin(); it != m_params.end(); )
		{
			vCardParam param = *it;

			if (param.group() == vCardParam::Encoding && param.value() == VC_ENCODING_QP)
			{
				M_Foreach(GString, val, GStringVector, m_values)
				{
					String strQP	= val.ToMbcs();
					String strUTF8	= QP_decode(strQP);
					GString gStr(strUTF8, GString::eSE_Utf8);
					val = gStr;
				}

				it = m_params.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	if (m_name == VC_TELEPHONE)
	{
		// Change MAIN to PREF
		M_Foreach(vCardParam, param, vCardParamList, m_params)
		{
			if (param.group() == vCardParam::Type && param.value() == "MAIN")
			{
				param.value("PREF");
				break;
			}
		}
	
	}

	// Remove "INTERNET" for EMAIL
	if (m_name == VC_EMAIL)
	{
		for(vCardParamList::iterator it = m_params.begin(); it != m_params.end(); ++it)
		{
			vCardParam param = *it;
			if (param.group() == vCardParam::Type && param.value() == "INTERNET")
			{
				m_params.erase(it);
				break;
			}
		}
	}
}


vCardProperty::vCardProperty()
: _bIsExtProp(false)
, _iEditorIndex(s_iEditorIndex++)
{
}

vCardProperty::vCardProperty(const Cactus::GString& name, const Cactus::GString& value, const vCardParamList& params)
: m_name(name)
, m_params(params)
, _bIsExtProp(false)
, _iEditorIndex(s_iEditorIndex++)
{
    m_values = value.Split(VC_SEPARATOR_TOKEN);

	_PatchProperty();
}

vCardProperty::vCardProperty(const Cactus::GString& name, const Cactus::GStringVector& values, const vCardParamList& params)
: m_name(name)
, m_values(values)
, m_params(params)
, _bIsExtProp(false)
, _iEditorIndex(s_iEditorIndex++)
{
	_PatchProperty();
}

vCardProperty::vCardProperty(const Cactus::GString& name, const Cactus::GString& value, const Cactus::GString& params)
: m_name(name)
, _bIsExtProp(false)
, _iEditorIndex(s_iEditorIndex++)
{
    m_values = value.Split(VC_SEPARATOR_TOKEN);
    m_params = vCardParam::fromGString(params);

	_PatchProperty();
}

vCardProperty::vCardProperty(const Cactus::GString& name, const Cactus::GStringVector& values, const Cactus::GString& params)
: m_name(name)
, m_values(values)
, _bIsExtProp(false)
, _iEditorIndex(s_iEditorIndex++)
{
	if (params.Str().length() > 0)
		m_params = vCardParam::fromGString(params);

	_PatchProperty();
}

vCardProperty::~vCardProperty()
{
}


Cactus::GString vCardProperty::name() const
{
    return m_name;
}

Cactus::GString vCardProperty::value(const Cactus::GString& separated/* = VC_SEPARATOR_TOKEN*/) const
{
    return Cactus::GString(m_values, separated);
}

Cactus::GStringVector vCardProperty::values() const
{
    return m_values;
}

Cactus::GStringVector& vCardProperty::values()
{
	return m_values;
}

vCardParamList vCardProperty::params() const
{
    return m_params;
}

void vCardProperty::params(const vCardParamList& params)
{
	m_params = params;
}

bool vCardProperty::isValueValid() const
{
	if (m_values.size() == 0)
		return false;

	bool bAllNull = true;
	M_Foreach_Const (GString, gStr, GStringVector, m_values)
		if (gStr.Str().size() != 0)
		{
			bAllNull = false;
			break;
		}

		if (bAllNull)
			return false;

	return true;
}


bool vCardProperty::isValid() const
{
    if (m_name.Str().length() == 0)
        return false;

    if (m_values.size() == 0)
        return false;

	if (!isValueValid())
		return false;

    M_Foreach_Const (vCardParam, param, vCardParamList, m_params)
        if (!param.isValid())
            return false;

    return true;
}

bool vCardProperty::operator== (const vCardProperty& prop) const
{
    return ((m_name == prop.name()) && (m_values == prop.values()));
}

bool vCardProperty::operator!= (const vCardProperty& prop) const
{
    return ((m_name != prop.name()) || (m_values != prop.values()));
}

//对过长的属性字符串进行分行
GString LimitLineLength(GString& str)
{
	int szLeftLen = str.Str().length();
	if (szLeftLen <= 74)
		return str;

	GString buffer = str.Left(74);
	szLeftLen -= 74;

	while(szLeftLen > 0)
	{
		str = str.Right(szLeftLen);

		buffer += VC_END_LINE_TOKEN;
		buffer += " ";
		buffer += str.Left(73);

		szLeftLen -= (73);
	}

	return buffer;
}

GString vCardProperty::toGString(int& iIndex, EvCardVersion version/* = VC_VER_3_0*/, EvCardUsage usage/* = VC_Usage_Android_Apple*/) const
{
    GString buffer;

    switch (version)
    {
        case VC_VER_2_1:
        case VC_VER_3_0:
        {
			GString name = m_name;
			name.ToUpper();

			if (!_bIsExtProp)
			{
				buffer += name;

				GString params = vCardParam::toGString(name, m_params, version, usage);
				if (params.Str().length() != 0)
				{
					buffer += VC_SEPARATOR_TOKEN;
					buffer += params;
				}

				buffer += Cactus::GString(VC_ASSIGNMENT_TOKEN);
				buffer += Cactus::GString(m_values, VC_SEPARATOR_TOKEN);
			}
			else
			{
				GString gStr;
				gStr.Format(L"ITEM%d.", iIndex);

				// Line 1
				buffer += gStr + name;

				buffer += Cactus::GString(VC_ASSIGNMENT_TOKEN);
				buffer += Cactus::GString(m_values, VC_SEPARATOR_TOKEN);

				// Line 2
				buffer += VC_END_LINE_TOKEN;
				buffer += gStr + "X-ABLABEL";
				buffer += Cactus::GString(VC_ASSIGNMENT_TOKEN);
				buffer += _strExtLabelValue;

				iIndex++;
			}
        }
        break;

        default:
            break;
    }

    return LimitLineLength(buffer);
}

//static
vCardPropertyList vCardProperty::fromGString(const GString& gStr)
{
	Cactus::GStringVector lines = gStr.Split(VC_END_LINE_TOKEN);
	vCardPropertyList properties;

	M_Foreach (Cactus::GString, line, Cactus::GStringVector, lines)
	{
		if (line == VC_BEGIN_TOKEN || line == VC_END_TOKEN)
			break;

		Cactus::GStringVector tokens = line.Split(VC_ASSIGNMENT_TOKEN);	// :
		if (tokens.size() >= 2)
		{
			Cactus::GStringVector property_tokens = tokens.at(0).Split(VC_SEPARATOR_TOKEN);	// ;

			Cactus::GString name = property_tokens.at(0);
			name.ToUpper();

			property_tokens.erase(property_tokens.begin());

			if (name != VC_VERSION)
			{
				vCardParamList params;
				if (property_tokens.size() > 0)
					params = vCardParam::fromGString( Cactus::GString(property_tokens, VC_SEPARATOR_TOKEN) );	// ;

				properties.push_back(vCardProperty(name, tokens.at(1), params));
			}
		}
	}

	return properties;
}

vCardProperty vCardProperty::createAddress(const Cactus::GString& street
										   , const Cactus::GString& locality
										   , const Cactus::GString& region
										   , const Cactus::GString& postal_code
										   , const Cactus::GString& country
										   , const Cactus::GString& post_office_box
										   , const Cactus::GString& ext_address
										   , const vCardParamList& params)
{
    Cactus::GStringVector values;
    values.push_back(post_office_box);
    values.push_back(ext_address);
    values.push_back(street);
    values.push_back(locality);
    values.push_back(region);
    values.push_back(postal_code);
    values.push_back(country);

    return vCardProperty(VC_ADDRESS, values, params);
}

//vCardProperty vCardProperty::createBirthday(const QDate& birthday, const vCardParamList& params)
//{
//    return vCardProperty(VC_BIRTHDAY, birthday.toString("yyyy-MM-dd"), params);
//}
//
//vCardProperty vCardProperty::createBirthday(const QDateTime& birthday, const vCardParamList& params)
//{
//    return vCardProperty(VC_BIRTHDAY, birthday.toString("yyyy-MM-ddThh:mm:ssZ"), params);
//}

vCardProperty vCardProperty::createGeographicPosition(float latitude
													  , float longitude
													  , const vCardParamList& params)
{
    Cactus::GStringVector values;

	GString gStr("%1");
	gStr.Arg(latitude);
    values.push_back(gStr);

	gStr = GString("%1");
	gStr.Arg(longitude);
	values.push_back(gStr);

    return vCardProperty(VC_GEOGRAPHIC_POSITION, values, params);
}

vCardProperty vCardProperty::createName(const Cactus::GString& firstname
										, const Cactus::GString& lastname
										, const Cactus::GString& additional
										, const Cactus::GString& prefix
										, const Cactus::GString& suffix
										, const vCardParamList& params)
{
    Cactus::GStringVector values;
    values.push_back(lastname);
    values.push_back(firstname);
    values.push_back(additional);
    values.push_back(prefix);
    values.push_back(suffix);

    return vCardProperty(VC_NAME, values, params);
}

vCardProperty vCardProperty::createOrganization(const Cactus::GString& name
												, const Cactus::GStringVector& levels
												, const vCardParamList& params)
{
    Cactus::GStringVector values;
    values.push_back(name);
    values.insert(values.end(), levels.begin(), levels.end());

    return vCardProperty(VC_ORGANIZATION, values, params);
}

bool vCardProperty::isUniqueProperty( const Cactus::GString& name )
{
	return (
		name == VC_NAME
		|| name == VC_NICKNAME
		|| name == VC_FORMATTED_NAME
		|| name == VC_ORGANIZATION
		|| name == VC_TITLE
		|| name == VC_PHOTO
		|| name == VC_REVISION
		|| name == VC_CATEGORIES
		);
}

