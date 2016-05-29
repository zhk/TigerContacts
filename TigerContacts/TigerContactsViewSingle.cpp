
// TigerContactsView.cpp : CTigerContactsView 类的实现
//

#include "stdafx.h"
#include "TigerContacts.h"

#include "TigerContactsDoc.h"
#include "TigerContactsViewSingle.h"
#include "TigerContactsViewMulti.h"

#include "PhotoEditDlg.h"

using namespace Cactus;

//------------------------------------------------------------------------------------
IMPLEMENT_DYNCREATE(CTigerContactsViewSingle, CScrollView)

BEGIN_MESSAGE_MAP(CTigerContactsViewSingle, CScrollView)
	ON_COMMAND(ID_BROWSE_FIRST, &CTigerContactsViewSingle::OnBrowseFirst)
	ON_COMMAND(ID_BROWSE_PREV, &CTigerContactsViewSingle::OnBrowsePrev)
	ON_COMMAND(ID_BROWSE_NEXT, &CTigerContactsViewSingle::OnBrowseNext)
	ON_COMMAND(ID_BROWSE_LAST, &CTigerContactsViewSingle::OnBrowseLast)

	ON_COMMAND(ID_SINGLE_ADD_NICKNAME, &CTigerContactsViewSingle::OnSingleAddNickname)
	ON_COMMAND(ID_SINGLE_ADD_ADDRESS, &CTigerContactsViewSingle::OnSingleAddAddress)
	ON_COMMAND(ID_SINGLE_ADD_COMPANY, &CTigerContactsViewSingle::OnSingleAddCompany)
	ON_COMMAND(ID_SINGLE_ADD_EMAIL, &CTigerContactsViewSingle::OnSingleAddEmail)
	ON_COMMAND(ID_SINGLE_ADD_EVENT, &CTigerContactsViewSingle::OnSingleAddEvent)
	ON_COMMAND(ID_SINGLE_ADD_IM, &CTigerContactsViewSingle::OnSingleAddIm)
	ON_COMMAND(ID_SINGLE_ADD_NOTE, &CTigerContactsViewSingle::OnSingleAddNote)
	ON_COMMAND(ID_SINGLE_ADD_PHONE, &CTigerContactsViewSingle::OnSingleAddPhone)
	ON_COMMAND(ID_SINGLE_ADD_RELATS, &CTigerContactsViewSingle::OnSingleAddRelats)
	ON_COMMAND(ID_SINGLE_ADD_WEBSITE, &CTigerContactsViewSingle::OnSingleAddWebsite)
	ON_WM_CREATE()

	ON_COMMAND_RANGE(M_Cmd_Button_Start, M_Cmd_Button_END, &CTigerContactsViewSingle::OnCommandRange_ButtonClicked)
	ON_CONTROL_RANGE(EN_CHANGE, M_Cmd_Edit_Start, M_Cmd_Edit_END, &CTigerContactsViewSingle::OnCommandRange_EditUpdated)
	ON_CONTROL_RANGE(CBN_SELCHANGE, M_Cmd_Combo_Start, M_Cmd_Combo_END, &CTigerContactsViewSingle::OnCommandRange_ComboSelected)
	ON_CONTROL_RANGE(CBN_EDITCHANGE, M_Cmd_Combo_Start, M_Cmd_Combo_END, &CTigerContactsViewSingle::OnCommandRange_ComboEditUpdated)


	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CTigerContactsView 构造/析构

CTigerContactsViewSingle::CTigerContactsViewSingle()
{
	Mem_New Controls(this);
}

CTigerContactsViewSingle::~CTigerContactsViewSingle()
{
	M_Foreach(PropGroup*, pProp, NamePropGroups_ListType, _Groups)
	{
		Mem_Delete pProp;
	}
	_Groups.clear();
}

BOOL CTigerContactsViewSingle::PreCreateWindow(CREATESTRUCT& cs)
{
	//  CREATESTRUCT cs 来修改窗口类或样式
//	cs.style |= WS_CLIPCHILDREN;

	return CScrollView::PreCreateWindow(cs);
}

// CTigerContactsView 绘制

void CTigerContactsViewSingle::OnDraw(CDC* pDC)
{
	CTigerContactsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}

void CTigerContactsViewSingle::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pCard = pDoc->GetFirstvCard();

	if (pCard)
	{
		_pCard = 0;
		SetvCard(pCard);
	}
	else
	{
		SetvCard(0);

		CSize sizeTotal;
		sizeTotal.cx = 600;
		sizeTotal.cy = 600;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
}


int CTigerContactsViewSingle::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CTigerContactsViewSingle::OnDestroy()
{
	Mem_Delete Controls::getSingletonPtr();

	CScrollView::OnDestroy();
}

BOOL CTigerContactsViewSingle::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	//pDC->FillSolidRect(rcClient, GetSysColor(COLOR_3DFACE));
	pDC->FillSolidRect(rcClient, RGB(221, 236, 255));

	return TRUE;

	//return CScrollView::OnEraseBkgnd(pDC);
}

//------------------------------------------------------------------------------------
void CTigerContactsViewSingle::SetvCard(vCard* pCard)
{
	if (_pCard == pCard)
		return;

	_pCard = pCard;

	M_Foreach(PropGroup*, g, NamePropGroups_ListType, _Groups)
		Mem_Delete g;
	_Groups.clear();
	Controls::getSingleton().ReturnAllUsedControls();

	if (!_pCard)
		return;

	SetScrollPos(SB_VERT, 0, FALSE);

	// 头像
	CStatic* pWnd = Controls::getSingleton().GetStaticBmp();
	pWnd->MoveWindow( CRect(CPoint(130, 30), CSize(96, 96)), FALSE );
	pWnd->SetIcon( GetDocument()->GetImageList().ExtractIcon(_pCard->GetImageIndex()) );
	pWnd->ShowWindow(SW_SHOW);

	CButton* pBtn = Controls::getSingleton().GetButton();
	pBtn->MoveWindow( CRect(CPoint(130 - 30, 30 + 102), CSize(45, M_LineH)), FALSE );
	pBtn->SetWindowText( _("删除") );
	pBtn->ShowWindow(SW_SHOW);
	UserData* pUserData = Mem_New UserData(VC_PHOTO, 0, (int)pWnd, "Photo_Del");
	Controls::getSingleton()._UserData[pBtn] = pUserData;

	pBtn = Controls::getSingleton().GetButton();
	pBtn->MoveWindow( CRect(CPoint(130 - 30 + 50, 30 + 102), CSize(45, M_LineH)), FALSE );
	pBtn->SetWindowText( _("更改") );
	pBtn->ShowWindow(SW_SHOW);
	pUserData = Mem_New UserData(VC_PHOTO, 0, (int)pWnd, "Photo_Change");
	Controls::getSingleton()._UserData[pBtn] = pUserData;

	pBtn = Controls::getSingleton().GetButton();
	pBtn->MoveWindow( CRect(CPoint(130 - 30 + 50 + 50, 30 + 102), CSize(45, M_LineH)), FALSE );
	pBtn->SetWindowText( _("导出") );
	pBtn->ShowWindow(SW_SHOW);
	pUserData = Mem_New UserData(VC_PHOTO, 0, (int)pWnd, "Photo_Export");
	Controls::getSingleton()._UserData[pBtn] = pUserData;


	// Build VC_FORMATTED_NAME
	PropGroup* pGroup = Mem_New PropGroup(_pCard, VC_FORMATTED_NAME, M_Top_1 - 4 * (M_LineH + M_LineH_M), false, false, true);
	_Groups.push_back(pGroup);

	// Build VC_NAME
	// 4名称前缀;1姓氏;3中间名;2名字;5名称后缀
	pGroup = Mem_New PropGroup(_pCard, VC_NAME, _Groups.back()->GetNextLineHeightPos(), false, false, true);
	_Groups.push_back(pGroup);

	// Build VC_NICKNAME
	pGroup = Mem_New PropGroup(_pCard, VC_NICKNAME, _Groups.back()->GetNextLineHeightPos(), false, false, true);
	_Groups.push_back(pGroup);

	// Build VC_ORGANIZATION
	pGroup = Mem_New PropGroup(_pCard, VC_ORGANIZATION, _Groups.back()->GetNextLineHeightPos(), false, false, true);
	_Groups.push_back(pGroup);

	// Build VC_TITLE
	pGroup = Mem_New PropGroup(_pCard, VC_TITLE, _Groups.back()->GetNextLineHeightPos(), false, false, true);
	_Groups.push_back(pGroup);


	M_Foreach_Const(GString, gStr, GStringVector, Controls::getSingleton()._supportPropNames)
	{
		NameLabelGroupMapType::iterator it = Controls::getSingleton()._PropLabelsGroup.find(gStr);
		if (it != Controls::getSingleton()._PropLabelsGroup.end())
		{
			pGroup = Mem_New PropGroup(_pCard, it->second
			, _Groups.back()->GetNextLineHeightPos(), true, true, false
				);
		}
		else
		{
			pGroup = Mem_New PropGroup(_pCard, gStr, _Groups.back()->GetNextLineHeightPos(), true, true, false);
		}
		_Groups.push_back(pGroup);
	}

	UpdateScrollRange();
}

void CTigerContactsViewSingle::UpdateScrollRange()
{
	if (_Groups.size() > 0)
	{
		CSize sizeTotal;
		sizeTotal.cx = 600;
		sizeTotal.cy = _Groups.back()->GetNextLineHeightPos() + 30;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}
}

void CTigerContactsViewSingle::PropGroupIncrease( PropGroup* pGroup, int iH )
{
	for (NamePropGroups_ListType::reverse_iterator it = _Groups.rbegin(); it != _Groups.rend(); ++it)
	{
		PropGroup* pCurGroup = *it;
		if (pCurGroup != pGroup)
		{
			pCurGroup->MoveDownControls(this, iH);
		}
		else
		{
			break;
		}
	}
}

void CTigerContactsViewSingle::PropGroupDescrease( PropGroup* pGroup, int iIndex/* = 0*/ )
{
	int iH = pGroup->RemoveOneLine( this, iIndex );

	for (NamePropGroups_ListType::reverse_iterator it = _Groups.rbegin(); it != _Groups.rend(); ++it)
	{
		PropGroup* pCurGroup = *it;
		if (pCurGroup != pGroup)
		{
			pCurGroup->MoveUpControls( this, iH );
		}
		else
		{
			break;
		}
	}
}
//------------------------------------------------------------------------------------

void CTigerContactsViewSingle::OnBrowseFirst()
{
	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pCard = pDoc->GetFirstvCard();

	if (pCard)
	{
		SetvCard(pCard);
	}
}

void CTigerContactsViewSingle::OnBrowsePrev()
{
	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pCard = pDoc->GetPrevvCard();

	if (pCard)
	{
		SetvCard(pCard);
	}
}

void CTigerContactsViewSingle::OnBrowseNext()
{
	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pCard = pDoc->GetNextvCard();

	if (pCard)
	{
		SetvCard(pCard);
	}
}

void CTigerContactsViewSingle::OnBrowseLast()
{
	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pCard = pDoc->GetLastvCard();

	if (pCard)
	{
		SetvCard(pCard);
	}
}

void CTigerContactsViewSingle::OnSingleAddNickname()
{
	// TODO: Add your command handler code here
}

void CTigerContactsViewSingle::OnSingleAddCompany()
{
	// TODO: Add your command handler code here
}



void CTigerContactsViewSingle::OnSingleAddAddress()
{
	UINT ID = Controls::getSingleton()._CmdButtons[VC_ADDRESS];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddEmail()
{
	UINT ID = Controls::getSingleton()._CmdButtons[VC_EMAIL];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddEvent()
{
	UINT ID = Controls::getSingleton()._CmdButtons["X-ABDATE"];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddIm()
{
	UINT ID = Controls::getSingleton()._CmdButtons["IM"];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddNote()
{
	UINT ID = Controls::getSingleton()._CmdButtons[VC_NOTE];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddPhone()
{
	UINT ID = Controls::getSingleton()._CmdButtons[VC_TELEPHONE];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddRelats()
{
	UINT ID = Controls::getSingleton()._CmdButtons["X-ABRELATEDNAMES"];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

void CTigerContactsViewSingle::OnSingleAddWebsite()
{
	UINT ID = Controls::getSingleton()._CmdButtons[VC_URL];
	if (ID != 0)
	{
		OnCommandRange_ButtonClicked(ID);
	}
}

//------------------------------------------------------------------------------------
void CTigerContactsViewSingle::OnCommandRange_ButtonClicked(UINT nID)
{
	CWnd* pButton = GetDlgItem(nID);

	CWndUserData_MapType::iterator it = Controls::getSingleton()._UserData.find(pButton);
	if (it == Controls::getSingleton()._UserData.end())
		return;

	UserData* pUserData = it->second;

	if (pUserData->_strParam == "BtnAdd")				//增加属性行
	{
		SPropInfo* pPropInfo = Controls::getSingleton().GetAvaliablePropInfo(pUserData->_strKey);
		if (pPropInfo == 0)
			return;

		GStringVector vals;
		if (pUserData->_strKey == VC_ADDRESS)
		{
			//vals.push_back(_T("邮政信箱："));
			//vals.push_back(_T("邻居："));
			//vals.push_back(_T("街道："));
			//vals.push_back(_T("市县："));
			//vals.push_back(_T("州省："));
			//vals.push_back(_T("邮编："));
			//vals.push_back(_T("国家："));

			for (int iCount = 0; iCount < 7; ++iCount)
			{
				vals.push_back("");
			}
		}
		else
			vals.push_back("");

		vCardProperty prop( vCardProperty(pUserData->_strKey, vals, pPropInfo->_strParam) );
		prop.setExtendProp(pPropInfo->_bExtend);
		if (pPropInfo->_bExtend)
		{
			if (pPropInfo->_strComboKey == "CUSTOM")
				prop.setExtendLabelValue(pPropInfo->_strComboCap);
			else
				prop.setExtendLabelValue(pPropInfo->_strComboKey);
		}
		if (pPropInfo->_bReplaceName)
		{
			prop.name(pPropInfo->_strComboKey);
		}
		_pCard->pushProperty( prop );
		_pCard->updateReversion();
		GetDocument()->SetModifiedFlag(TRUE);

		PropGroup* pGroup = (PropGroup*)(pUserData->_intParam2);
		int iH = pGroup->AddPropLine(prop, GetScrollPos(SB_VERT), true);

		PropGroupIncrease(pGroup, iH);

		UpdateScrollRange();

		//Update MultiView
		CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
		pViewMulti->OnRowUpdated(_pCard);

	}
	else if (pUserData->_strParam == "BtnRemove")		//删除属性行
	{
		int iIndex = pUserData->_intParam1;
		PropGroup* pGroup = (PropGroup*)(pUserData->_intParam2);

		PropGroupDescrease(pGroup, iIndex);

		_pCard->removeProperty(iIndex);
		GetDocument()->SetModifiedFlag(TRUE);

		UpdateScrollRange();

		//Update MultiView
		CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
		pViewMulti->OnRowUpdated(_pCard);

	}
	else if (pUserData->_strParam == "Photo_Del")		//删除图片
	{
		if (_pCard->GetImageIndex() <= 0)
		{
			MessageBox(_("此联系人没有头像！"), _("提示"), MB_OK | MB_ICONINFORMATION);
			return;
		}

		_pCard->removeProperty(VC_PHOTO);
		_pCard->SetImageIndex(0);
		GetDocument()->SetModifiedFlag(TRUE);

		CStatic* pStatic = (CStatic*)(pUserData->_intParam2);
		pStatic->SetIcon( GetDocument()->GetImageList().ExtractIcon(0) );

		//Update MultiView
		CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
		pViewMulti->OnRowUpdated(_pCard);

	}
	else if (pUserData->_strParam == "Photo_Export")	//导出图片
	{
		if (_pCard->GetImageIndex() <= 0)
		{
			MessageBox(_("此联系人没有头像！"), _("提示"), MB_OK | MB_ICONINFORMATION);
			return;
		}

		GString gStr = _pCard->getValid_FN_N_Name();

		CFileDialog dlgFileDlg(FALSE, _T("jpg"), gStr.Str().c_str()
			, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("JPEG Files (*.jpg)|*.jpg|Bitmap Files (*.bmp)|*.bmp||") );

		if( dlgFileDlg.DoModal() == IDOK)
		{
			gStr = (LPCTSTR)dlgFileDlg.GetPathName();
			gStr.ToUpper();
			bool b = false;
			if (gStr.EndWith("BMP"))
				b = _pCard->exportPhoto(gStr, CXIMAGE_FORMAT_BMP);
			else
				b = _pCard->exportPhoto(gStr, CXIMAGE_FORMAT_JPG);

			if (b)
				::ShellExecute(0, _T("open"), gStr.Str().c_str(), 0, 0, SW_SHOW);
		}
	}
	else if (pUserData->_strParam == "Photo_Change")	//更改图片
	{
		PhotoEditDlg photoEditDlg;
		if ( photoEditDlg.DoModal() == IDOK)
		{
			_pCard->addProperty(vCardProperty(VC_PHOTO, photoEditDlg._strPhotoBytes.c_str(), "TYPE=JPEG;ENCODING=BASE64"));
			_pCard->updateReversion();

			CxImage imgBig;
			photoEditDlg._img.Resample(96, 96, CxImage::IM_BILINEAR, &imgBig);

			CxImage imgSmall;
			photoEditDlg._img.Resample(16, 16, CxImage::IM_BILINEAR, &imgSmall);


			HBITMAP hBitmap = imgBig.MakeBitmap();
			CBitmap bmp;
			bmp.Attach(hBitmap);
			int iImgIndex = GetDocument()->GetImageList().Add(&bmp, RGB(0, 0, 0));
			bmp.DeleteObject();

			_pCard->SetImageIndex(iImgIndex);

			hBitmap = imgSmall.MakeBitmap();
			bmp.Attach(hBitmap);
			iImgIndex = GetDocument()->GetImageListSmall().Add(&bmp, RGB(0, 0, 0));
			bmp.DeleteObject();

			GetDocument()->SetModifiedFlag(TRUE);

			CStatic* pStatic = (CStatic*)(pUserData->_intParam2);
			pStatic->SetIcon( GetDocument()->GetImageList().ExtractIcon(iImgIndex) );

			//Update MultiView
			CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
			pViewMulti->OnRowUpdated(_pCard);
		}
	}
	
}

void CTigerContactsViewSingle::OnCommandRange_EditUpdated(UINT nID)
{
	CWnd* pEdit = GetDlgItem(nID);

	CWndUserData_MapType::iterator it = Controls::getSingleton()._UserData.find(pEdit);
	if (it == Controls::getSingleton()._UserData.end())
		return;

	UserData* pUserData = it->second;

	//SUserData(_strPropKey, iIndex, iCount++, "Edit")
	if (pUserData->_strParam != "Edit")
		return;

	vCardProperty* pProp = _pCard->getProperty(pUserData->_intParam1);
	if (!pProp)
		return;

	GStringVector& values = pProp->values();
	while (int(values.size()) <= pUserData->_intParam2)
		values.push_back("");

	CString strText;
	pEdit->GetWindowText(strText);
	
	GString gStr((LPCTSTR)strText);
	values[pUserData->_intParam2] = gStr;

	_pCard->updateReversion();

	//Update MultiView
	CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
	pViewMulti->OnRowUpdated(_pCard);

}

void CTigerContactsViewSingle::OnCommandRange_ComboSelected(UINT nID)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);

	CWndUserData_MapType::iterator it = Controls::getSingleton()._UserData.find(pCombo);
	if (it == Controls::getSingleton()._UserData.end())
		return;

	UserData* pUserData = it->second;

	//UserData(_strLabelSection, iIndex, 0, "Combo");
	if (pUserData->_strParam != "Combo")
		return;

	vCardProperty* pProp = _pCard->getProperty(pUserData->_intParam1);
	if (!pProp)
		return;


	CString cStr;
	//pCombo->GetWindowText(cStr);	// Text is old.
	pCombo->GetLBText(pCombo->GetCurSel(), cStr);
	SPropInfo* pPropInfo = Controls::getSingleton().GetPropInfoFromCaption(pUserData->_strKey, (LPCTSTR)cStr);
	if (!pPropInfo)
		return;

	pProp->setExtendProp(pPropInfo->_bExtend);
	if (pPropInfo->_strParam.Str().length() > 0)
	{
		vCardParamList params = vCardParam::fromGString(pPropInfo->_strParam.Str());
		pProp->params(params);
	}

	if (pPropInfo->_bReplaceName)	//For IM
	{
		pProp->name(pPropInfo->_strComboKey);
	}

	_pCard->updateReversion();
	GetDocument()->SetModifiedFlag(TRUE);
}

void CTigerContactsViewSingle::OnCommandRange_ComboEditUpdated(UINT nID)
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(nID);

	CWndUserData_MapType::iterator it = Controls::getSingleton()._UserData.find(pCombo);
	if (it == Controls::getSingleton()._UserData.end())
		return;

	UserData* pUserData = it->second;

	//UserData(_strLabelSection, iIndex, 0, "Combo");
	if (pUserData->_strParam != "Combo")
		return;

	vCardProperty* pProp = _pCard->getProperty(pUserData->_intParam1);
	if (!pProp)
		return;


	CString cStr;
	pCombo->GetWindowText(cStr);
	SPropInfo* pPropInfo = Controls::getSingleton().GetPropInfoFromCaption(pUserData->_strKey, (LPCTSTR)cStr);
	if (!pPropInfo)
		return;

	pProp->setExtendProp(pPropInfo->_bExtend);
	if (pPropInfo->_strParam.Str().length() > 0)
	{
		vCardParamList params = vCardParam::fromGString(pPropInfo->_strParam.Str());
		pProp->params(params);
	}

	if (pPropInfo->_strComboKey == "CUSTOM")
	{
		pProp->setExtendLabelValue((LPCTSTR)cStr);
	}

	_pCard->updateReversion();
	GetDocument()->SetModifiedFlag(TRUE);
}
