
// TigerContactsView.h : CTigerContactsView 类的接口
//


#pragma once

#include "vCardEditor.h"

class CTigerContactsViewSingle : public CScrollView
{
protected:
	CTigerContactsViewSingle();
	DECLARE_DYNCREATE(CTigerContactsViewSingle)
	DECLARE_MESSAGE_MAP()

	virtual void OnInitialUpdate();


	NamePropGroups_ListType	_Groups;
	vCard*					_pCard;

	void	PropGroupIncrease(PropGroup* pGroup, int iH);
	void	PropGroupDescrease(PropGroup* pGroup, int iIndex = -1);
	void	UpdateScrollRange();

public:
	CTigerContactsDoc* GetDocument() const;
	virtual ~CTigerContactsViewSingle();

	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void		SetvCard(vCard* pCard);

public:
	afx_msg void OnBrowseFirst();
	afx_msg void OnBrowsePrev();
	afx_msg void OnBrowseNext();
	afx_msg void OnBrowseLast();
	afx_msg void OnSingleAddNickname();
	afx_msg void OnSingleAddAddress();
	afx_msg void OnSingleAddCompany();
	afx_msg void OnSingleAddEmail();
	afx_msg void OnSingleAddEvent();
	afx_msg void OnSingleAddIm();
	afx_msg void OnSingleAddNote();
	afx_msg void OnSingleAddPhone();
	afx_msg void OnSingleAddRelats();
	afx_msg void OnSingleAddWebsite();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCommandRange_ButtonClicked(UINT nID);
	afx_msg void OnCommandRange_EditUpdated(UINT nID);
	afx_msg void OnCommandRange_ComboSelected(UINT nID);
	afx_msg void OnCommandRange_ComboEditUpdated(UINT nID);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


inline CTigerContactsDoc* CTigerContactsViewSingle::GetDocument() const
   { return reinterpret_cast<CTigerContactsDoc*>(m_pDocument); }


