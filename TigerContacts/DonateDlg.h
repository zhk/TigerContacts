#pragma once


// DonateDlg dialog

class DonateDlg : public CDialog
{
	DECLARE_DYNAMIC(DonateDlg)

public:
	DonateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~DonateDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_DONATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int _iType;
	afx_msg void OnBnClickedOK();
};
