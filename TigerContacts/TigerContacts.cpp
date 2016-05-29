
// TigerContacts.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "TigerContacts.h"

#include "MainFrm.h"
#include "AboutDlg.h"

#include "TigerContactsDoc.h"
#include "TigerContactsViewSingle.h"
#include "TigerContactsViewMulti.h"

#include "DonateDlg.h"

#include <SkinSharp/SkinH.h>


// CTigerContactsApp

BEGIN_MESSAGE_MAP(CTigerContactsApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CTigerContactsApp::OnAppAbout)
	ON_COMMAND(ID_MENU_VIEW_MULTI, &CTigerContactsApp::OnViewMulti)
	ON_COMMAND(ID_MENU_VIEW_ICON, &CTigerContactsApp::OnViewIcon)
	ON_COMMAND(ID_MENU_VIEW_PAGE, &CTigerContactsApp::OnViewPage)
	ON_UPDATE_COMMAND_UI(ID_MENU_VIEW_MULTI, &CTigerContactsApp::OnUpdateCmdUI_MultiSingle)
	ON_UPDATE_COMMAND_UI(ID_MENU_VIEW_ICON, &CTigerContactsApp::OnUpdateCmdUI_MultiSingle)
	ON_UPDATE_COMMAND_UI(ID_MENU_VIEW_PAGE, &CTigerContactsApp::OnUpdateCmdUI_MultiSingle)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_MENU_DONATE, &CTigerContactsApp::OnMenuDonate)
	ON_COMMAND(ID_MENU_HELP_USAGE, &CTigerContactsApp::OnMenuHelpUsage)
END_MESSAGE_MAP()


// CTigerContactsApp 构造

CTigerContactsApp::CTigerContactsApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CTigerContactsApp 对象

CTigerContactsApp theApp;


// CTigerContactsApp 初始化

BOOL CTigerContactsApp::InitInstance()
{
	setlocale(LC_CTYPE, "");

	Mem_New Cactus::LogManager(Cactus::eLogLevelDebug, "TigerContacts.log", 1);

	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("ZHKSoft"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTigerContactsDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CTigerContactsViewMulti));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

#if 1
	//XP下会造成保存文件失败问题，估计是XP下代码段不对。Win7是好的。

	//破解SkinSharp
	HMODULE hSkinBase = GetModuleHandleW(L"SkinHu.dll");
	unsigned char *pPatchByte = NULL;
	DWORD dwOldProtect = 0;
	if ( hSkinBase == NULL )
	{
		return FALSE;
	}
	//10021632       55                   push    ebp
	pPatchByte = (unsigned char*)((DWORD)hSkinBase + 0x00021632);
	VirtualProtectEx(GetCurrentProcess(), pPatchByte, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	*pPatchByte = 0x50;

	//1000F37F       52                   push    edx
	pPatchByte = (unsigned char*)((DWORD)hSkinBase + 0x0000F37F);
	VirtualProtectEx(GetCurrentProcess(), pPatchByte, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	*pPatchByte = 0x50;

#endif

	SkinH_AttachEx(_T("qqgame.she"), _T(""));
	//SkinH_SetAero(1);


	_pViewMulti = ((CFrameWnd*)m_pMainWnd)->GetActiveView();

	//创建一个新的视图
	_pViewPage = (CView *)RUNTIME_CLASS(CTigerContactsViewSingle)->CreateObject();

	//文档和视图关联
	CDocument* pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();

	CCreateContext context;
	context.m_pCurrentDoc = pDoc;

	//创建视图
	UINT m_IDFORANOTHERVIEW = AFX_IDW_PANE_FIRST + 1;
	CRect rect;
	_pViewPage->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_IDFORANOTHERVIEW, &context);
	_pViewPage->OnInitialUpdate();


	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->CenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


int CTigerContactsApp::ExitInstance()
{
	Cactus::LogManager::getSingleton().Flush();
	Mem_Delete Cactus::LogManager::getSingletonPtr();

	return CWinApp::ExitInstance();
}

void CTigerContactsApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


void CTigerContactsApp::OnViewMulti() 
{
	CMainFrame::EViewType e = ((CMainFrame*)m_pMainWnd)->GetUIType(); 
	if ( e == CMainFrame::eViewMultiList )
	{
		return;
	}
	else if ( e == CMainFrame::eViewIconList )
	{
		LONG lStyle;
		lStyle = GetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE);	
		lStyle &= ~LVS_ICON;
		lStyle |= LVS_REPORT;
		SetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE, lStyle);
	}
	else
	{
		UINT temp = ::GetWindowLong(_pViewPage->m_hWnd, GWL_ID);
		::SetWindowLong(_pViewPage->m_hWnd, GWL_ID, ::GetWindowLong(_pViewMulti->m_hWnd, GWL_ID));
		::SetWindowLong(_pViewMulti->m_hWnd, GWL_ID, temp);

		_pViewPage->ShowWindow(SW_HIDE);
		_pViewMulti->ShowWindow(SW_SHOW);

		((CFrameWnd*) m_pMainWnd)->SetActiveView(_pViewMulti);  
		((CFrameWnd*) m_pMainWnd)->RecalcLayout();
		_pViewMulti->Invalidate();
	}

	((CMainFrame*)m_pMainWnd)->SwitchToUI(CMainFrame::eViewMultiList);
}

void CTigerContactsApp::OnViewIcon() 
{
	CMainFrame::EViewType e = ((CMainFrame*)m_pMainWnd)->GetUIType(); 
	if ( e == CMainFrame::eViewMultiList )
	{
		LONG lStyle;
		lStyle = GetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE);	
		lStyle &= ~LVS_REPORT;
		lStyle |= LVS_ICON;
		SetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE, lStyle);
	}
	else if ( e == CMainFrame::eViewIconList )
	{
		return;
	}
	else
	{
		LONG lStyle;
		lStyle = GetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE);	
		lStyle &= ~LVS_REPORT;
		lStyle |= LVS_ICON;
		SetWindowLong(_pViewMulti->m_hWnd, GWL_STYLE, lStyle);

		UINT temp = ::GetWindowLong(_pViewPage->m_hWnd, GWL_ID);
		::SetWindowLong(_pViewPage->m_hWnd, GWL_ID, ::GetWindowLong(_pViewMulti->m_hWnd, GWL_ID));
		::SetWindowLong(_pViewMulti->m_hWnd, GWL_ID, temp);

		_pViewPage->ShowWindow(SW_HIDE);
		_pViewMulti->ShowWindow(SW_SHOW);

		((CFrameWnd*) m_pMainWnd)->SetActiveView(_pViewMulti);  
		((CFrameWnd*) m_pMainWnd)->RecalcLayout();
		_pViewMulti->Invalidate();
	}


	((CMainFrame*)m_pMainWnd)->SwitchToUI(CMainFrame::eViewIconList);
}

void CTigerContactsApp::OnViewPage()
{
	UINT temp = ::GetWindowLong(_pViewPage->m_hWnd, GWL_ID);
	::SetWindowLong(_pViewPage->m_hWnd, GWL_ID, ::GetWindowLong(_pViewMulti->m_hWnd, GWL_ID));
	::SetWindowLong(_pViewMulti->m_hWnd, GWL_ID, temp);

	_pViewMulti->ShowWindow(SW_HIDE);
	_pViewPage->ShowWindow(SW_SHOW);  	

	((CFrameWnd*) m_pMainWnd)->SetActiveView(_pViewPage);  
	((CFrameWnd*) m_pMainWnd)->RecalcLayout();
	_pViewPage->Invalidate();

	((CMainFrame*)m_pMainWnd)->SwitchToUI(CMainFrame::eViewPage);
}

void CTigerContactsApp::OnUpdateCmdUI_MultiSingle(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID == ID_MENU_VIEW_MULTI)
	{
		pCmdUI->SetCheck( ((CMainFrame*)m_pMainWnd)->GetUIType() == CMainFrame::eViewMultiList );
	}
	else if (pCmdUI->m_nID == ID_MENU_VIEW_ICON)
	{
		pCmdUI->SetCheck( ((CMainFrame*)m_pMainWnd)->GetUIType() == CMainFrame::eViewIconList );
	}
	else if (pCmdUI->m_nID == ID_MENU_VIEW_PAGE)
	{
		pCmdUI->SetCheck( ((CMainFrame*)m_pMainWnd)->GetUIType() == CMainFrame::eViewPage );
	}
}

void CTigerContactsApp::OnMenuDonate()
{
	DonateDlg dlg;
	dlg.DoModal();
}

void CTigerContactsApp::OnMenuHelpUsage()
{
	// TODO: Add your command handler code here
}
