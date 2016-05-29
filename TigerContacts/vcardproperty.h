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

#ifndef VCARDPROPERTY_H
#define VCARDPROPERTY_H

#include "vcardparam.h"

class vCardProperty;
typedef Cactus::list<vCardProperty>::type vCardPropertyList;

typedef Cactus::map<Cactus::GString, Cactus::GString>::type		GStringMapType;

class vCardProperty : public Cactus::Object
{
public:
    enum EGenericFields
    {
        eDefaultValue = 0
    };

    enum EAddressFields
    {
        PostOfficeBox = 0,
        ExtendedAddress,
        Street,
        Locality,           // e.g. City.
        Region,             // e.g. State or province.
        PostalCode,
        Country
    };

    enum ENameFields
    {
        Lastname = 0,
        Firstname,
        Additional,
        Prefix,
        Suffix
    };

    enum EGeographicPositionFields
    {
        Latitude = 0,
        Longitude
    };

protected:

	int				_iEditorIndex;		//编辑辅助
	static int		s_iEditorIndex;		//编辑辅助

	//TEL;TYPE=WORK;TYPE=FAX:1-304-161-7827
    Cactus::GString			m_name;		//TEL
    Cactus::GStringVector	m_values;	//1-304-161-7827
    vCardParamList			m_params;	//TYPE=WORK;TYPE=FAX



	//ITEM1.TEL:8888888							//自定义电话
	//ITEM1.X-ABLABEL:自定义

	//ITEM8.X-ABRELATEDNAMES:李四
	//ITEM8.X-ABLABEL:_$!<Spouse>!$_	//配偶
	bool					_bIsExtProp;			//是否是扩展属性
	Cactus::GString			_strExtLabelValue;		//扩展属性的标签值: 自定义, _$!<Spouse>!$_

	void				_PatchProperty();

public:

    vCardProperty();
    vCardProperty(const Cactus::GString& name, const Cactus::GString& value, const vCardParamList& params = vCardParamList());
    vCardProperty(const Cactus::GString& name, const Cactus::GStringVector& values, const vCardParamList& params = vCardParamList());
    vCardProperty(const Cactus::GString& name, const Cactus::GString& value, const Cactus::GString& params);
    vCardProperty(const Cactus::GString& name, const Cactus::GStringVector& values, const Cactus::GString& params);
    virtual ~vCardProperty();

    Cactus::GString name() const;
	void name(const Cactus::GString& n) { m_name = n; }

	Cactus::GString value(const Cactus::GString& separated = VC_SEPARATOR_TOKEN) const;
    Cactus::GStringVector values() const;
	Cactus::GStringVector& values();

	vCardParamList params() const;
	void params(const vCardParamList& params);
	bool isValid() const;
	bool isValueValid() const;

    bool operator== (const vCardProperty& param) const;
    bool operator!= (const vCardProperty& param) const;

    Cactus::GString toGString(int& iIndex, EvCardVersion version = VC_VER_3_0, EvCardUsage usage = VC_Usage_Android_Apple) const;

	static vCardPropertyList fromGString(const Cactus::GString& gStr);

    static vCardProperty createAddress(const Cactus::GString& street, const Cactus::GString& locality, const Cactus::GString& region, const Cactus::GString& postal_code, const Cactus::GString& country, const Cactus::GString& post_office_box = "", const Cactus::GString& ext_address = "", const vCardParamList& params = vCardParamList());
    //static vCardProperty createBirthday(const QDate& birthday, const vCardParamList& params = vCardParamList());
    //static vCardProperty createBirthday(const QDateTime& birthday, const vCardParamList& params = vCardParamList());
    static vCardProperty createGeographicPosition(float latitude, float longitude, const vCardParamList& params = vCardParamList());
    static vCardProperty createName(const Cactus::GString& firstname, const Cactus::GString& lastname, const Cactus::GString& additional = "", const Cactus::GString& prefix = "", const Cactus::GString& suffix = "", const vCardParamList& params = vCardParamList());
    static vCardProperty createOrganization(const Cactus::GString& name, const Cactus::GStringVector& levels = Cactus::GStringVector(), const vCardParamList& params = vCardParamList());

	void				setEditorIndex(int i) { _iEditorIndex = i; }
	int					getEditorIndex() const { return _iEditorIndex; }

	bool				isExtendProp() const { return _bIsExtProp; }
	void				setExtendProp(bool b) { _bIsExtProp = b; }

	Cactus::GString		getExtendLabelValue() const { return _strExtLabelValue; }
	void				setExtendLabelValue(const Cactus::GString& val) { _strExtLabelValue = val; }

	static bool			isUniqueProperty(const Cactus::GString& name);

};

#endif // VCARDPROPERTY_H
