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
#include "vcard.h"

using namespace Cactus;


vCard::vCard()
: _iImageIndex(0)
, _iMask(0)
{
}

vCard::vCard(const vCard& vcard)
{
   m_properties		= vcard.properties();
   _iImageIndex		= vcard._iImageIndex;
   _iMask			= vcard._iMask;
}

vCard::vCard(const vCardPropertyList& properties)
: m_properties(properties)
, _iImageIndex(0)
, _iMask(0)
{
}

vCard::~vCard()
{
}

bool vCard::addProperty(const vCardProperty& prop)
{
	for (vCardPropertyList::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
    {
        vCardProperty current = *it;
        if (current == prop)
		{
			return false;
		}
		else if (current.name() == prop.name() && current.params() == prop.params())
		{
			if( vCardProperty::isUniqueProperty( current.name() ) )
			{
				*it = prop;
				return true;
			}
        }
    }

    m_properties.push_back(prop);

	return true;
}

bool vCard::addProperties(const vCardPropertyList& properties)
{
	bool bModified = false;

	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, properties)
	{
		if (this->addProperty(prop))
		{
			bModified = true;
		}
		
	}

	return bModified;
}


bool vCard::pushProperty(const vCardProperty& prop)
{
	m_properties.push_back(prop);

	return true;
}

bool vCard::pushProperties(const vCardPropertyList& properties)
{
	bool bModified = false;

	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, properties)
	{
		if (this->pushProperty(prop))
		{
			bModified = true;
		}
	}

	return bModified;
}


bool vCard::removeProperty( const Cactus::GString& name )
{
	bool bFound = false;

	vCardPropertyList::iterator it = m_properties.begin();
	while ( it != m_properties.end() )
	{
		const vCardProperty current = *it;
		if (current.name() == name)
		{
			it = m_properties.erase(it);
			if(!bFound)
				bFound = true;
		}
		else
			++it;
	}

	if( bFound )
		updateReversion();

	return bFound;
}

bool vCard::removeProperty( int iEditorIndex )
{
	vCardPropertyList::iterator it = m_properties.begin();

	while ( it != m_properties.end() )
	{
		const vCardProperty current = *it;
		if (current.getEditorIndex() == iEditorIndex)
		{
			it = m_properties.erase(it);
			updateReversion();
			return true;
		}
		else
			++it;
	}

	return false;
}

vCardPropertyList vCard::properties() const
{
    return m_properties;
}

vCardProperty vCard::property(const Cactus::GString& name, const vCardParamList& params, bool strict) const
{
	for (vCardPropertyList::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
	{
		const vCardProperty current = *it;
        if (current.name() == name)
        {
            vCardParamList current_params = current.params();

            if (strict)
            {
                if (params != current_params)
                    continue;
            }
            else
            {
                M_Foreach_Const (vCardParam, param, vCardParamList, params)
					if (std::find(current_params.begin(), current_params.end(), param) == current_params.end())
                        continue;
            }

            return current;
        }
    }

    return vCardProperty();
}

bool vCard::contains(const Cactus::GString& name, const vCardParamList& params, bool strict) const
{
	for (vCardPropertyList::const_iterator it = m_properties.begin(); it != m_properties.end(); ++it)
	{
		const vCardProperty current = *it;

		if (current.name() != name)
            continue;

        vCardParamList current_params = current.params();

        if (strict)
        {
            if (params != current_params)
                continue;
        }

        else
        {
            M_Foreach_Const (vCardParam, param, vCardParamList, params)
				if (std::find(current_params.begin(), current_params.end(), param) == current_params.end())
					continue;
        }

        return true;
    }

    return false;
}

bool vCard::contains(const vCardProperty& prop) const
{
    return (std::find(m_properties.begin(), m_properties.end(), prop) != m_properties.end());
}

bool vCard::isValid() const
{
    if (m_properties.size() == 0)
        return false;

    M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
        if (!prop.isValid())
            return false;

    return true;
}

int vCard::getPropsCount(const Cactus::GString& propName/* = ""*/) const
{
	if (propName.Str().size() == 0)
	    return m_properties.size();

	int iCount = 0;
	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
		if (prop.name() == propName)
			iCount++;

	return iCount;
}

bool vCard::hasProperty(const Cactus::GString& propName) const
{
	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
	{
		if (prop.isValid() && prop.name() == propName)
			return true;
	}

	return false;
}

vCardProperty vCard::getProperty(const Cactus::GString& propName) const
{
	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
		if (/*prop.isValid() &&*/ prop.name() == propName)
			return prop;
	
	return vCardProperty();
}

vCardProperty* vCard::getProperty(int iEditorIndex)
{
	M_Foreach(vCardProperty, prop, vCardPropertyList, m_properties)
	{
		if (/*prop.isValid() &&*/ prop.getEditorIndex() == iEditorIndex)
			return &prop;
	}
	
	return 0;
}

vCardPropertyList vCard::getProperties(const Cactus::GString& propName) const
{
	vCardPropertyList propList;
	M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
		if (/*prop.isValid() &&*/ prop.name() == propName)
			propList.push_back(prop);

	return propList;
}

vCardPropertyList vCard::getProperties(const Cactus::GStringVector& propNames) const
{
	vCardPropertyList propList;
	M_Foreach_Const (GString, name, GStringVector, propNames)
	{
		M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
			if (/*prop.isValid() &&*/ prop.name() == name)
				propList.push_back(prop);
	}

	return propList;
}


Cactus::GString vCard::toGString(EvCardVersion version/* = VC_VER_3_0*/, EvCardUsage usage/* = VC_Usage_Android_Apple*/) const
{
    Cactus::GStringVector lines;

    lines.push_back(VC_BEGIN_TOKEN);

	int iIndex = 1;

    switch (version)
    {
        case VC_VER_2_1:
            lines.push_back(vCardProperty(VC_VERSION, "2.1").toGString(iIndex, version, usage));
            break;

        case VC_VER_3_0:
            lines.push_back(vCardProperty(VC_VERSION, "3.0").toGString(iIndex, version, usage));
            break;

        default:
            return GString();
    }

    M_Foreach_Const (vCardProperty, prop, vCardPropertyList, m_properties)
	{
		if (prop.isValid())	//空值不存盘
			lines.push_back(prop.toGString(iIndex, version, usage));
	}
   
    lines.push_back(VC_END_TOKEN);

	return GString(lines, VC_END_LINE_TOKEN);
}

bool vCard::saveToFile(const Cactus::String& filename) const
{
	IFileSystem* pFS = FileManager::GetInstance().GetDefaultFileSystem();
	IDataOutStreamPtr spOutStream = pFS->OpenFileWrite(filename);
	if (!spOutStream)
	{
		Log_Error("vCard::saveToFile - failed to write file: " << filename);
		return false;
	}

	String strOut = this->toGString().ToUtf8();
	spOutStream->Write((void*)strOut.c_str(), strOut.length());
   
    return false;
}

//static
vCardList vCard::fromByteArray(const Cactus::String& data, GString::EGStringEncoding charset)
{
    vCardList vcards;
    vCard current;
    bool started = false;

	GString gStr(data, charset);
	GStringVector lines = gStr.SplitByString(VC_END_LINE_TOKEN);

	if (lines.size() <= 2)
		lines = gStr.SplitByString(VC_END_LINE_TOKEN_LINUX);

	if (lines.size() <= 2)
		return vcards;


	GString lineCache;
	for (size_t t = 0; t < lines.size(); )
	{
		GString line = lines[t];

		line.Simplify();
		if (line.Str().size() == 0)
		{
			t++;
			continue;
		}

		if ((line == VC_BEGIN_TOKEN) && !started)
		{
			started = true;

			t++;
		}
		else if ((line == VC_END_TOKEN) && started)
		{
			if (lineCache.Str().length() != 0)
			{
				vCardPropertyList props = vCardProperty::fromGString(lineCache);
				if (props.size())
				{
					current.pushProperties(props);
				}

				lineCache = "";
			}

			current.normalize();

			Log_Debug("vCard::fromByteArray, add " << current.getValid_FN_N_Name().ToUtf8());

			vcards.push_back(current);
			current = vCard();
			started = false;

			t++;
		}
		else if (started)
		{
			//新行是新属性行，需要整理旧行属性
			if (lineCache.Str().length() != 0 && line.Str().find(L':') != WString::npos)
			{
				vCardPropertyList props = vCardProperty::fromGString(lineCache);
				if (props.size())
				{
					current.pushProperties(props);
				}

				lineCache = "";
			}


			GString lineNext	= (t < lines.size() - 1) ? lines[t + 1] : "";
			lineNext.Simplify();

			//下一行存在，并不是新属性行
			if (lineNext.Str().size() > 0 && lineNext.Str().find(L':') == WString::npos)
			{
				//(软换行)QP编码要求编码后每行不能超过76个字符。当超过这个限制时，将适用软换行，用”=”表示编码行的断行，后接CRLF。
				//然而有些VCF文件并不遵守76个字符规定。
				// length 40 --- ENCODING=QUOTED-PRINTABLE;CHARSET=utf-8:
				size_t szLen_0 = line.Str().size();
				if (szLen_0 > 40 && line.EndWith("="))
					line = line.Left(szLen_0 - 1);

				szLen_0 = lineNext.Str().size();
				if (lineNext.EndWith("="))
				{
					lineNext = lineNext.Left(szLen_0 - 1);
				}

				lineCache += line + lineNext;		//连接下一行

				t += 2;
			}
			else if (lineNext.Str().size() == 0)	//跳过空行
			{
				lineCache += line;

				t += 2;
			}
			else	//新属性行
			{
				lineCache += line;

				vCardPropertyList props = vCardProperty::fromGString(lineCache);
				if (props.size())
				{
					current.pushProperties(props);
				}

				lineCache = "";

				t++;
			}
		}
		else
		{
			t++;		//跳过无效行
		}

	}

   
   return vcards;
}

//static
vCardList vCard::fromFile(const Cactus::String& filename, EvCardUsage origin/* = VC_Usage_Android_Apple*/)
{
    vCardList vcards;

	IFileSystem* pFS = FileManager::GetInstance().GetDefaultFileSystem();
	IDataInStreamPtr spInStream = pFS->OpenFileRead( filename );
	if(!spInStream)
	{
		Log_Error("vCard::fromFile - failed to open file: " << filename);
		return vcards;
	}

	size_t sSize = spInStream->GetLength();
	if(sSize <= 0)
	{
		spInStream->Close();
		Log_Error("vCard::fromFile - empty file: " << filename);
	}

	char* pBuffer = Mem_Alloc_T(char, sSize + 1);
	memset(pBuffer, 0, sSize + 1);

	spInStream->Read(pBuffer, sSize);
	spInStream->Close();

	String strData(pBuffer, sSize);

	GString::EGStringEncoding charset = GString::eSE_Utf8;
	if (origin == VC_Usage_Android_Apple || origin == VC_Usage_Nokia_S60)
	{
	}
	else if (origin == VC_Usage_Outlook)
	{
		charset = GString::eSE_Mbcs;
	}
	else	//VC_Usage_Unknown
	{
		if (strData.find(VC_CHARSET_UTF_8) == String::npos)
			charset = GString::eSE_Mbcs;		
	}
	
	vcards = vCard::fromByteArray(strData, charset);

	Mem_Free(pBuffer);

    return vcards;
}

Cactus::GString vCard::getValid_FN_N_Name() const
{
	vCardProperty propFN = getProperty(VC_FORMATTED_NAME);
	if (propFN.isValid())
	{
		return propFN.values().at(0);
	}

	vCardProperty propN = getProperty(VC_NAME);
	if (propN.isValid())
	{
		//return propN.value(" ");

		Cactus::GStringVector& vals = propN.values();

		//0 - 4: 姓、名、中间名、前缀、后缀
		return vals[3] + vals[0] + vals[2] + vals[1] + vals[4];
	}

	return "";
}

bool vCard::Merge( const vCard& card )
{
	return addProperties( card.properties() );
}

bool vCard::normalize_N(int iType)
{
	bool bHandle = false;

	vCardProperty propN = getProperty(VC_NAME);
	if (propN.isValid())
	{
		Cactus::GStringVector& vals = propN.values();
		if (vals.size() < 5)
			return false;

		//姓、名、中间名、前缀、后缀
		switch(iType)
		{
		default:
		case 0:		//中间名到名

			if (vals[2] != "")
			{
				vals[1] = vals[2] + vals[1];
				vals[2] = "";

				addProperty(propN);
				bHandle = true;
			}

			break;

		case 1:		//中间名、名到姓

			if (vals[1] != "" || vals[2] != "")
			{
				vals[0] = vals[0] + vals[2] + vals[1];
				vals[1] = "";
				vals[2] = "";

				addProperty(propN);
				bHandle = true;
			}

			break;

		case 2:		//将姓名(N)的5部分，按照：前缀+姓+中间名+名+后缀 的顺序填充到显示姓名(FN)。

			vCardProperty propFN = getProperty(VC_FORMATTED_NAME);
			propFN.values()[0] = vals[3] + vals[0] + vals[2] + vals[1] + vals[4];

			addProperty(propFN);
			bHandle = true;

			break;
		}

		if (bHandle)
		{
			updateReversion();
		}
		return bHandle;
	}

	return false;
}

bool vCard::normalize_TEL(int iFlag)	//0x1 remove minus, 0x10 remove space, 0x100 remove +86
{
	bool bHandle1 = false;
	bool bHandle2 = false;

	M_Foreach (vCardProperty, prop, vCardPropertyList, m_properties)
	{
		if (prop.isValid() && prop.name() == VC_TELEPHONE)
		{

			Cactus::GStringVector& vals = prop.values();
			for (size_t t = 0; t < vals.size(); ++t)
			{
				Cactus::GString strTmp = vals[t];
				vals[t] = "";
				for (size_t l = 0; l < strTmp.Str().length(); ++l)
				{
					wchar_t wch = strTmp.Str()[l];
					if ( ( ((iFlag & 0x1) == 0x1) && wch == L'-' )		// Remove '-' in phone number
						||
						( ((iFlag & 0x10) == 0x10) && iswspace(wch) )		// Remove space in phone number
						)
					{
						bHandle1 = true;
					}
					else
					{
						vals[t] += strTmp.Str()[l];
					}
				}

				if (((iFlag & 0x100) == 0x100) && vals[t].StartWith("+86"))
				{
					vals[t] = vals[t].SubStr(3, vals[t].Str().length() - 3);

					bHandle2 = true;
				}
			}
		}
	}

	if(bHandle1 || bHandle2)
		updateReversion();

	return bHandle1 || bHandle2;
}


bool vCard::operator==(const vCard& right) const
{
	if (m_properties.size() != right.m_properties.size())
		return false;

	vCardProperty propL = getProperty(VC_NAME);
	vCardProperty propR = right.getProperty(VC_NAME);

	bool bValidL = propL.isValid();
	bool bValidR = propR.isValid();

	if (bValidL != bValidR)
		return false;

	if (bValidL && bValidR
		&&
		propL != propR
		)
	{
		return false;
	}
	

	return toGString() == right.toGString();
}

void vCard::normalize()
{
	//Make sure current has VC_NAME, VC_FORMATTED_NAME and VC_NICKNAME
	vCardProperty prop = getProperty(VC_NAME);
	if (!prop.isValid())
	{
		GStringVector values;
		int iCount = 5;
		while(iCount-- != 0)
			values.push_back("");
		addProperty(vCardProperty(VC_NAME, values));
	}
	else
	{
		GStringVector& values = prop.values();
		while(values.size() < 5)
			values.push_back("");
		addProperty(prop);
	}

	prop = getProperty(VC_FORMATTED_NAME);
	if (!prop.isValid())
		addProperty(vCardProperty(VC_FORMATTED_NAME, ""));

	prop = getProperty(VC_NICKNAME);
	if (!prop.isValid())
		addProperty(vCardProperty(VC_NICKNAME, ""));


	prop = getProperty(VC_ORGANIZATION);
	if (!prop.isValid())
		addProperty(vCardProperty(VC_ORGANIZATION, ""));

	prop = getProperty(VC_TITLE);
	if (!prop.isValid())
		addProperty(vCardProperty(VC_TITLE, ""));


	//Handle extend properties
	vCardPropertyList props;
	for (vCardPropertyList::iterator it = m_properties.begin(); it != m_properties.end(); )
	{
		if (it->name().StartWith("ITEM"))
		{
			props.insert(props.end(), *it);
			it = m_properties.erase(it);
		}
		else
		{
			++it;
		}
	}

	//ITEM3.TEL:000000
	//ITEM3.X-ABLABEL:自定义
	int iCount = props.size() / 2;
	int iIndex = 0;
	while(iIndex < iCount)
	{
		GString gStr;
		gStr.Format(L"ITEM%d", iIndex + 1);

		vCardProperty prop;

		for (vCardPropertyList::iterator it = props.begin(); it != props.end(); )
		{
			if (it->name().StartWith(gStr))
			{
				prop = *it;
				prop.setExtendProp(true);
				it = props.erase(it);
				break;
			}

			++it;
		}

		for (vCardPropertyList::iterator it = props.begin(); it != props.end(); )
		{
			if (it->name().StartWith(gStr))
			{
				GStringVector vals = it->values();
				prop.setExtendLabelValue( (vals.size() > 0) ? vals.at(0) : "");

				it = props.erase(it);
				break;
			}

			++it;
		}

		GStringVector outs = prop.name().Split(".");
		if (outs.size() == 2)
		{
			prop.name(outs[1]);
			m_properties.push_back(prop);
		}

		iIndex++;
	}

	
}

bool vCard::exportPhoto(const GString& strPathName, DWORD imageType)
{
	vCardProperty propPhoto = getProperty(VC_PHOTO);
	if (!propPhoto.isValid())
	{
		return false;
	}

	GString gStrEncoding;
	GString gstrType;

	vCardParamList params = propPhoto.params();
	M_Foreach_Const(vCardParam, param, vCardParamList, params)
	{
		if (param.group() == vCardParam::Encoding)
			gStrEncoding = param.value();
		else if (param.group() == vCardParam::Type)
			gstrType = param.value();
		else if (param.group() == vCardParam::Undefined)	// for support VC_VER_2_1
			gstrType = param.value();
	}

	gStrEncoding.ToUpper();
	gstrType.ToUpper();

	if ( gStrEncoding != _T("B") && gStrEncoding != _T("BASE64") )
		return false;

	int iFormat = -1;
	if (gstrType == _T("JPEG"))
	{
		iFormat = CXIMAGE_FORMAT_JPG;
	}
	else if (gstrType == _T("GIF"))
	{
		iFormat = CXIMAGE_FORMAT_GIF;
	}
	else if (gstrType == _T("PNG"))
	{
		iFormat = CXIMAGE_FORMAT_PNG;
	}

	if (iFormat == -1)
	{
		Log_Error("vCard::exportPhoto: image format is unknown " << gstrType.ToUtf8());
		return false;
	}

	Cactus::String strData = base64_decode(	propPhoto.values().at(0).ToMbcs() );
	CxImage image((BYTE *)strData.c_str(), strData.size(), iFormat);

	image.Save(strPathName.Str().c_str(), imageType);

	return true;
}

bool vCard::HasMask(int iVal)
{
	return (_iMask & iVal) == iVal;
}

void vCard::AddMask(int iVal)
{
	_iMask |= iVal;
}

void vCard::ClearMask(int iVal, bool bAll/* = false*/)
{
	if (bAll)
	{
		_iMask = 0;
		return;
	}
	
	_iMask &= ~iVal;
}

//REV:20060220T180305Z
void vCard::updateReversion()
{
	CTime time = CTime::GetCurrentTime();

	CString strTime = time.FormatGmt(_T("%Y%m%dT%H%M%SZ"));
	
	vCardProperty prop(	VC_REVISION, (LPCTSTR)strTime);
	
	//addProperty(prop);
	for (vCardPropertyList::iterator it = m_properties.begin(); it != m_properties.end(); ++it)
	{
		vCardProperty current = *it;
		if ( current.name() == prop.name() )
		{
			*it = prop;
			return;
		}
	}

	m_properties.push_back(prop);
}

void vCard::setCatagory( const Cactus::GString& str, bool add )
{
	Cactus::set<Cactus::GString>::type Catas;

	vCardProperty propCat = getProperty(VC_CATEGORIES);
	if (propCat.isValid())
	{
		Cactus::GStringVector& vects = propCat.values();
		for (size_t k = 0; k < vects.size(); ++k)
		{
			Cactus::GStringVector vs = vects[k].Split(L",");
			for (size_t j = 0; j < vs.size(); ++j)
				Catas.insert(vs[j]);
		}
	}

	if (Catas.size() == 0 && !add)
		return;

	if (add)
	{
		Catas.insert(str);
	}
	else
	{
		Catas.erase(str);
	}

	int iCount = 0;
	Cactus::GString strAll;
	for (Cactus::set<Cactus::GString>::type::const_iterator cit = Catas.begin();
		cit != Catas.end(); ++cit)
	{
		if (iCount != 0)
			strAll += L",";
		
		strAll += *cit;
		iCount++;
	}

	vCardParam param = vCardParam(VC_CHARSET_UTF_8, vCardParam::Charset);
	addProperty(vCardProperty(VC_CATEGORIES, strAll, param.toGString()));
}
