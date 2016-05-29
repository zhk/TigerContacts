#pragma once


// CVCFImportDlg dialog

class CVCFImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CVCFImportDlg)

public:
	CVCFImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVCFImportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IMPORT_VCF };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int _iFileOrigin;
	CString _strFilePathName;
	bool	_bSingleVCF;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonBrowse();
	virtual BOOL OnInitDialog();
};
