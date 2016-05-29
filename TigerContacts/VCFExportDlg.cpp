// VCFExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "VCFExportDlg.h"


// CVCFExportDlg dialog

IMPLEMENT_DYNAMIC(CVCFExportDlg, CDialog)

CVCFExportDlg::CVCFExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCFExportDlg::IDD, pParent)
	, _iFileUsage(0)
{

}

CVCFExportDlg::~CVCFExportDlg()
{
}

void CVCFExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_FILE_ORIGIN, _iFileUsage);
}


BEGIN_MESSAGE_MAP(CVCFExportDlg, CDialog)
END_MESSAGE_MAP()


// CVCFExportDlg message handlers
