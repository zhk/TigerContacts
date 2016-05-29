// DialogEditCatagory.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "DialogEditCatagory.h"


// DialogEditCatagory dialog

IMPLEMENT_DYNAMIC(DialogEditCatagory, CDialog)

DialogEditCatagory::DialogEditCatagory(CWnd* pParent /*=NULL*/)
	: CDialog(DialogEditCatagory::IDD, pParent)
	, _newCata(_T(""))
	, _curState(_T(""))
{

}

DialogEditCatagory::~DialogEditCatagory()
{
}

void DialogEditCatagory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, _newCata);
	DDX_Control(pDX, IDC_LIST1, _listbox);
	DDX_Text(pDX, IDC_EDIT2, _curState);
}


BEGIN_MESSAGE_MAP(DialogEditCatagory, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &DialogEditCatagory::OnBnClickedButtonAdd)
	ON_LBN_SELCHANGE(IDC_LIST1, &DialogEditCatagory::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDOK, &DialogEditCatagory::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL DialogEditCatagory::OnInitDialog()
{
	CDialog::OnInitDialog();
	_listbox.SetCheckStyle(BS_AUTO3STATE);

	int i = 0;
	for (CatasMapType::const_iterator cit = _Catas.begin(); cit != _Catas.end(); ++cit)
	{
		_listbox.AddString(cit->first.Str().c_str());

		if (cit->second == _iCount)
			_listbox.SetCheck(i++, BST_CHECKED);
		else
			_listbox.SetCheck(i++, BST_INDETERMINATE);
	}

	return TRUE;
}


// DialogEditCatagory message handlers

void DialogEditCatagory::OnBnClickedButtonAdd()
{
	UpdateData();

	if(_newCata.GetLength() > 0)
	{
		if (_listbox.FindString(0, _newCata) != LB_ERR)
		{
			MessageBox(_("该条目已经存在！"), _("提示"), MB_OK | MB_ICONINFORMATION);
			return;
		}

		_listbox.AddString(_newCata);
		_listbox.SetCheck(_listbox.GetCount() - 1, BST_UNCHECKED);
	}
}

void DialogEditCatagory::OnLbnSelchangeList1()
{
	int curSel = _listbox.GetCurSel();

	int iState = _listbox.GetCheck(curSel);
	CString text;
	_listbox.GetText(curSel, text);
	
	if (iState == BST_CHECKED)
	{
		_curState = _("选中：") + text;
	}
	else if (iState == BST_UNCHECKED)
	{
		_curState = _("清除：") + text;
	}
	else
	{
		_curState = _("中间状态，无修改：") + text;
	}

	UpdateData(FALSE);
}

void DialogEditCatagory::OnBnClickedOk()
{
	UpdateData();

	_add.clear();
	_clear.clear();

	for(int i = 0; i < _listbox.GetCount(); ++i)
	{
		int iState = _listbox.GetCheck(i);
		
		CString text;
		_listbox.GetText(i, text);

		if (iState == BST_CHECKED)
		{
			if (_Catas[(LPCTSTR)text] != _iCount)
			{
				_add.push_back((LPCTSTR)text);
			}
		}
		else if (iState == BST_UNCHECKED)
		{
			if (_Catas[(LPCTSTR)text] != 0)
			{
				_clear.push_back((LPCTSTR)text);
			}
		}
	} 

	OnOK();
}
