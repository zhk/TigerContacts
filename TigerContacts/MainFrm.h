
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once

class CMainFrame : public CFrameWnd
{
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)


	CToolBar		m_wndToolBar;
	CImageList		m_imgListToolBar;
	CMenu			m_menuMain;
	CMenu			m_menuSingleDoc;
	CComboBox		m_wndComboFind;
	CFont			m_fontDefault;


	bool			PrepareImageList();
	void			SwitchToListView();
	void			SwitchToPageView();


// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	virtual ~CMainFrame();

	enum EViewType
	{
		eViewMultiList,
		eViewIconList,
		eViewPage
	};
	void			SwitchToUI(EViewType e);
	EViewType		GetUIType() const { return _eUIType; }
	void			SetvCardCount(int iCount);


protected:  // �ؼ���Ƕ���Ա
	CStatusBar			m_wndStatusBar;
	EViewType			_eUIType;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnActionFind();
};


