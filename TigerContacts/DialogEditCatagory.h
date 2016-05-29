#pragma once
#include "afxwin.h"


// DialogEditCatagory dialog

class DialogEditCatagory : public CDialog
{
	DECLARE_DYNAMIC(DialogEditCatagory)

public:
	DialogEditCatagory(CWnd* pParent = NULL);   // standard constructor
	virtual ~DialogEditCatagory();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_CATEGORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CString _newCata;
	CCheckListBox _listbox;

	typedef Cactus::map<Cactus::GString, int>::type		CatasMapType;
	CatasMapType	_Catas;
	int				_iCount;

	Cactus::GStringVector	_add, _clear;

	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedOk();
	CString _curState;
};
