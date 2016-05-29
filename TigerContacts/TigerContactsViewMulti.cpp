
// TigerContactsView.cpp : CTigerContactsView 类的实现
//

#include "stdafx.h"
#include "TigerContacts.h"

#include "TigerContactsDoc.h"
#include "TigerContactsViewMulti.h"

#include "ChiToLetter.h"

#include "NameOpDialog.h"
#include "TelOpDialog.h"
#include "DialogEditCatagory.h"

static int s_iSortCountName = 0;


// CTigerContactsView

IMPLEMENT_DYNCREATE(CTigerContactsViewMulti, CListView)

BEGIN_MESSAGE_MAP(CTigerContactsViewMulti, CListView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CTigerContactsViewMulti::OnLvnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTigerContactsViewMulti::OnNMDblclk)

	ON_COMMAND(ID_SELECT_ALL, &CTigerContactsViewMulti::OnSelectAll)
	ON_COMMAND(ID_SELECT_NONE, &CTigerContactsViewMulti::OnSelectNone)
	ON_COMMAND(ID_SELECT_INVERSE, &CTigerContactsViewMulti::OnSelectInverse)
	ON_COMMAND(ID_SELECT_NO_FN, &CTigerContactsViewMulti::OnSelectNoFn)
	ON_COMMAND(ID_SELECT_HAS_EMAIL, &CTigerContactsViewMulti::OnSelectHasEmail)
	ON_COMMAND(ID_SELECT_HAS_PHONE, &CTigerContactsViewMulti::OnSelectHasPhone)
	ON_COMMAND(ID_SELECT_NO_EMAIL, &CTigerContactsViewMulti::OnSelectNoEmail)
	ON_COMMAND(ID_SELECT_NO_PHONE, &CTigerContactsViewMulti::OnSelectNoPhone)
	ON_COMMAND(ID_SELECT_WITH_GROUP, &CTigerContactsViewMulti::OnSelectWithGroup)
	ON_COMMAND(ID_SELECT_HAS_PHOTO, &CTigerContactsViewMulti::OnSelectHasPhoto)
	ON_COMMAND(ID_SELECT_HAS_COMPANY, &CTigerContactsViewMulti::OnSelectHasCompany)
	ON_COMMAND(ID_SELECT_INCREMENTAL, &CTigerContactsViewMulti::OnSelectIncremental)


	ON_UPDATE_COMMAND_UI(ID_SELECT_ALL, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_ALL, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_NONE, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_INVERSE, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_NO_FN, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_HAS_EMAIL, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_HAS_PHONE, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_NO_EMAIL, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_NO_PHONE, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_WITH_GROUP, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_HAS_PHOTO, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_HAS_COMPANY, &CTigerContactsViewMulti::OnUpdateSelectOperations)
	ON_UPDATE_COMMAND_UI(ID_SELECT_INCREMENTAL, &CTigerContactsViewMulti::OnUpdateSelectIncremental)


	ON_COMMAND(ID_ACTION_DELETE, &CTigerContactsViewMulti::OnActionDelete)
	ON_COMMAND(ID_ACTION_DELETE_ALL, &CTigerContactsViewMulti::OnActionDeleteAll)
	ON_COMMAND(ID_ACTION_EDIT_PEOPLE, &CTigerContactsViewMulti::OnActionEditPeople)
	ON_COMMAND(ID_ACTION_REMOVE_DUPLICATED, &CTigerContactsViewMulti::OnActionRemoveDuplicated)
	ON_COMMAND(ID_ACTION_MERGE_SELECTION, &CTigerContactsViewMulti::OnActionMergeSelection)
	ON_COMMAND(ID_ACTION_DEL_SELECTED, &CTigerContactsViewMulti::OnActionDelSelected)
	ON_COMMAND(ID_ACTION_NAME_OP_SELECTION, &CTigerContactsViewMulti::OnActionNameOpSelection)
	ON_COMMAND(ID_ACTION_TEL_OP_SELECTION, &CTigerContactsViewMulti::OnActionTelOpSelection)
	ON_COMMAND(ID_ACTION_AUTO_MERGE_DUPLICATED, &CTigerContactsViewMulti::OnActionAutoMergeDuplicated)
	ON_COMMAND(ID_ACTION_EDIT_CATAGORY, &CTigerContactsViewMulti::OnActionEditCatagory)


	ON_UPDATE_COMMAND_UI(ID_ACTION_DELETE, &CTigerContactsViewMulti::OnUpdateActionDelete)
	ON_UPDATE_COMMAND_UI(ID_ACTION_EDIT_PEOPLE, &CTigerContactsViewMulti::OnUpdateActionEditPeople)
	ON_UPDATE_COMMAND_UI(ID_ACTION_REMOVE_DUPLICATED, &CTigerContactsViewMulti::OnUpdateActionRemoveDuplicated)
	ON_UPDATE_COMMAND_UI(ID_ACTION_MERGE_SELECTION, &CTigerContactsViewMulti::OnUpdateActionMergeSelection)
	ON_UPDATE_COMMAND_UI(ID_ACTION_DELETE_ALL, &CTigerContactsViewMulti::OnUpdateActionDeleteAll)
	ON_UPDATE_COMMAND_UI(ID_ACTION_DEL_SELECTED, &CTigerContactsViewMulti::OnUpdateActionDelSelected)
	ON_UPDATE_COMMAND_UI(ID_ACTION_NAME_OP_SELECTION, &CTigerContactsViewMulti::OnUpdateActionNameOpSelection)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TEL_OP_SELECTION, &CTigerContactsViewMulti::OnUpdateActionTelOpSelection)
	ON_UPDATE_COMMAND_UI(ID_ACTION_AUTO_MERGE_DUPLICATED, &CTigerContactsViewMulti::OnUpdateActionAutoMergeDuplicated)
	ON_UPDATE_COMMAND_UI(ID_ACTION_EDIT_CATAGORY, &CTigerContactsViewMulti::OnUpdateActionEditCatagory)


END_MESSAGE_MAP()

// CTigerContactsView 构造/析构

CTigerContactsViewMulti::CTigerContactsViewMulti()
{
	_bIncrementalCheck = true;
}

CTigerContactsViewMulti::~CTigerContactsViewMulti()
{
}

int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	vCard* pData1 = (vCard*)lParam1;
	vCard* pData2 = (vCard*)lParam2;

	CMyChiToLetter convert;

	int iColum = (int)lParamSort;
	if (iColum == 0)		//姓名
	{
		Cactus::String str1 = pData1->getValid_FN_N_Name().ToMbcs();
		Cactus::String str2 = pData2->getValid_FN_N_Name().ToMbcs();


		Cactus::String strPY1 = convert.GetLetter(str1);
		Cactus::String strPY2 = convert.GetLetter(str2);

		if (s_iSortCountName % 2 == 0)
			return strPY1 > strPY2;
		else
			return strPY1 < strPY2;
	}
	else if (iColum == 1)	//电话
	{
		return 0;
	}
	else if (iColum == 2)	//邮箱
	{
		return 0;
	}
	else if (iColum == 3)	//地址
	{
		return 0;
	}
	else if (iColum == 4)	//公司职务
	{
		return 0;
	}
	else if (iColum == 5)	//备注
	{
		return 0;
	}

	return (int)(lParam1 - lParam2);
}


void CTigerContactsViewMulti::OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	GetListCtrl().SortItems(MyCompareProc, pNMLV->iSubItem);

	if (pNMLV->iSubItem == 0)
	{
		s_iSortCountName++;
	}

	*pResult = 0;
}

BOOL CTigerContactsViewMulti::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style |= LVS_REPORT;
	cs.style |= LVS_SHOWSELALWAYS;
	//cs.style |= LVS_SORTASCENDING;

	//cs.style |= WS_CLIPSIBLINGS;
	cs.style |= WS_CLIPCHILDREN;

	return CListView::PreCreateWindow(cs);
}

// CTigerContactsView 绘制

void CTigerContactsViewMulti::OnDraw(CDC* pDC)
{
	CTigerContactsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}

void CTigerContactsViewMulti::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);

	//GetListCtrl().Invalidate();
	GetListCtrl().Arrange(LVA_DEFAULT);
}

int CTigerContactsViewMulti::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetListCtrl().SetExtendedStyle( GetListCtrl().GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SUBITEMIMAGES);
	int nCol = 0;
	GetListCtrl().InsertColumn(nCol++, _("姓名"),			LVCFMT_LEFT,	120);
	GetListCtrl().InsertColumn(nCol++, _("电话"),			LVCFMT_LEFT,	160);
	GetListCtrl().InsertColumn(nCol++, _("邮箱"),			LVCFMT_LEFT,	160);
	GetListCtrl().InsertColumn(nCol++, _("地址"),			LVCFMT_LEFT,	200);
	GetListCtrl().InsertColumn(nCol++, _("公司职务"),		LVCFMT_LEFT,	200);
	GetListCtrl().InsertColumn(nCol++, _("分组"),			LVCFMT_LEFT,	200);
	GetListCtrl().InsertColumn(nCol++, _("备注"),			LVCFMT_LEFT,	160);

	return 0;
}

void CTigerContactsViewMulti::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// 清除旧内容
	GetListCtrl().DeleteAllItems();

	//
	CTigerContactsDoc* pDoc = GetDocument();
	vCardList& vcardList = pDoc->GetvCardList();

	GetListCtrl().SetImageList(&pDoc->GetImageList(), LVSIL_NORMAL);
	GetListCtrl().SetImageList(&pDoc->GetImageListSmall(), LVSIL_SMALL);

	M_Foreach_Const(vCard, card, vCardList, vcardList)
	{
		_AddvCard(card);
	}
}

void CTigerContactsViewMulti::_UpdateRow(const vCard& card, int iIndex)
{
	int iImageIndex = card.GetImageIndex();
	if (iImageIndex == -1)
		iImageIndex = 0;

	CListCtrl& listCtrl = GetListCtrl();
	int nCol = 0;

	// Format Name
	vCardProperty propFN = card.getProperty(VC_FORMATTED_NAME);
	if (propFN.isValid())
		listCtrl.SetItem(iIndex, nCol++, LVIF_TEXT | LVIF_IMAGE, propFN.values().at(0).Str().c_str(), iImageIndex, 0, 0, 0);
	else
	{
		vCardProperty propN = card.getProperty(VC_NAME);
		if (propN.isValid())
			listCtrl.SetItem(iIndex, nCol++, LVIF_TEXT | LVIF_IMAGE, propN.values().at(0).Str().c_str(), iImageIndex, 0, 0, 0);
		else
			listCtrl.SetItem(iIndex, nCol++, LVIF_TEXT | LVIF_IMAGE, _T(""), iImageIndex, 0, 0, 0);
	}

	// 电话
	vCardPropertyList propsTEL = card.getProperties(VC_TELEPHONE);
	size_t szProps = propsTEL.size();
	if (szProps != 0)
	{
		CString strCap;
		if ( szProps > 1)
			strCap.Format(_T("%s (+%d)"), propsTEL.front().values().at(0).Str().c_str(), szProps - 1);
		else
			strCap = propsTEL.front().values().at(0).Str().c_str();

		listCtrl.SetItemText(iIndex, nCol++, strCap);
	}
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));

	// 邮箱
	vCardPropertyList propsEMail = card.getProperties(VC_EMAIL);
	szProps = propsEMail.size();
	if (szProps != 0)
	{
		CString strCap;
		if ( szProps > 1)
			strCap.Format(_T("%s (+%d)"), propsEMail.front().values().at(0).Str().c_str(), szProps - 1);
		else
			strCap = propsEMail.front().values().at(0).Str().c_str();

		listCtrl.SetItemText(iIndex, nCol++, strCap);
	}
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));

	// 地址
	vCardPropertyList propsAddr = card.getProperties(VC_ADDRESS);
	szProps = propsAddr.size();
	if (szProps != 0)
	{
		Cactus::GStringVector vals = propsAddr.front().values();
		CString strCap;

		Cactus::GString allContent;
		for (size_t t = 0; t < vals.size(); ++t)
		{
			allContent += vals[t] + " ";
		}

		if (szProps > 1)
			strCap.Format(_T("%s (+%d)"), allContent.Str().c_str(), szProps - 1);
		else
			strCap = allContent.Str().c_str();


		listCtrl.SetItemText(iIndex, nCol++, strCap);
	}
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));

	// 公司职务
	vCardProperty propCompany = card.getProperty(VC_ORGANIZATION);
	vCardProperty propTitle = card.getProperty(VC_TITLE);
	if (propCompany.isValid() || propTitle.isValid())
	{
		CString strCap;
		if (propCompany.isValid())
			strCap = propCompany.values().at(0).Str().c_str();

		if (propTitle.isValid())
		{
			strCap += _T(" ");
			strCap += propTitle.values().at(0).Str().c_str();
		}

		listCtrl.SetItemText(iIndex, nCol++, strCap);
	}
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));

	// 分组
	vCardProperty propCate = card.getProperty(VC_CATEGORIES);
	if (propCate.isValid())
		listCtrl.SetItemText(iIndex, nCol++, propCate.values().at(0).Str().c_str());
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));

	// 备注
	vCardProperty propNote = card.getProperty(VC_NOTE);
	if (propNote.isValid())
		listCtrl.SetItemText(iIndex, nCol++, propNote.values().at(0).Str().c_str());
	else
		listCtrl.SetItemText(iIndex, nCol++, _T(""));


	listCtrl.SetItemData(iIndex, DWORD_PTR(&card));
}


void CTigerContactsViewMulti::_AddvCard(const vCard& card)
{
	CListCtrl& listCtrl = GetListCtrl();
	int iIndex = listCtrl.InsertItem(GetListCtrl().GetItemCount(), _T(""));

	_UpdateRow(card, iIndex);
}


void CTigerContactsViewMulti::OnAddvCardList(const vCardList& vcardList, size_t iOffset/* = 0*/)
{
	vCardList::const_iterator cit = vcardList.begin();
	
	while(iOffset != 0)
	{
		cit++;
		iOffset--;
	}

	for (; cit != vcardList.end(); ++cit)
	{
		_AddvCard(*cit);
	}

}

void CTigerContactsViewMulti::OnRowUpdated(vCard* pCard)
{
	CListCtrl& listCtrl = GetListCtrl();
	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		vCard* pData = (vCard*)listCtrl.GetItemData(t);

		if (pCard == pData)
		{
			_UpdateRow(*pCard, t);
			break;
		}
	}
}


void CTigerContactsViewMulti::_FindContact(const Cactus::GString& strFN)
{
	CListCtrl& listCtrl = GetListCtrl();
	bool bEnsureVisible = false;
	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		vCard* pData = (vCard*)listCtrl.GetItemData(t);
		Cactus::GString str1 = strFN;
		str1.ToLower();
		Cactus::GString str2 = pData->getValid_FN_N_Name();
		str2.ToLower();

		if (str2.Str().find(str1.Str().c_str()) != Cactus::WString::npos)
		{
			listCtrl.SetItemState(t, LVIS_SELECTED, LVIS_SELECTED);
			if (!bEnsureVisible)
			{
				bEnsureVisible = true;
				listCtrl.EnsureVisible(t, FALSE);
			}
		}
		else
		{
			listCtrl.SetItemState(t, ~LVIS_SELECTED, LVIS_SELECTED);
		}
	}
}

//---------------------------------------------------------------------------------------------
void CTigerContactsViewMulti::OnSelectAll()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		listCtrl.SetCheck(t); 
	}

}

void CTigerContactsViewMulti::OnSelectNone()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		listCtrl.SetCheck(t, FALSE); 
	}

}

void CTigerContactsViewMulti::OnSelectInverse()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		listCtrl.SetCheck(t, !listCtrl.GetCheck(t)); 
	}

}

void CTigerContactsViewMulti::OnSelectNoFn()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);

				vCardProperty propFN = pData->getProperty(VC_FORMATTED_NAME);
				vCardProperty propN = pData->getProperty(VC_NAME);

				if (!propFN.isValid() && !propN.isValid())
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			vCardProperty propFN = pData->getProperty(VC_FORMATTED_NAME);
			vCardProperty propN = pData->getProperty(VC_NAME);

			listCtrl.SetCheck(t, (!propFN.isValid() && !propN.isValid()) ); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectHasEmail()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (pData->hasProperty(VC_EMAIL))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, pData->hasProperty(VC_EMAIL)); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectNoEmail()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (!pData->hasProperty(VC_EMAIL))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, !pData->hasProperty(VC_EMAIL)); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectHasPhone()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (pData->hasProperty(VC_TELEPHONE))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, pData->hasProperty(VC_TELEPHONE)); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectNoPhone()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (!pData->hasProperty(VC_TELEPHONE))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, !pData->hasProperty(VC_TELEPHONE)); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectWithGroup()
{
	// TODO: Add your command handler code here
}


void CTigerContactsViewMulti::OnSelectHasCompany()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (pData->hasProperty(VC_ORGANIZATION))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, pData->hasProperty(VC_ORGANIZATION)); 
		}
	}

}

void CTigerContactsViewMulti::OnSelectHasPhoto()
{
	CListCtrl& listCtrl = GetListCtrl();

	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (_bIncrementalCheck)
		{
			if (!listCtrl.GetCheck(t))
			{
				vCard* pData = (vCard*)listCtrl.GetItemData(t);
				if (pData->hasProperty(VC_PHOTO))
				{
					listCtrl.SetCheck(t, TRUE); 
				}
			}
		}
		else
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			listCtrl.SetCheck(t, pData->hasProperty(VC_PHOTO)); 
		}
	}

}


void CTigerContactsViewMulti::OnSelectIncremental()
{
	_bIncrementalCheck = !_bIncrementalCheck;
}
void CTigerContactsViewMulti::OnUpdateSelectIncremental(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
	pCmdUI->SetCheck(_bIncrementalCheck);
}


void CTigerContactsViewMulti::OnUpdateSelectOperations(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

//-----------------------------------------------------------------------------

void CTigerContactsViewMulti::OnActionDelete()
{
	CListCtrl& listCtrl = GetListCtrl();
	int iCount = 0;

	for (int t = 0; t < listCtrl.GetItemCount(); )
	{
		if (listCtrl.GetCheck(t))
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			CTigerContactsDoc* pDoc = GetDocument();
			if( pDoc->DeleteOnevCard(pData) )
			{
				listCtrl.DeleteItem(t);
				iCount++;
			}
		}
		else
		{
			++t;
		}
	}

	if (iCount == 0)
		MessageBox(_("你没有标记任何联系人。"), _("删除已标记的联系人"), MB_ICONINFORMATION | MB_OK);

}
void CTigerContactsViewMulti::OnUpdateActionDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionDeleteAll()
{
	if( MessageBox(_("确信要删除所有联系人吗？"), _("询问"), MB_ICONQUESTION | MB_OKCANCEL) != IDOK)
		return;

	CListCtrl& listCtrl = GetListCtrl();
	CTigerContactsDoc* pDoc = GetDocument();
	if( pDoc->DeleteAllvCards() )
	{
		listCtrl.DeleteAllItems();
	}
}
void CTigerContactsViewMulti::OnUpdateActionDeleteAll(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionEditPeople()
{
	CListCtrl& listCtrl = GetListCtrl();

	POSITION pos = listCtrl.GetFirstSelectedItemPosition();
	if (pos)
	{
		int nItem = listCtrl.GetNextSelectedItem(pos);
		vCard* pData = (vCard*)listCtrl.GetItemData(nItem);

		CTigerContactsDoc* pDoc = GetDocument();
		pDoc->SetCurrentvCard(pData);

		((CTigerContactsApp*)AfxGetApp())->OnViewPage();
	}
}
void CTigerContactsViewMulti::OnUpdateActionEditPeople(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() != 0);
}


void CTigerContactsViewMulti::OnActionDelSelected()
{
	int iCount = 0;

	CListCtrl& listCtrl = GetListCtrl();
	for (int t = 0; t < listCtrl.GetItemCount(); )
	{
		if ( LVIS_SELECTED == listCtrl.GetItemState(t, LVIS_SELECTED) )
		{
			CTigerContactsDoc* pDoc = GetDocument();
			pDoc->DeleteOnevCard( (vCard*)listCtrl.GetItemData(t) );

			listCtrl.DeleteItem(t);
			iCount++;
		}
		else
			++t;
	}

	if (iCount == 0)
	{
		MessageBox(_("没有选中的联系人！"), _("提示"), MB_ICONINFORMATION | MB_OK);
	}
}

void CTigerContactsViewMulti::OnUpdateActionDelSelected(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetSelectedCount() != 0);
}

// Find and Merge
void CTigerContactsViewMulti::OnActionRemoveDuplicated()
{
	CDialog dlg;
	dlg.Create(IDD_DIALOG_IN_PROGRESS);
	dlg.ShowWindow(SW_SHOW);
	dlg.CenterWindow();
	dlg.UpdateWindow();
	

	CWnd* pWnd = AfxGetMainWnd();
	pWnd->EnableWindow(FALSE);

	CTigerContactsDoc* pDoc = GetDocument();
	int iRemovedCount = pDoc->RemoveDuplicated();

	pWnd->EnableWindow();

	if( iRemovedCount > 0 )
	{
		CListCtrl& listCtrl = GetListCtrl();

		for (int t = 0; t < listCtrl.GetItemCount(); )
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			if ( pData->HasMask(0x1) )
			{
				listCtrl.DeleteItem(t);
				continue;
			}

			++t;
		}

		dlg.DestroyWindow();

		CString strTmp;
		strTmp.Format(_("一共删除了 %d 个重复的联系人。"), iRemovedCount);
		MessageBox(strTmp, _("删除重复的联系人"), MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		dlg.DestroyWindow();

		MessageBox(_("恭喜！没有发现重复的联系人。"), _("删除重复的联系人"), MB_ICONINFORMATION | MB_OK);
	}
}
void CTigerContactsViewMulti::OnUpdateActionRemoveDuplicated(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionMergeSelection()
{
	Cactus::list<vCard*>::type checked;

	int iHead = -1;

	CListCtrl& listCtrl = GetListCtrl();
	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (listCtrl.GetCheck(t))
		{
			if (iHead == -1)
				iHead = t;

			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			checked.push_back(pData);
		}
	}

	if (checked.size() < 2)
	{
		MessageBox(_("请标记两个及以上要合并的联系人。"), _("提示"), MB_ICONINFORMATION | MB_OK);
		return;
	}

	CTigerContactsDoc* pDoc = GetDocument();
	vCard* pHead = checked.front();
	checked.pop_front();

	pDoc->MergevCard(pHead, checked);

	checked.push_front(pHead);
	M_Foreach(vCard*, pCard, Cactus::list<vCard*>::type, checked)
	{
		for (int t = 0; t < listCtrl.GetItemCount(); )
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);

			if ( pCard == pData )
			{
				listCtrl.DeleteItem(t);
				continue;
			}

			++t;
		}
	}

	_AddvCard(*pHead);
}
void CTigerContactsViewMulti::OnUpdateActionMergeSelection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	OnActionEditPeople();

	*pResult = 0;
}

void CTigerContactsViewMulti::OnActionNameOpSelection()
{
	NameOpDialog dlg;
	if( dlg.DoModal() == IDOK )
	{
		int iCount = 0;
		vCard* pCard = 0;
		int iHandleCount = 0;

		CListCtrl& listCtrl = GetListCtrl();
		for (int t = 0; t < listCtrl.GetItemCount(); ++t)
		{
			if (listCtrl.GetCheck(t))
			{
				iCount++;
				vCard* pData = (vCard*)listCtrl.GetItemData(t);

				if (pCard == 0)
					pCard = pData;

				if( pData->normalize_N(dlg._iOpType) )
				{
					iHandleCount++;

					_UpdateRow(*pData, t);
				}
			}
		}

		if (iCount == 0)
		{
			MessageBox(_("没有标记任何联系人！"), _("提示"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			if (iHandleCount > 0)
			{
				CTigerContactsDoc* pDoc = GetDocument();
				pDoc->SetModifiedFlag(TRUE);
				pDoc->SetCurrentvCard(0);
				pDoc->SetCurrentvCard(pCard);
			}

			CString strTmp;
			strTmp.Format(_("共整理了 %d 个联系人。"), iHandleCount);
			MessageBox(strTmp, _("提示"), MB_ICONINFORMATION | MB_OK);
		}
	}
}
void CTigerContactsViewMulti::OnUpdateActionNameOpSelection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionTelOpSelection()
{
	TelOpDialog dlg;

	if( dlg.DoModal() == IDOK )
	{
		int iFlag = 0;
		if (dlg._bRemoveMinus)
			iFlag |= 0x1;
		if (dlg._bRemoveSpace)
			iFlag |= 0x10;
		if (dlg._bRemovePlus86)
			iFlag |= 0x100;

		if (iFlag == 0)
			return;


		int iCount = 0;
		vCard* pCard = 0;
		int iHandleCount = 0;

		CListCtrl& listCtrl = GetListCtrl();
		for (int t = 0; t < listCtrl.GetItemCount(); ++t)
		{
			if (listCtrl.GetCheck(t))
			{
				iCount++;
				vCard* pData = (vCard*)listCtrl.GetItemData(t);

				if (pCard == 0)
					pCard = pData;

				if (pData->normalize_TEL(iFlag))
				{
					iHandleCount++;

					_UpdateRow(*pData, t);
				}
			}
		}

		if (iCount == 0)
		{
			MessageBox(_("没有标记任何联系人！"), _("提示"), MB_ICONINFORMATION | MB_OK);
		}
		else
		{
			if (iHandleCount > 0)
			{
				CTigerContactsDoc* pDoc = GetDocument();
				pDoc->SetModifiedFlag(TRUE);
				pDoc->SetCurrentvCard(0);
				pDoc->SetCurrentvCard(pCard);
			}

			CString strTmp;
			strTmp.Format(_("共整理了 %d 个联系人。"), iHandleCount);
			MessageBox(strTmp, _("提示"), MB_ICONINFORMATION | MB_OK);
		}
	}
}
void CTigerContactsViewMulti::OnUpdateActionTelOpSelection(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionAutoMergeDuplicated()
{
	CDialog dlg;
	dlg.Create(IDD_DIALOG_IN_PROGRESS);
	dlg.ShowWindow(SW_SHOW);
	dlg.CenterWindow();
	dlg.UpdateWindow();


	CWnd* pWnd = AfxGetMainWnd();
	pWnd->EnableWindow(FALSE);

	CTigerContactsDoc* pDoc = GetDocument();
	int iCount = pDoc->MergeDuplicated();

	pWnd->EnableWindow();

	if( iCount > 0 )
	{
		CListCtrl& listCtrl = GetListCtrl();

		for (int t = 0; t < listCtrl.GetItemCount(); )
		{
			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			if ( pData->HasMask(0x1) )
			{
				listCtrl.DeleteItem(t);
				continue;
			}

			++t;
		}

		dlg.DestroyWindow();

		CString strTmp;
		strTmp.Format(_("一共合并了 %d 个重复的联系人。"), iCount);
		MessageBox(strTmp, _("合并重复的联系人"), MB_ICONINFORMATION | MB_OK);
	}
	else
	{
		dlg.DestroyWindow();

		MessageBox(_("恭喜！没有发现重复的联系人。"), _("合并重复的联系人"), MB_ICONINFORMATION | MB_OK);
	}
}
void CTigerContactsViewMulti::OnUpdateActionAutoMergeDuplicated(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}

void CTigerContactsViewMulti::OnActionEditCatagory()
{
	DialogEditCatagory dlg;
	dlg._iCount = 0;

	CListCtrl& listCtrl = GetListCtrl();
	for (int t = 0; t < listCtrl.GetItemCount(); ++t)
	{
		if (listCtrl.GetCheck(t))
		{
			dlg._iCount++;

			vCard* pData = (vCard*)listCtrl.GetItemData(t);
			vCardProperty propCat = pData->getProperty(VC_CATEGORIES);
			if (propCat.isValid())
			{
				Cactus::GStringVector& vects = propCat.values();
				for (size_t k = 0; k < vects.size(); ++k)
				{
					Cactus::GStringVector vs = vects[k].Split(L",");
					for (size_t j = 0; j < vs.size(); ++j)
					{
						dlg._Catas[vs[j]]++;
					}
				}
			}
		}
	}

	if( dlg.DoModal() == IDOK )
	{
		for (size_t k = 0; k < dlg._add.size(); ++k)
		{
			for (int t = 0; t < listCtrl.GetItemCount(); ++t)
			{
				if (listCtrl.GetCheck(t))
				{
					vCard* pData = (vCard*)listCtrl.GetItemData(t);
					pData->setCatagory(dlg._add[k], true);

					OnRowUpdated(pData);
				}
			}
		}

		for (size_t k = 0; k < dlg._clear.size(); ++k)
		{
			for (int t = 0; t < listCtrl.GetItemCount(); ++t)
			{
				if (listCtrl.GetCheck(t))
				{
					vCard* pData = (vCard*)listCtrl.GetItemData(t);
					pData->setCatagory(dlg._clear[k], false);

					OnRowUpdated(pData);
				}
			}
		}

		if (dlg._add.size() + dlg._clear.size() > 0)
		{
			CTigerContactsDoc* pDoc = GetDocument();
			pDoc->SetModifiedFlag(TRUE);
		}
	}
}
void CTigerContactsViewMulti::OnUpdateActionEditCatagory(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetListCtrl().GetItemCount() != 0);
}
