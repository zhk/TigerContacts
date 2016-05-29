
// TigerContactsView.h : CTigerContactsView 类的接口
//


#pragma once

#include <afxcview.h>


class CTigerContactsViewMulti : public CListView
{
protected:
	CTigerContactsViewMulti();
	DECLARE_DYNCREATE(CTigerContactsViewMulti)

	virtual void OnInitialUpdate(); // 构造后第一次调用

	void	_AddvCard(const vCard& card);
	void	_FindContact(const Cactus::GString& strFN);
	void	_UpdateRow(const vCard& card, int iIndex);

	bool	_bIncrementalCheck;

	DECLARE_MESSAGE_MAP()
public:
	virtual ~CTigerContactsViewMulti();
	CTigerContactsDoc* GetDocument() const;

	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void		OnAddvCardList(const vCardList& vcardList, size_t iOffset = 0);
	void		OnRowUpdated(vCard* pCard);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLvnColumnclick(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnSelectAll();
	afx_msg void OnSelectNone();
	afx_msg void OnSelectInverse();
	afx_msg void OnSelectNoFn();
	afx_msg void OnSelectHasEmail();
	afx_msg void OnSelectHasPhone();
	afx_msg void OnSelectNoEmail();
	afx_msg void OnSelectNoPhone();
	afx_msg void OnSelectWithGroup();
	afx_msg void OnSelectHasCompany();
	afx_msg void OnSelectHasPhoto();
	afx_msg void OnSelectIncremental();
	afx_msg void OnUpdateSelectOperations(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSelectIncremental(CCmdUI *pCmdUI);

	afx_msg void OnActionDelete();
	afx_msg void OnActionDeleteAll();
	afx_msg void OnActionEditPeople();
	afx_msg void OnActionRemoveDuplicated();
	afx_msg void OnActionMergeSelection();
	afx_msg void OnActionDelSelected();
	afx_msg void OnActionNameOpSelection();
	afx_msg void OnActionTelOpSelection();
	afx_msg void OnActionAutoMergeDuplicated();
	afx_msg void OnActionEditCatagory();

	afx_msg void OnUpdateActionDelete(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionEditPeople(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionRemoveDuplicated(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionMergeSelection(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionDeleteAll(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionDelSelected(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionNameOpSelection(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionTelOpSelection(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionAutoMergeDuplicated(CCmdUI *pCmdUI);
	afx_msg void OnUpdateActionEditCatagory(CCmdUI *pCmdUI);

	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};


inline CTigerContactsDoc* CTigerContactsViewMulti::GetDocument() const
   { return reinterpret_cast<CTigerContactsDoc*>(m_pDocument); }


