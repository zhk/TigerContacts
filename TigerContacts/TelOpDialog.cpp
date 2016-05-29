// TelOpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "TelOpDialog.h"


// TelOpDialog dialog

IMPLEMENT_DYNAMIC(TelOpDialog, CDialog)

TelOpDialog::TelOpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TelOpDialog::IDD, pParent)
	, _bRemoveMinus(TRUE)
	, _bRemoveSpace(TRUE)
	, _bRemovePlus86(FALSE)
{

}

TelOpDialog::~TelOpDialog()
{
}

void TelOpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TEL_OP1, _bRemoveMinus);
	DDX_Check(pDX, IDC_CHECK_TEL_OP2, _bRemoveSpace);
	DDX_Check(pDX, IDC_CHECK_TEL_OP3, _bRemovePlus86);
}


BEGIN_MESSAGE_MAP(TelOpDialog, CDialog)
END_MESSAGE_MAP()


// TelOpDialog message handlers
