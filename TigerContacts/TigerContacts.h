
// TigerContacts.h : TigerContacts Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "vcard.h"


// CTigerContactsApp:
// �йش����ʵ�֣������ TigerContacts.cpp
//

class CTigerContactsApp : public CWinApp
{
public:
	CTigerContactsApp();

	virtual BOOL InitInstance();

	CView*	GetMultiView(){ return _pViewMulti; }
	CView*	GetPageView(){ return _pViewPage; }

// ʵ��
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