
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "TigerContacts.h"

#include "MainFrm.h"
#include "TigerContacts.h"
#include "TigerContactsDoc.h"
#include "TigerContactsViewMulti.h"

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

//--------------------------------------------------------------------------------
#define M_CMD_Separator				0
#define M_CMD_Find_Combo			WM_USER + 1

//--------------------------------------------------------------------------------

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_INDICATOR_COUNT,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


//--------------------------------------------------------------------------------
static CString g_s_tbb_imge_files[] =
{
	//---------------------------------------------------
	_T("data/file_new.bmp"),				// 0
	_T("data/file_save.bmp"),				// 1
	//---------------------------------------------------
	_T("data/list_view.bmp"),				// 2
	_T("data/icon_view.bmp"),				// 3
	_T("data/page_view.bmp"),				// 4
	//---------------------------------------------------
	_T("data/action_add_people.bmp"),		// 5
	_T("data/action_edit_people.bmp"),		// 6
	_T("data/action_delete_people.bmp"),	// 7
	_T("data/find.bmp"),					// 8
	_T("data/action_merge.bmp"),			// 9
	//---------------------------------------------------
	_T("data/browse_first.bmp"),			//10
	_T("data/browse_prev.bmp"),				//11
	_T("data/browse_next.bmp"),				//12
	_T("data/browse_last.bmp"),				//13
	//---------------------------------------------------
	_T("data/add_nickname.bmp"),			//14
	_T("data/add_phone.bmp"),				//15
	_T("data/add_address.bmp"),				//16
	_T("data/add_email.bmp"),				//17
	_T("data/add_im.bmp"),					//18
	_T("data/add_relats.bmp"),				//19
	_T("data/add_company.bmp"),				//20
	_T("data/add_website.bmp"),				//21
	_T("data/add_event.bmp"),				//22
	_T("data/add_note.bmp"),				//23
	//---------------------------------------------------
	_T("data/person.bmp"),					//24
	_T("data/help.bmp"),					//25
	_T("data/file_open.bmp"),				//26
	_T("data/donate.bmp"),					//27
};

static TBBUTTON g_s_tbb_multi[] =
{
	{  0,  ID_FILE_NEW,					TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 26,  ID_FILE_OPEN,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  1,  ID_FILE_SAVE,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{  2,  ID_MENU_VIEW_MULTI,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{  3,  ID_MENU_VIEW_ICON,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{  4,  ID_MENU_VIEW_PAGE,			TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{ -1,  M_CMD_Separator,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{  5,  ID_ACTION_ADD_NEW,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  6,  ID_ACTION_EDIT_PEOPLE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  7,  ID_ACTION_DELETE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  8,  ID_ACTION_FIND,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Find_Combo,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{  9,  ID_ACTION_MERGE_SELECTION,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	//{ 27,  ID_MENU_DONATE,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 25,  ID_APP_ABOUT,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 }
};

static TBBUTTON g_s_tbb_single[] =
{
	{  0,  ID_FILE_NEW,				TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 26,  ID_FILE_OPEN,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  1,  ID_FILE_SAVE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{  2,  ID_MENU_VIEW_MULTI,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{  3,  ID_MENU_VIEW_ICON,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{  4,  ID_MENU_VIEW_PAGE,		TBSTATE_ENABLED, TBSTYLE_BUTTON | TBSTYLE_CHECK,	0, 0 },
	{ -1,  M_CMD_Separator,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{  5,  ID_ACTION_ADD_NEW,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	{ 10,  ID_BROWSE_FIRST,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 11,  ID_BROWSE_PREV,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 12,  ID_BROWSE_NEXT,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 13,  ID_BROWSE_LAST,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
//	{ 14,  ID_SINGLE_ADD_NICKNAME,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 15,  ID_SINGLE_ADD_PHONE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 16,  ID_SINGLE_ADD_ADDRESS,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 17,  ID_SINGLE_ADD_EMAIL,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 18,  ID_SINGLE_ADD_IM,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 19,  ID_SINGLE_ADD_RELATS,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
//	{ 20,  ID_SINGLE_ADD_COMPANY,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 21,  ID_SINGLE_ADD_WEBSITE,	TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 22,  ID_SINGLE_ADD_EVENT,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 23,  ID_SINGLE_ADD_NOTE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ -1,  M_CMD_Separator,			TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	//{ 27,  ID_MENU_DONATE,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{ 25,  ID_APP_ABOUT,			TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 }
};

//--------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_ACTION_FIND, &CMainFrame::OnActionFind)
END_MESSAGE_MAP()


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	_eUIType = eViewMultiList;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// 工具条
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_TOOLTIPS, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) )
		return -1;

	PrepareImageList();

	m_menuMain.LoadMenu(IDR_MAINFRAME);
	m_menuSingleDoc.LoadMenu(IDR_MENU_DOC);

	SwitchToListView();

	{
		m_fontDefault.CreatePointFont(110, _T("新宋体"));

		int index   =   0; 
		while(m_wndToolBar.GetItemID(index) != M_CMD_Find_Combo)
			index++; 

		m_wndToolBar.SetButtonInfo(index, M_CMD_Find_Combo, TBBS_SEPARATOR, 120); 
		CRect rect; 
		m_wndToolBar.GetItemRect(index, &rect); 
		rect.top += 6; 
		rect.bottom += 200; 

		if(!m_wndComboFind.Create(WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS
			, rect, &m_wndToolBar, M_CMD_Find_Combo)
			) 
		{ 
			return FALSE; 
		}

		m_wndComboFind.SetFont(&m_fontDefault);

		m_wndComboFind.ShowWindow(SW_SHOW); 

		//m_wndComboFind.AddString( _T("1 Select") ); 
		//m_wndComboFind.AddString( _T("2 Select") ); 
		//m_wndComboFind.AddString( _T("3 Select") ); 
		//m_wndComboFind.SetCurSel(0); 
	}

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.cx = 1280;
	cs.cy = 768;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 消息处理程序
bool CMainFrame::PrepareImageList()
{
	m_imgListToolBar.Create(32, 32, ILC_COLOR32|ILC_MASK, 0, 0);

	int	iButtons = sizeof(g_s_tbb_imge_files) / sizeof(g_s_tbb_imge_files[0]);
	for(int i = 0; i < iButtons; ++i)
	{
		HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetResourceHandle(), g_s_tbb_imge_files[i], IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		CBitmap bmp;
		bmp.Attach(hBitmap);
		m_imgListToolBar.Add(&bmp, RGB(0, 0, 0));
		bmp.DeleteObject();
	}

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imgListToolBar);
	m_wndToolBar.SetSizes(CSize(32 + 10, 32 + 6), CSize(32, 32));

	return true;
}

void CMainFrame::SwitchToListView()
{
	SetMenu(&m_menuMain);

	while( m_wndToolBar.GetToolBarCtrl().DeleteButton(0) );

	int	iButtons = sizeof(g_s_tbb_multi) / sizeof(g_s_tbb_multi[0]);
	for(int i = 0; i < iButtons; ++i)
	{
		m_wndToolBar.GetToolBarCtrl().AddButtons(1, &g_s_tbb_multi[i]);
	}

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CMainFrame::SwitchToPageView()
{
	SetMenu(&m_menuSingleDoc);

	while( m_wndToolBar.GetToolBarCtrl().DeleteButton(0) );

	int	iButtons = sizeof(g_s_tbb_single) / sizeof(g_s_tbb_single[0]);
	for(int i = 0; i < iButtons; ++i)
	{
		m_wndToolBar.GetToolBarCtrl().AddButtons(1, &g_s_tbb_single[i]);
	}

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
}

void CMainFrame::SwitchToUI(EViewType e)
{
	if (e == eViewMultiList)
	{
		if (_eUIType == eViewIconList)
		{
		}
		else
		{
			SwitchToListView();
			{
				int index   =   0; 
				while(m_wndToolBar.GetItemID(index) != M_CMD_Find_Combo) index++; 

				m_wndToolBar.SetButtonInfo(index, M_CMD_Find_Combo, TBBS_SEPARATOR, 120); 
				m_wndComboFind.ShowWindow(SW_SHOW);
			}
		}

	}
	else if (e == eViewIconList)
	{
		if (_eUIType == eViewMultiList)
		{
		}
		else
		{
			SwitchToListView();
			{
				int index   =   0; 
				while(m_wndToolBar.GetItemID(index) != M_CMD_Find_Combo) index++; 

				m_wndToolBar.SetButtonInfo(index, M_CMD_Find_Combo, TBBS_SEPARATOR, 120); 
				m_wndComboFind.ShowWindow(SW_SHOW);
			}
		}

	}
	else
	{
		SwitchToPageView();
		m_wndComboFind.ShowWindow(SW_HIDE);
	}

	_eUIType = e;
}


void CMainFrame::SetvCardCount( int iCount )
{
	CString strCap;
	strCap.Format(_T("联系人数量：%d"), iCount);
	m_wndStatusBar.SetPaneText(m_wndStatusBar.CommandToIndex(ID_INDICATOR_COUNT), strCap);
}

void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 800; 
	lpMMI->ptMinTrackSize.y = 600; 

	//lpMMI->ptMaxTrackSize.x = 1024; 
	//lpMMI->ptMaxTrackSize.y = 768; 

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::OnActionFind()
{
	CString strFind;
	m_wndComboFind.GetWindowText(strFind);
	if (strFind.GetLength() == 0)
	{
		m_wndComboFind.SetWindowText(_T("联系人名字"));
		m_wndComboFind.SetFocus();
		m_wndComboFind.SetEditSel(0, -1);

		return;
	}

	bool bFound = false;
	for (int i = 0; i < m_wndComboFind.GetCount(); ++i)
	{
		CString strList;
		m_wndComboFind.GetLBText(i, strList);
		if (strList == strFind)
		{
			bFound = true;
			break;
		}
	}

	if (!bFound)
		m_wndComboFind.InsertString(0, strFind);

	CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
	pViewMulti->_FindContact((LPCTSTR)strFind);
	pViewMulti->SetFocus();
}
