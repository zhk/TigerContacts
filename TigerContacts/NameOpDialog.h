#pragma once


// NameOpDialog dialog

class NameOpDialog : public CDialog
{
	DECLARE_DYNAMIC(NameOpDialog)

public:
	NameOpDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~NameOpDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAME_OP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int _iOpType;
};
