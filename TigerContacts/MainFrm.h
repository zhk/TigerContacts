
// MainFrm.h : CMainFrame 类的接口
//

#pragma once

class CMainFrame : public CFrameWnd
{
protected: // 仅从序列化创建
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


// 重写
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


protected:  // 控件条嵌入成员
	CStatusBar			m_wndStatusBar;
	EViewType			_eUIType;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnActionFind();
};


