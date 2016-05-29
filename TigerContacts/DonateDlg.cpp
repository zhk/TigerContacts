// DonateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "DonateDlg.h"


// DonateDlg dialog

IMPLEMENT_DYNAMIC(DonateDlg, CDialog)

DonateDlg::DonateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(DonateDlg::IDD, pParent)
	, _iType(0)
{

}

DonateDlg::~DonateDlg()
{
}

void DonateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_DONATE, _iType);
}


BEGIN_MESSAGE_MAP(DonateDlg, CDialog)
	ON_BN_CLICKED(IDOK, &DonateDlg::OnBnClickedOK)
END_MESSAGE_MAP()


// DonateDlg message handlers

void DonateDlg::OnBnClickedOK()
{
	UpdateData(TRUE);
	
	if (_iType == 0)
	{
		::ShellExecute(0, _T("open"), _T("http://ershou.taobao.com/item.htm?id=17017707672"), 0, 0, SW_SHOWNORMAL);
	}
	else if (_iType == 1)
	{
		::ShellExecute(0, _T("open"), _T("http://ershou.taobao.com/item.htm?id=19653180219"), 0, 0, SW_SHOWNORMAL);
	}
	else if (_iType == 2)
	{
		::ShellExecute(0, _T("open"), _T("http://ershou.taobao.com/item.htm?id=17017995947"), 0, 0, SW_SHOWNORMAL);
	}

	OnOK();
}
