
// TigerContacts.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_MENU_DONATE, &CTigerContactsApp::OnMenuDonate)
	ON_COMMAND(ID_MENU_HELP_USAGE, &CTigerContactsApp::OnMenuHelpUsage)
END_MESSAGE_MAP()


// CTigerContactsApp ����

CTigerContactsApp::CTigerContactsApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CTigerContactsApp ����

CTigerContactsApp theApp;


// CTigerContactsApp ��ʼ��

BOOL CTigerContactsApp::InitInstance()
{
	setlocale(LC_CTYPE, "");

	Mem_New Cactus::LogManager(Cactus::eLogLevelDebug, "TigerContacts.log", 1);

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("ZHKSoft"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTigerContactsDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CTigerContactsViewMulti));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

#if 1
	//XP�»���ɱ����ļ�ʧ�����⣬������XP�´���β��ԡ�Win7�Ǻõġ�

	//�ƽ�SkinSharp
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

	//����һ���µ���ͼ
	_pViewPage = (CView *)RUNTIME_CLASS(CTigerContactsViewSingle)->CreateObject();

	//�ĵ�����ͼ����
	CDocument* pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();

	CCreateContext context;
	context.m_pCurrentDoc = pDoc;

	//������ͼ
	UINT m_IDFORANOTHERVIEW = AFX_IDW_PANE_FIRST + 1;
	CRect rect;
	_pViewPage->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_IDFORANOTHERVIEW, &context);
	_pViewPage->OnInitialUpdate();


	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->CenterWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	// ������/��
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
