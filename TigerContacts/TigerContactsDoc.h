
// TigerContactsDoc.h : CTigerContactsDoc 类的接口
//

#pragma once

class CTigerContactsDoc : public CDocument
{
protected: // 仅从序列化创建
	CTigerContactsDoc();
	DECLARE_DYNCREATE(CTigerContactsDoc)

	vCardList	_vcardList;
	CImageList	_imageList;
	CImageList	_imageListSmall;
	vCardList::iterator	_itCur;

	void		BuildImageList(vCardList& vcardList);
	void		AddVCFFile(LPCTSTR lpszPathName, EvCardUsage usage = VC_Usage_Android_Apple);

	bool		DeleteOnevCard(vCard* pCard);
	bool		DeleteAllvCards();

	int			RemoveDuplicated();
	void		MergevCard(vCard* pCardUpdated, Cactus::list<vCard*>::type& removed);

	int			MergeDuplicated();

public:
	virtual ~CTigerContactsDoc();

	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	vCardList&	GetvCardList() { return _vcardList; }
	CImageList& GetImageList() { return _imageList; }
	CImageList& GetImageListSmall() { return _imageListSmall; }

	vCard*		GetFirstvCard();
	vCard*		GetPrevvCard();
	vCard*		GetNextvCard();
	vCard*		GetLastvCard();
	void		SetCurrentvCard(vCard* pCard);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMenuFileImportSingleVcf();
	afx_msg void OnMenuFileImportMultiVcf();
	afx_msg void OnMenuFileExportMultiVcf();
	afx_msg void OnMenuFileExportSingleVcf();
	afx_msg void OnUpdateMenuFileExportMultiVcf(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMenuFileExportSingleVcf(CCmdUI *pCmdUI);
	afx_msg void OnActionAddNew();
};


