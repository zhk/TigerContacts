#pragma once
#include "afxcmn.h"


class PhotoEditView;

class PhotoEditDlg : public CDialog
{
	DECLARE_DYNAMIC(PhotoEditDlg)

public:
	PhotoEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~PhotoEditDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_PHOTO_EDIT };

	Cactus::String		_strPhotoBytes;
	CxImage				_img;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	CToolBar		m_wndToolBar;
	CImageList		m_imgListToolBar;

	PhotoEditView*	_pPhotoView;

	int			_iSlider;
	CSliderCtrl _sliderCtrl;

	int			_iViewFinder;
	CSliderCtrl _sliderViewFinder;

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnMenuPhotoOpen();
	afx_msg void OnMenuPhotoUpdate();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
