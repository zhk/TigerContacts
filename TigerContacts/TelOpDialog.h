#pragma once


// TelOpDialog dialog

class TelOpDialog : public CDialog
{
	DECLARE_DYNAMIC(TelOpDialog)

public:
	TelOpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TelOpDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEL_OP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BOOL _bRemoveMinus;
	BOOL _bRemoveSpace;
	BOOL _bRemovePlus86;
};
