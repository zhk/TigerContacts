#pragma once


// CVCFExportDlg dialog

class CVCFExportDlg : public CDialog
{
	DECLARE_DYNAMIC(CVCFExportDlg)

public:
	CVCFExportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVCFExportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_EXPORT_VCF };

	int _iFileUsage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
