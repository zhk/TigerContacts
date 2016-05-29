#include "StdAfx.h"
#include "vCardEditor.h"
#include "resource.h"

using namespace Cactus;

Controls* Cactus::Singleton<Controls>::s_pSingleton = 0;


//------------------------------------------------------------------------------------
CStatic* Controls::GetStatic(int iType)
{
	if(_staticsLib.size() != 0)
	{
		CStatic* pStatic = _staticsLib.front();
		_staticsLib.pop_front();
		pStatic->SetFont(iType == 0 ? &_fontDefault : &_fontLarge);

		_statics.push_back(pStatic);
		return pStatic;
	}

	CStatic* pStatic = new CStatic;

	CRect rc(0, 0, 1, 1);
	pStatic->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_LEFT, rc, _pWnd);
	pStatic->SetFont(iType == 0 ? &_fontDefault : &_fontLarge);

	_statics.push_back(pStatic);

	return pStatic;
}

CStatic* Controls::GetStaticBmp()
{
	if(_staticsBmpLib.size() != 0)
	{
		CStatic* pStatic = _staticsBmpLib.front();
		_staticsBmpLib.pop_front();

		_staticsBmp.push_back(pStatic);
		return pStatic;
	}

	CStatic* pStatic = new CStatic;

	CRect rc(0, 0, 1, 1);
	pStatic->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_ICON | SS_CENTERIMAGE, rc, _pWnd);

	_staticsBmp.push_back(pStatic);

	return pStatic;
}

CButton* Controls::GetButton()
{
	if(_buttonsLib.size() != 0)
	{
		CButton* pBtn = _buttonsLib.front();
		_buttonsLib.pop_front();

		_buttons.push_back(pBtn);
		return pBtn;
	}

	if (_iCurCmdID_Button == M_Cmd_Button_END)
	{
		Log_Error("Controls::GetButton, out of range!");
		return 0;
	}

	CButton* pBtn = new CButton;

	CRect rc(0, 0, 1, 1);
	pBtn->Create( _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON, rc, _pWnd, _iCurCmdID_Button++ );
	pBtn->SetFont(&_fontDefault);

	_buttons.push_back(pBtn);

	return pBtn;
}

CButton* Controls::GetButtonBmp( int iType )
{
	if(_buttonsBmpLib.size() != 0)
	{
		CButton* pBtn = _buttonsBmpLib.front();
		_buttonsBmpLib.pop_front();
		pBtn->SetIcon(iType == 0 ? _iconAdd : _iconDel);

		_buttonsBmp.push_back(pBtn);
		return pBtn;
	}

	if (_iCurCmdID_Button == M_Cmd_Button_END)
	{
		Log_Error("Controls::GetButtonBmp, out of range!");
		return 0;
	}

	CButton* pBtn = new CButton;

	CRect rc(0, 0, 1, 1);
	pBtn->Create( _T(""), WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_PUSHBUTTON | BS_ICON, rc, _pWnd, _iCurCmdID_Button++ );
	pBtn->SetIcon(iType == 0 ? _iconAdd : _iconDel);

	_buttonsBmp.push_back(pBtn);

	return pBtn;
}

CComboBox* Controls::GetCombo()
{
	if(_comboesLib.size() != 0)
	{
		CComboBox* pCombo = _comboesLib.front();
		_comboesLib.pop_front();

		_comboes.push_back(pCombo);
		return pCombo;
	}

	if (_iCurCmdID_Combo == M_Cmd_Combo_END)
	{
		Log_Error("Controls::GetCombo, out of range!");
		return 0;
	}

	CComboBox* pCombo = new CComboBox;
	CRect rc(0, 0, 1, 200);
	pCombo->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | CBS_AUTOHSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS | CBS_SORT, rc, _pWnd, _iCurCmdID_Combo++);
	pCombo->SetFont(&_fontDefault);

	_comboes.push_back(pCombo);

	return pCombo;
}

CEdit* Controls::GetEdit()
{
	if(_editsLib.size() != 0)
	{
		CEdit* pEdit = _editsLib.front();
		_editsLib.pop_front();

		_edits.push_back(pEdit);
		return pEdit;
	}

	if (_iCurCmdID_Edit == M_Cmd_Edit_END)
	{
		Log_Error("Controls::GetEdit, out of range!");
		return 0;
	}

	CEdit* pEdit = new CEdit;
	CRect rc(0, 0, 1, 1);
	pEdit->Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, rc, _pWnd, _iCurCmdID_Edit++);
	pEdit->SetFont(&_fontDefault);

	_edits.push_back(pEdit);

	return pEdit;
}

void Controls::ReturnAllUsedControls()
{
	M_Foreach(CStatic*, pWnd, CStatic_ListType, _statics)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_staticsLib.insert(_staticsLib.end(), _statics.begin(), _statics.end());
	_statics.clear();


	M_Foreach(CStatic*, pWnd, CStatic_ListType, _staticsBmp)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_staticsBmpLib.insert(_staticsBmpLib.end(), _staticsBmp.begin(), _staticsBmp.end());
	_staticsBmp.clear();


	M_Foreach(CEdit*, pWnd, CEdit_ListType, _edits)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_editsLib.insert(_editsLib.end(), _edits.begin(), _edits.end());
	_edits.clear();


	M_Foreach(CButton*, pWnd, CButton_ListType, _buttons)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_buttonsLib.insert(_buttonsLib.end(), _buttons.begin(), _buttons.end());
	_buttons.clear();


	M_Foreach(CButton*, pWnd, CButton_ListType, _buttonsBmp)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_buttonsBmpLib.insert(_buttonsBmpLib.end(), _buttonsBmp.begin(), _buttonsBmp.end());
	_buttonsBmp.clear();


	M_Foreach(CComboBox*, pWnd, CComboBox_ListType, _comboes)
	{
		pWnd->ShowWindow(SW_HIDE);
	}
	_comboesLib.insert(_comboesLib.end(), _comboes.begin(), _comboes.end());
	_comboes.clear();


	for (CWndUserData_MapType::iterator it = _UserData.begin(); it != _UserData.end(); ++it)
	{
		Mem_Delete it->second;
	}
	_UserData.clear();

	ResetLabelUsage();

	_CmdButtons.clear();
}

void Controls::LoadPropInfoGroup(const Cactus::GString& strSectionName)
{
	GroupPropInfoMapType group;

	FileConfig::SectionListType sectionList = _FileConfig.GetSection(strSectionName.ToMbcs());
	M_Foreach_Const(FileConfig::SPair, p, FileConfig::SectionListType, sectionList)
	{
		GString gStrFirst(p.first, GString::eSE_Utf8);
		if ( gStrFirst.StartWith("#") )
			continue;

		GString gStrSecond(p.second, GString::eSE_Utf8);

		GStringVector outs = gStrSecond.Split(",");
		if(outs.size() != 4)
			continue;

		SPropInfo propInfo;
		propInfo._strPropGroup	= strSectionName;
		propInfo._strComboKey	= gStrFirst;
		propInfo._strComboCap	= outs[0];
		propInfo._bExtend		= outs[1] == "true";
		propInfo._bReplaceName	= outs[2] == "true";
		propInfo._strParam		= outs[3];
		propInfo._bUsed			= false;

		group[gStrFirst] = propInfo;
	}

	_PropInfoGroup[strSectionName] = group;
}

Cactus::GStringVector Controls::GetAllLabels(const Cactus::GString& strSectionName)
{
	GStringVector outs;
	if(_PropInfoGroup.find(strSectionName) == _PropInfoGroup.end())
		return outs;

	GroupPropInfoMapType group = _PropInfoGroup[strSectionName];
	for (GroupPropInfoMapType::iterator it = group.begin(); it != group.end(); ++it)
	{
		outs.push_back(it->second._strComboCap);
	}

	return outs;
}

SPropInfo* Controls::GetAvaliablePropInfo(const Cactus::GString& strSectionName)
{
	if(_PropInfoGroup.find(strSectionName) == _PropInfoGroup.end())
		return 0;

	GroupPropInfoMapType& group = _PropInfoGroup[strSectionName];
	for (GroupPropInfoMapType::iterator it = group.begin(); it != group.end(); ++it)
	{
		if (!it->second._bUsed)
		{
			it->second._bUsed = true;
			return &(it->second);
		}
	}

	return &group["CUSTOM"];
}

SPropInfo* Controls::GetPropInfoFromCaption(const Cactus::GString& strSectionName, const Cactus::GString& strCap)
{
	if(_PropInfoGroup.find(strSectionName) == _PropInfoGroup.end())
		return 0;

	GroupPropInfoMapType& group = _PropInfoGroup[strSectionName];
	for (GroupPropInfoMapType::iterator it = group.begin(); it != group.end(); ++it)
	{
		if (it->second._strComboCap == strCap)
		{
			return &(it->second);
		}
	}

	return &group["CUSTOM"];
}


void Controls::ResetLabelUsage()
{
	for (NameGroupPropInfoMapType::iterator itGroup = _PropInfoGroup.begin(); itGroup != _PropInfoGroup.end(); ++itGroup)
	{
		GroupPropInfoMapType& group = itGroup->second;
		for (GroupPropInfoMapType::iterator it = group.begin(); it != group.end(); ++it)
		{
			it->second._bUsed = false;
		}
	}
}

Cactus::GString Controls::GetLabel(const Cactus::GString& strSectionName, const vCardProperty& prop)
{
	NameGroupPropInfoMapType::iterator itGroup = _PropInfoGroup.find(strSectionName);
	if(itGroup == _PropInfoGroup.end())
	{
		return "";
	}

	GroupPropInfoMapType& group = itGroup->second;

	if (!prop.isExtendProp())					//普通属性
	{
		vCardParamList params = prop.params();
		if (params.size() > 0)	//有参数
		{
			GString gStrKey;
			int iIndex = 0;
			M_Foreach_Const(vCardParam, param, vCardParamList, params)
			{
				if (param.group() == vCardParam::Type)
				{
					if (iIndex++ != 0)
						gStrKey += ";";

					gStrKey += param.value();
				}
			}

			if (iIndex != 0)
			{
				group[gStrKey]._bUsed = true;
				return group[gStrKey]._strComboCap;
			}
			else
			{
				GroupPropInfoMapType::iterator it = group.find(prop.name());	//for IM
				if (it != group.end())
				{
					it->second._bUsed = true;
					return it->second._strComboCap;
				}

				return "";
			}
		}
		else						//无参数
		{
			GroupPropInfoMapType::iterator it = group.find(prop.name());	//for IM
			if (it != group.end())
			{
				it->second._bUsed = true;
				return it->second._strComboCap;
			}

			return "";
		}
	}
	else
	{
		GString strExtValue = prop.getExtendLabelValue();
		if (strExtValue.Str().length() == 0)
		{
			return group["CUSTOM"]._strComboCap;
		}
		
		GroupPropInfoMapType::iterator it = group.find(strExtValue);		//_$!<Spouse>!$_
		if (it == group.end())
		{
			return strExtValue;
		}
		else
		{
			it->second._bUsed = true;
			return it->second._strComboCap;
		}
	}
}


Controls::Controls(CScrollView* pWnd)
: _pWnd(pWnd)
{
	_fontDefault.CreatePointFont(110, _T("新宋体"));
	_fontLarge.CreatePointFont(135, _T("新宋体"));

	_iconAdd = AfxGetApp()->LoadIcon(IDI_ICON_ADD);
	_iconDel = AfxGetApp()->LoadIcon(IDI_ICON_DELETE);

	_iCurCmdID_Button	= M_Cmd_Button_Start;
	_iCurCmdID_Edit		= M_Cmd_Edit_Start;
	_iCurCmdID_Combo	= M_Cmd_Combo_Start;


	_FileConfig.SetSource("config.ini");
	{
		String strNames = "TEL,EMAIL,ADR,IM,URL,X-ABDATE,X-ABRELATEDNAMES,NOTE";
		_FileConfig.GetString("NAMES_SUPPORT", "Names", strNames);
		GString gStrNames(strNames);
		_supportPropNames = gStrNames.Split(",");
		M_Foreach_Const(GString, gStr, GStringVector, _supportPropNames)
			Controls::getSingleton().LoadPropInfoGroup(gStr);
	}

	FileConfig::SectionListType sectionList = _FileConfig.GetSection("LABEL_GROUP");
	M_Foreach_Const(FileConfig::SPair, p, FileConfig::SectionListType, sectionList)
	{
		GString gStrFirst(p.first, GString::eSE_Utf8);
		if ( gStrFirst.StartWith("#") )
			continue;

		GString gStrSecond(p.second);

		GStringVector outs = gStrSecond.Split(",");
		SLabelGroup lg;
		lg._strLabelSection = gStrFirst;
		M_Foreach_Const(GString, gStr, GStringVector, outs)
			lg._strPropNames.push_back(gStr);

		_PropLabelsGroup[lg._strLabelSection] = lg;
	}

}

Controls::~Controls()
{
	M_Foreach(CStatic*, pWnd, CStatic_ListType, _statics)
		delete pWnd;

	M_Foreach(CStatic*, pWnd, CStatic_ListType, _staticsBmp)
		delete pWnd;

	M_Foreach(CEdit*, pWnd, CEdit_ListType, _edits)
		delete pWnd;

	M_Foreach(CButton*, pWnd, CButton_ListType, _buttons)
		delete pWnd;

	M_Foreach(CButton*, pWnd, CButton_ListType, _buttonsBmp)
		delete pWnd;

	M_Foreach(CComboBox*, pWnd, CComboBox_ListType, _comboes)
	{
		pWnd->ResetContent();
		delete pWnd;
	}



	M_Foreach(CStatic*, pWnd, CStatic_ListType, _staticsLib)
		delete pWnd;

	M_Foreach(CStatic*, pWnd, CStatic_ListType, _staticsBmpLib)
		delete pWnd;

	M_Foreach(CEdit*, pWnd, CEdit_ListType, _editsLib)
		delete pWnd;

	M_Foreach(CButton*, pWnd, CButton_ListType, _buttonsLib)
		delete pWnd;

	M_Foreach(CButton*, pWnd, CButton_ListType, _buttonsBmpLib)
		delete pWnd;

	M_Foreach(CComboBox*, pWnd, CComboBox_ListType, _comboesLib)
	{
		pWnd->ResetContent();
		delete pWnd;
	}


	for (CWndUserData_MapType::iterator it = _UserData.begin(); it != _UserData.end(); ++it)
	{
		Mem_Delete it->second;
	}
}
//------------------------------------------------------------------------------------
SPropLine::SPropLine(int iEditorIndex)
: _iEditorIndex(iEditorIndex)
{
}

SPropLine::~SPropLine()
{

}


//------------------------------------------------------------------------------------
PropGroup::PropGroup(vCard* pCard
					 , const Cactus::GString& strLabelSection
					 , int iIndexH
					 , bool bHasTitle
					 , bool bCombo
					 , bool bPhotoRight
					 )
: _strLabelSection(strLabelSection)
, _iNextLinePosH(iIndexH)
, _pLaber(0)
, _pButtonBmp(0)
, _bHasTitle(bHasTitle)
, _bPhotoRight(bPhotoRight)
, _pCard(pCard)
, _bCombo(bCombo)
{
	String strCaption = strLabelSection.ToMbcs();
	Controls::getSingleton()._FileConfig.GetString("NAMES", strCaption.c_str(), strCaption);
	GString gStrCaption(strCaption, GString::eSE_Utf8);
	_strCaption = gStrCaption.Str().c_str();

	//
	if (bHasTitle)
	{
		// Create Label
		_pLaber = Controls::getSingleton().GetStatic(1);
		if (!_pLaber)
			return;
		_pLaber->SetWindowText(_strCaption);
		_pLaber->MoveWindow( CRect(CPoint(130, _iNextLinePosH), CSize(120, M_LineH)), FALSE );
		_pLaber->ShowWindow(SW_SHOW);

		// Create ButtonBmp
		// Remove BmpButton
		_pButtonBmp = Controls::getSingleton().GetButtonBmp(0);
		if (!_pButtonBmp)
			return;
		_pButtonBmp->MoveWindow( CRect(CPoint(620, _iNextLinePosH - 2), CSize(32, 32)), FALSE );
		_pButtonBmp->ShowWindow(SW_SHOW);

		UserData* pUserData = Mem_New UserData(_strLabelSection, -1, (int)this, "BtnAdd");
		Controls::getSingleton()._UserData[_pButtonBmp] = pUserData;

		Controls::getSingleton()._CmdButtons[_strLabelSection] = _pButtonBmp->GetDlgCtrlID();

		_iNextLinePosH += M_LineH + M_LineH_M;
	}

	vCardPropertyList props = pCard->getProperties(_strLabelSection);
	M_Foreach_Const(vCardProperty, prop, vCardPropertyList, props)
	{
		AddPropLine(prop, 0, false);
	}
}

PropGroup::PropGroup(vCard* pCard
					 , const SLabelGroup& labelGroup
					 , int iIndexH
					 , bool bHasTitle
					 , bool bCombo
					 , bool bPhotoRight
					 )
: _strLabelSection(labelGroup._strLabelSection)
, _iNextLinePosH(iIndexH)
, _pLaber(0)
, _pButtonBmp(0)
, _bHasTitle(bHasTitle)
, _bPhotoRight(bPhotoRight)
, _pCard(pCard)
, _bCombo(bCombo)
{
	String strCaption = labelGroup._strLabelSection.ToMbcs();
	Controls::getSingleton()._FileConfig.GetString("NAMES", strCaption.c_str(), strCaption);
	GString gStrCaption(strCaption, GString::eSE_Utf8);
	_strCaption = gStrCaption.Str().c_str();

	//
	if (bHasTitle)
	{
		// Create Label
		_pLaber = Controls::getSingleton().GetStatic(1);
		if (!_pLaber)
			return;
		_pLaber->SetWindowText(_strCaption);
		_pLaber->MoveWindow( CRect(CPoint(130, _iNextLinePosH), CSize(120, M_LineH)), FALSE );
		_pLaber->ShowWindow(SW_SHOW);

		// Create ButtonBmp
		// Add BmpButton
		_pButtonBmp = Controls::getSingleton().GetButtonBmp(0);
		if (!_pButtonBmp)
			return;
		_pButtonBmp->MoveWindow( CRect(CPoint(620, _iNextLinePosH - 2), CSize(32, 32)), FALSE );
		_pButtonBmp->ShowWindow(SW_SHOW);

		UserData* pUserData = Mem_New UserData(_strLabelSection, -1, (int)this, "BtnAdd");
		Controls::getSingleton()._UserData[_pButtonBmp] = pUserData;

		Controls::getSingleton()._CmdButtons[_strLabelSection] = _pButtonBmp->GetDlgCtrlID();

		_iNextLinePosH += M_LineH + M_LineH_M;
	}

	vCardPropertyList props = pCard->getProperties(labelGroup._strPropNames);
	M_Foreach_Const(vCardProperty, prop, vCardPropertyList, props)
	{
		AddPropLine(prop, 0, false);
	}
}


PropGroup::~PropGroup()
{
	M_Foreach(SPropLine*, pLine, PropLines_ListType, _lines)
	{
		Mem_Delete pLine;
	}
	_lines.clear();
}

int PropGroup::AddPropLine( const vCardProperty& prop, int iScrollPos, bool bEnsureVisible )
{
	int iStartLineH	= _iNextLinePosH - iScrollPos;
	int iLeftX		= _bPhotoRight ? 250 : 160;
	int iW			= _bPhotoRight ? 80 : 360;
	int iCaptionW	= _bPhotoRight ? 100 : 120;
	int iIndex		= prop.getEditorIndex();

	SPropLine* pLine = Mem_New SPropLine(iIndex);

	CWnd* pWnd;
	if (_bCombo)
	{
		pWnd = Controls::getSingleton().GetCombo();
		if (!pWnd)
			return 0;
		pWnd->MoveWindow( CRect(CPoint(iLeftX, iStartLineH), CSize(iCaptionW, M_LineH)), FALSE );

		((CComboBox*)pWnd)->ResetContent();
		GStringVector labels = Controls::getSingleton().GetAllLabels( _strLabelSection );
		M_Foreach_Const(GString, gStr, GStringVector, labels)
		{
			((CComboBox*)pWnd)->AddString(gStr.Str().c_str());
		}

		GString strLabel = Controls::getSingleton().GetLabel(_strLabelSection, prop);
		if( ((CComboBox*)pWnd)->SelectString(0, strLabel.Str().c_str()) == LB_ERR )
		{
			pWnd->SetWindowText(strLabel.Str().c_str());
		}
		pWnd->ShowWindow(SW_SHOW);

		UserData* pUserData = Mem_New UserData(_strLabelSection, iIndex, 0, "Combo");
		Controls::getSingleton()._UserData[pWnd] = pUserData;

		pLine->_controlsInLine.push_back(pWnd);
	}
	else
	{
		pWnd = Controls::getSingleton().GetStatic(0);
		if (!pWnd)
			return 0;
		pWnd->SetWindowText(_strCaption);
		pWnd->MoveWindow( CRect(CPoint(iLeftX, iStartLineH), CSize(iCaptionW, M_LineH)), FALSE );
		pWnd->ShowWindow(SW_SHOW);

		pLine->_controlsInLine.push_back(pWnd);
	}
	iLeftX += iCaptionW + 10;

	CWnd* pBtnRemove = 0;
	
	// Remove BmpButton
	if (_bHasTitle)
	{
		pWnd = Controls::getSingleton().GetButtonBmp(1);
		if (!pWnd)
			return 0;
		pWnd->MoveWindow( CRect(CPoint(iLeftX + 10 + (iW + 5), iStartLineH - 5), CSize(32, 32)), FALSE );
		pWnd->ShowWindow(SW_SHOW);

		UserData* pUserData = Mem_New UserData(_strLabelSection, iIndex, (int)this, "BtnRemove");
		Controls::getSingleton()._UserData[pWnd] = pUserData;

		pLine->_controlsInLine.push_back(pWnd);

		pBtnRemove = pWnd;
	}

	// Property Edit
	int iCount = 0;
	Cactus::GStringVector values = prop.values();
	int iValuesCount = values.size();
	bool bIsAddress = (prop.name() == VC_ADDRESS);
	M_Foreach_Const(GString, gStr, GStringVector, values)
	{
		pWnd = Controls::getSingleton().GetEdit();
		if (!pWnd)
			return 0;
		if (bIsAddress)
		{
			pWnd->MoveWindow( CRect(CPoint(iLeftX + 64 + 8, iStartLineH), CSize(iW - 64 - 8, M_LineH)), FALSE );
		}
		else
		{
			pWnd->MoveWindow( CRect(CPoint(iLeftX, iStartLineH), CSize((iValuesCount == 1 && _bPhotoRight) ? iW * 2 : iW, M_LineH)), FALSE );
		}
		pWnd->SetWindowText(gStr.Str().c_str());
		pWnd->ShowWindow(SW_SHOW);

		UserData* pUserData = Mem_New UserData(_strLabelSection, iIndex, iCount++, "Edit");
		Controls::getSingleton()._UserData[pWnd] = pUserData;

		pLine->_controlsInLine.push_back(pWnd);

		if (prop.name() == VC_ADDRESS)
			iStartLineH += M_LineH + 2;
		else
			iLeftX += (iW + 5);
	}

	if (bIsAddress)
	{
		CString strAddressCaps[] = {
		_("邮政信箱"),
		_("邻居"),
		_("街道"),
		_("市县"),
		_("州省"),
		_("邮编"),
		_("国家")
		};

		for (int iCount = 0; iCount < iValuesCount; ++iCount)
		{
			pWnd = Controls::getSingleton().GetStatic(0);
			if (!pWnd)
				return 0;
			pWnd->MoveWindow( CRect(CPoint(iLeftX + 4, iStartLineH - (iValuesCount - iCount) * (M_LineH + 2)), CSize(64, M_LineH)), FALSE );
			pWnd->SetWindowText(strAddressCaps[iCount]);
			pWnd->ShowWindow(SW_SHOW);

			pLine->_controlsInLine.push_back(pWnd);
		}

		pLine->_iLineHeight = iValuesCount * (M_LineH + 2) + M_LineH_M;
	}
	else if (prop.name() == VC_NAME)
	{
		pWnd = Controls::getSingleton().GetStatic(0);
		if (!pWnd)
			return 0;
		pWnd->MoveWindow( CRect(CPoint(iLeftX, iStartLineH), CSize(iW * 4, M_LineH)), FALSE );
		pWnd->SetWindowText(_("1姓|2名|3中间名|4前缀|5后缀"));
		pWnd->ShowWindow(SW_SHOW);

		pLine->_controlsInLine.push_back(pWnd);

		pLine->_iLineHeight = M_LineH + M_LineH_M;
	}
	else
		pLine->_iLineHeight = M_LineH + M_LineH_M;

	_lines.push_back(pLine);

	_iNextLinePosH += pLine->_iLineHeight;


	if (bEnsureVisible && pBtnRemove)
	{
		CRect rc;
		pBtnRemove->GetWindowRect(rc);
		Controls::getSingleton()._pWnd->ScreenToClient(rc);

		CRect rcClient;
		Controls::getSingleton()._pWnd->GetClientRect(rcClient);

		int iScrollPos = Controls::getSingleton()._pWnd->GetScrollPos(SB_VERT);

		Log_Debug("PropGroup::AddPropLine, top: " << rc.top << ", bottom: " << rc.bottom << ", Client bottom: " << rcClient.bottom);

		if (rc.top > rcClient.bottom - (M_LineH + M_LineH_M))
		{
			Controls::getSingleton()._pWnd->ScrollToPosition( CPoint( 0, iScrollPos + rc.top - rcClient.Height() + (M_LineH + M_LineH_M)) );
		}
		else if (rc.bottom < rcClient.top)
		{
			Controls::getSingleton()._pWnd->ScrollToPosition( CPoint(0, iScrollPos + rc.top - rcClient.Height() / 2) );
		}
	}


	return pLine->_iLineHeight;
}

SPropLine* PropGroup::GetLine(int iIndex)
{
	M_Foreach(SPropLine*, pLine, PropLines_ListType, _lines)
	{
		if(pLine->_iEditorIndex == iIndex)
			return pLine;
	}

	return 0;
}

void PropGroup::MoveDownControls(CWnd* pWnd, int iHeight)
{
	CRect rc;

	if (_pLaber)
	{
		_pLaber->GetWindowRect(rc);
		pWnd->ScreenToClient(rc);
		rc.OffsetRect(0, iHeight);
		_pLaber->MoveWindow(rc);
	}

	if (_pButtonBmp)
	{
		_pButtonBmp->GetWindowRect(rc);
		pWnd->ScreenToClient(rc);
		rc.OffsetRect(0, iHeight);
		_pButtonBmp->MoveWindow(rc);
	}

	M_Foreach(SPropLine*, pLine, PropLines_ListType, _lines)
	{
		M_Foreach(CWnd*, pTmp, CWnd_ListType, pLine->_controlsInLine)
		{
			pTmp->GetWindowRect(rc);
			pWnd->ScreenToClient(rc);
			rc.OffsetRect(0, iHeight);
			pTmp->MoveWindow(rc);
		}
	}

	_iNextLinePosH += iHeight;
}

int PropGroup::RemoveOneLine(CWnd* pWnd, int iIndex)
{
	bool bRemoved = false;
	int iOffsetH = 0;

	for(PropLines_ListType::iterator itLine = _lines.begin(); itLine != _lines.end(); )
	{
		SPropLine* pLine = *itLine;
		if (pLine->_iEditorIndex == iIndex)
		{
			iOffsetH = pLine->_iLineHeight;

			M_Foreach(CWnd*, pTmp, CWnd_ListType, pLine->_controlsInLine)
			{
				pTmp->ShowWindow(SW_HIDE);

				if ( strcmp( pTmp->GetRuntimeClass()->m_lpszClassName, "CStatic" ) == 0 )
				{
					Controls::getSingleton()._statics.remove((CStatic*)pTmp);
					Controls::getSingleton()._staticsLib.push_back((CStatic*)pTmp);
				}
				else if( strcmp( pTmp->GetRuntimeClass()->m_lpszClassName, "CButton" ) == 0 )
				{
					Controls::getSingleton()._buttonsBmp.remove((CButton*)pTmp);
					Controls::getSingleton()._buttonsBmpLib.push_back((CButton*)pTmp);
				}
				else if( strcmp( pTmp->GetRuntimeClass()->m_lpszClassName, "CEdit" ) == 0 )
				{
					Controls::getSingleton()._edits.remove((CEdit*)pTmp);
					Controls::getSingleton()._editsLib.push_back((CEdit*)pTmp);
				}
				else if( strcmp( pTmp->GetRuntimeClass()->m_lpszClassName, "CComboBox" ) == 0 )
				{
					Controls::getSingleton()._comboes.remove((CComboBox*)pTmp);
					Controls::getSingleton()._comboesLib.push_back((CComboBox*)pTmp);
				}

				CWndUserData_MapType::iterator it = Controls::getSingleton()._UserData.find(pTmp);
				if (it != Controls::getSingleton()._UserData.end())
				{
					Mem_Delete it->second;
					Controls::getSingleton()._UserData.erase(it);
				}

			}

			Mem_Delete pLine;
			itLine = _lines.erase(itLine);
			bRemoved = true;
		}
		else
		{
			if (bRemoved)
				M_Foreach(CWnd*, pTmp, CWnd_ListType, pLine->_controlsInLine)
			{
				CRect rc;
				pTmp->GetWindowRect(rc);
				pWnd->ScreenToClient(rc);
				rc.OffsetRect(0, -iOffsetH);
				pTmp->MoveWindow(rc);
			}

			itLine++;
		}
	}

	_iNextLinePosH -= iOffsetH;

	return iOffsetH;
}


void PropGroup::MoveUpControls(CWnd* pWnd, int iHeight)
{
	CRect rc;

	if (_pLaber)
	{
		_pLaber->GetWindowRect(rc);
		pWnd->ScreenToClient(rc);
		rc.OffsetRect(0, -iHeight);
		_pLaber->MoveWindow(rc);
	}

	if (_pButtonBmp)
	{
		_pButtonBmp->GetWindowRect(rc);
		pWnd->ScreenToClient(rc);
		rc.OffsetRect(0, -iHeight);
		_pButtonBmp->MoveWindow(rc);
	}

	for(PropLines_ListType::iterator itLine = _lines.begin(); itLine != _lines.end(); itLine++)
	{
		SPropLine* pLine = *itLine;
		M_Foreach(CWnd*, pTmp, CWnd_ListType, pLine->_controlsInLine)
		{
			pTmp->GetWindowRect(rc);
			pWnd->ScreenToClient(rc);
			rc.OffsetRect(0, -iHeight);
			pTmp->MoveWindow(rc);
		}
	}

	_iNextLinePosH -= iHeight;
}
