// NameOpDialog.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "NameOpDialog.h"


// NameOpDialog dialog

IMPLEMENT_DYNAMIC(NameOpDialog, CDialog)

NameOpDialog::NameOpDialog(CWnd* pParent /*=NULL*/)
	: CDialog(NameOpDialog::IDD, pParent)
	, _iOpType(0)
{

}

NameOpDialog::~NameOpDialog()
{
}

void NameOpDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NAME_OP1, _iOpType);
}


BEGIN_MESSAGE_MAP(NameOpDialog, CDialog)
END_MESSAGE_MAP()


// NameOpDialog message handlers
