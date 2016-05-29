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

#ifndef VCARD_H
#define VCARD_H

#include "vcardproperty.h"

class vCard;
typedef Cactus::list<vCard>::type vCardList;


class vCard : public Cactus::Object
{
protected:
    vCardPropertyList m_properties;

	int		_iImageIndex;		//编辑辅助
	int		_iMask;				//编辑辅助

public:
    vCard();
    vCard(const vCard& vcard);
    vCard(const vCardPropertyList& properties);
    virtual ~vCard();
    
	//如果name和param和已有的属性相同，已有的将被更新
    bool				addProperty(const vCardProperty& prop);
	bool				pushProperty(const vCardProperty& prop);

	//删除所有名为name的属性
    bool				removeProperty(const Cactus::GString& name);
	bool				removeProperty(int iEditorIndex);

	//内部调用addProperty
    bool				addProperties(const vCardPropertyList& properties);
	bool				pushProperties(const vCardPropertyList& properties);

    vCardPropertyList	properties() const;

	//获取名字为name的属性，如果strict为true，将比较params
    vCardProperty		property(const Cactus::GString& name, const vCardParamList& params = vCardParamList(), bool strict = false) const;
    
	//是否包括名字为name的属性，如果strict为true，将比较params
	bool				contains(const Cactus::GString& name, const vCardParamList& params = vCardParamList(), bool strict = false) const;
 
	//是否包括prop属性
	bool				contains(const vCardProperty& prop) const;

	//每条属性都有效才有效，属性有效的条件是，名字、参数(如有)和值都有效
	bool				isValid() const;
    
    int					getPropsCount(const Cactus::GString& propName = "") const;
	bool				hasProperty(const Cactus::GString& propName) const;
	vCardProperty		getProperty(const Cactus::GString& propName) const;
	//获取编辑索引为iIndex的属性
	vCardProperty*		getProperty(int iEditorIndex);
	vCardPropertyList	getProperties(const Cactus::GString& propName) const;
	//获取一组property
	vCardPropertyList	getProperties(const Cactus::GStringVector& propNames) const;

    Cactus::GString		toGString(EvCardVersion version = VC_VER_3_0, EvCardUsage usage = VC_Usage_Android_Apple) const;
    bool				saveToFile(const Cactus::String& filename) const;
    
    static vCardList	fromByteArray(const Cactus::String& data, Cactus::GString::EGStringEncoding charset);
    static vCardList	fromFile(const Cactus::String& filename, EvCardUsage origin = VC_Usage_Android_Apple);

	int					GetImageIndex() const { return _iImageIndex; }
	void				SetImageIndex(int i) { _iImageIndex = i; }

	bool				HasMask(int iVal);
	void				AddMask(int iVal);
	void				ClearMask(int iVal, bool bAll = false);

	Cactus::GString		getValid_FN_N_Name() const;
	bool				Merge(const vCard& card);
	bool				normalize_N(int iType);
	bool				normalize_TEL(int iFlag);	//0x1 remove minus, 0x10 remove space, 0x100 remove +86

	void				updateReversion();

	void				normalize();

	bool				exportPhoto(const Cactus::GString& strPathName, DWORD imageType);

	bool				operator==(const vCard& right) const;

	void				setCatagory(const Cactus::GString& str, bool add);
};


#endif // VCARD_H
