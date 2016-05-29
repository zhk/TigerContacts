
// TigerContacts.h : TigerContacts 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "vcard.h"


// CTigerContactsApp:
// 有关此类的实现，请参阅 TigerContacts.cpp
//

class CTigerContactsApp : public CWinApp
{
public:
	CTigerContactsApp();

	virtual BOOL InitInstance();

	CView*	GetMultiView(){ return _pViewMulti; }
	CView*	GetPageView(){ return _pViewPage; }

// 实现
	afx_msg void OnAppAbout();
	afx_msg void OnViewMulti();
	afx_msg void OnViewIcon();
	afx_msg void OnViewPage();
	afx_msg void OnUpdateCmdUI_MultiSingle(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

protected:
	CView*	_pViewMulti;
	CView*	_pViewPage;
public:
	virtual int ExitInstance();
	afx_msg void OnMenuDonate();
	afx_msg void OnMenuHelpUsage();
};

extern CTigerContactsApp theApp;