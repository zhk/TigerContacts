// VCFImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "VCFImportDlg.h"
#include "FolderDlg.h"

using namespace Cactus;


// CVCFImportDlg dialog

IMPLEMENT_DYNAMIC(CVCFImportDlg, CDialog)

CVCFImportDlg::CVCFImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVCFImportDlg::IDD, pParent)
	, _iFileOrigin(0)
	, _strFilePathName(_T(""))
	, _bSingleVCF(true)
{

}

CVCFImportDlg::~CVCFImportDlg()
{
}

void CVCFImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_VCF_NAME, _strFilePathName);
	DDX_Radio(pDX, IDC_RADIO_FILE_ORIGIN, _iFileOrigin);
}


BEGIN_MESSAGE_MAP(CVCFImportDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CVCFImportDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CVCFImportDlg::OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CVCFImportDlg message handlers

BOOL CVCFImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (_bSingleVCF)
	{
		SetWindowText(_("���뵥�� vcf �ļ�"));
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText(_("�ļ�����"));
	}
	else
	{
		SetWindowText(_("����Ŀ¼������ vcf �ļ�"));
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText(_("Ŀ¼����"));
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CVCFImportDlg::OnBnClickedButtonBrowse()
{
	UpdateData(TRUE);

	GString gStr(FileManager::GetInstance().GetModuleDir());
	CString strDataDir = gStr.Str().c_str();

	if (!_bSingleVCF)
	{
		CFolderDialog dlg(_("��ѡ�� vcf ���ڵ�Ŀ¼��"), strDataDir);
		if( dlg.DoModal() == IDOK )
		{
			_strFilePathName = dlg.GetFolderPath();
			if (_strFilePathName.GetLength() != 0)
			{
				_strFilePathName += "\\";
			}

			UpdateData(FALSE);
		}
	}
	else
	{
		try
		{
			CFileDialog dlg(TRUE);
			if( dlg.DoModal() == IDOK)
			{
				_strFilePathName = dlg.GetPathName();
				UpdateData(FALSE);
			}
		}
		catch(...)
		{
			_strFilePathName = "";
		}
	}
}

void CVCFImportDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

