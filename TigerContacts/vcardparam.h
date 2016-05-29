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

#ifndef VCARDPARAM_H
#define VCARDPARAM_H

#include <CommonUtil/CommonUtil.h>

#define VC_SEPARATOR_TOKEN		';'
#define VC_END_LINE_TOKEN		"\r\n"
#define VC_END_LINE_TOKEN_LINUX	"\n"
#define VC_BEGIN_TOKEN			"BEGIN:VCARD"
#define VC_END_TOKEN			"END:VCARD"

#define VC_CHARSET_UTF_8		"UTF-8"
#define VC_ENCODING_QP			"QUOTED-PRINTABLE"






#define VC_ADDRESS              "ADR"
#define VC_AGENT                "AGENT"
#define VC_BIRTHDAY             "BDAY"

/*
目的：vcard应用的分类信息 
例子：CATEGORIES:TRAVEL AGENT 
CATEGORIES:INTERNET,IETF,INDUSTRY,INFORMATION TECHNOLOGY
*/
#define VC_CATEGORIES           "CATEGORIES"

/*
目的：指定访问Vcard对象的访问分级。 
例子：  CLASS:PUBLIC 
CLASS:PRIVATE 
CLASS:CONFIDENTIAL 
说明：安全分级需要参考目录服务的访问分级
*/
#define VC_CLASS                "CLASS"

#define VC_DELIVERY_LABEL       "LABEL"
#define VC_EMAIL                "EMAIL"
#define VC_FORMATTED_NAME       "FN"
#define VC_GEOGRAPHIC_POSITION  "GEO"
#define VC_KEY                  "KEY"
#define VC_LOGO                 "LOGO"
#define VC_MAILER               "MAILER"
#define VC_NAME                 "N"
#define VC_NICKNAME             "NICKNAME"
#define VC_NOTE                 "NOTE"
#define VC_ORGANIZATION         "ORG"
#define VC_PHOTO                "PHOTO"
#define VC_PRODUCT_IDENTIFIER   "PRODID"

/*
目的：指定当前Vcard的修改信息 
例子：REV:1995-10-31T22:27:10Z 
REV:1997-11-15 
REV:20060220T180305Z
*/
#define VC_REVISION             "REV"
#define VC_ROLE                 "ROLE"
#define VC_SORT_STRING          "SORT-STRING"
#define VC_SOUND                "SOUND"
#define VC_TELEPHONE            "TEL"
#define VC_TIME_ZONE            "TZ"
#define VC_TITLE                "TITLE"
#define VC_URL                  "URL"
#define VC_VERSION              "VERSION"


enum EvCardVersion
{
	VC_VER_2_1,
	VC_VER_3_0
};

enum EvCardUsage
{
	VC_Usage_Android_Apple,
	VC_Usage_Nokia_S60,
	VC_Usage_Outlook,
	VC_Usage_Unknown
};


class vCardParam;
typedef Cactus::list<vCardParam>::type		vCardParamList;

class vCardParam : public Cactus::Object
{
public:
    enum EvCardParamGroup
    {
        Type,
        Encoding,
        Charset,
        Undefined
    };

protected:

	//TEL;TYPE=CELL:1-391-601-9693
	//ADR;TYPE=HOME;CHARSET=UTF-8:;;鍥介『涓滆矾900寮?9鍙?02瀹ゆ眹鍏冨潑;;;;
	
    EvCardParamGroup	m_group;
    Cactus::GString		m_value;

public:
    vCardParam();
    vCardParam(const Cactus::GString& value, EvCardParamGroup group = vCardParam::Undefined);
    virtual ~vCardParam();

    EvCardParamGroup group() const;
    Cactus::GString value() const;

	void	value(const Cactus::GString& val){ m_value = val; }

    bool isValid() const;

    bool operator== (const vCardParam& param) const;
    bool operator!= (const vCardParam& param) const;

    Cactus::GString			toGString(EvCardVersion version = VC_VER_3_0, EvCardUsage usage = VC_Usage_Android_Apple) const;
	static Cactus::GString	toGString(const Cactus::GString& name, const vCardParamList& params, EvCardVersion version = VC_VER_3_0, EvCardUsage usage = VC_Usage_Android_Apple);

    static vCardParamList	fromGString(const Cactus::GString& gStr);
};

#endif // VCARDPARAM_H
