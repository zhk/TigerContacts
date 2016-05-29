// demoView.h : interface of the CDemoView class

class PhotoEditView : public CView
{
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void	SetImage(CxImage* pImage);
	void	DoZoom(double scale);
	void	SetViewFinderSize(int s);
	bool	SnapShoot(CBitmap& bmp);

protected: // create from serialization only
	PhotoEditView();
	virtual ~PhotoEditView();
	DECLARE_DYNCREATE(PhotoEditView)


	CSize	GetImageSize();

	//{{AFX_MSG(CDemoView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CPoint			_RefPoint;
	CxImage*		_pImg;
	double			_fScale;

	int				_iViewFinder;
	CPoint			_ptViewFinder;
	bool			_bLBDown;

	CPoint			CaculateOffset();

	virtual void PostNcDestroy();
	virtual void OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame);

public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

