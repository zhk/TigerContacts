// demoView.cpp : implementation of the CPhotoEditView class
//

#include "stdafx.h"
#include "TigerContacts.h"

#include "TigerContactsDoc.h"
#include "PhotoEditView.h"
#include "memdc.h"

IMPLEMENT_DYNCREATE(PhotoEditView, CView)

BEGIN_MESSAGE_MAP(PhotoEditView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEACTIVATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



int PhotoEditView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	// TODO: Add your message handler code here and/or call default

	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void PhotoEditView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO: Add your message handler code here
}

void PhotoEditView::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class

	CWnd::PostNcDestroy();
}

void PhotoEditView::OnActivateFrame(UINT nState, CFrameWnd* pDeactivateFrame)
{
	// TODO: Add your specialized code here and/or call the base class

	CView::OnActivateFrame(nState, pDeactivateFrame);
}




void PhotoEditView::SetImage(CxImage* pImage)
{
	if (_pImg)
		delete _pImg;
	
	_pImg = pImage;

	//SetScrollSizes(MM_TEXT, CSize((int)(_pImg->GetWidth() * m_fScale),
	//	(int)(_pImg->GetHeight() * m_fScale)));

	Invalidate();
}

void PhotoEditView::DoZoom(double scale)
{
	if (!_pImg || _fScale == scale)
		return;

	_fScale = scale;

	//SetScrollSizes(MM_TEXT, CSize((int)(_pImg->GetWidth() * m_fScale),
	//	(int)(_pImg->GetHeight() * m_fScale)));

	Invalidate();
}

void PhotoEditView::SetViewFinderSize(int s)
{
	if(_iViewFinder == s)
		return;

	_iViewFinder = s;

	CRect rcClient;
	GetClientRect(rcClient);

	if( _ptViewFinder.x + s > rcClient.Width() )
		_ptViewFinder.x = rcClient.Width() - s;

	if( _ptViewFinder.y + s > rcClient.Height() )
		_ptViewFinder.y = rcClient.Height() - s;


	Invalidate();
}

bool PhotoEditView::SnapShoot(CBitmap& bmp)
{
	if (!_pImg)
		return false;

	CClientDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	
	bmp.CreateCompatibleBitmap(&dc, _iViewFinder, _iViewFinder);
	CBitmap* pOldBmp = memDC.SelectObject(&bmp);


	CDC memDC_2;
	CBitmap bmp2;
	{
		CRect rcClient;
		GetClientRect(rcClient);

		memDC_2.CreateCompatibleDC(&dc);
		bmp2.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		memDC_2.SelectObject(&bmp2);


		CSize szImage = GetImageSize();
		CRect rcDest = CRect(CPoint(0, 0), szImage);
		rcDest.OffsetRect( (rcClient.Width() - szImage.cx)/2, (rcClient.Height() - szImage.cy)/2);

		CPoint ptOffset = CaculateOffset();
		rcDest.OffsetRect(ptOffset);

		_pImg->Draw(memDC_2.GetSafeHdc(), rcDest, 0, false);
	}

	memDC.BitBlt(0, 0, _iViewFinder, _iViewFinder, &memDC_2, _ptViewFinder.x, _ptViewFinder.y, SRCCOPY);

	memDC.SelectObject(pOldBmp);

	return true;
}


CPoint PhotoEditView::CaculateOffset()
{
	CRect rcClient;
	GetClientRect(rcClient);

	CSize szDoc = GetImageSize();

	CPoint ptOffset(0, 0);

	double fRatioX = 1.0 * (_ptViewFinder.x + _iViewFinder / 2 - rcClient.Width() / 2 ) / (rcClient.Width() / 2 - _iViewFinder / 2);
	ptOffset.x = long((rcClient.Width() - szDoc.cx) / 2 * fRatioX);

	double fRatioY = 1.0 * (_ptViewFinder.y + _iViewFinder / 2 - rcClient.Height() / 2 ) / (rcClient.Height() / 2 - _iViewFinder / 2);
	ptOffset.y = long((rcClient.Height() - szDoc.cy) / 2 * fRatioY);

	if (szDoc.cx < rcClient.Width())
	{
		ptOffset.x = 0;
	}

	if (szDoc.cy < rcClient.Height())
	{
		ptOffset.y = 0;
	}

	return ptOffset;
}


PhotoEditView::PhotoEditView()
: _pImg(0)
, _fScale(1.0f)
, _iViewFinder(96)
, _bLBDown(false)
{
}

PhotoEditView::~PhotoEditView()
{
	if (_pImg)
	{
		delete _pImg;
		_pImg = 0;
	}
}

BOOL PhotoEditView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}


void PhotoEditView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	CRect rcClient;
	GetClientRect(rcClient);

	CRect rcViewFinder(0, 0, _iViewFinder, _iViewFinder);
	rcViewFinder.OffsetRect(CPoint(rcClient.Width() / 2, rcClient.Height() / 2));
	rcViewFinder.OffsetRect(-_iViewFinder/2, -_iViewFinder/2);

	_ptViewFinder = rcViewFinder.TopLeft();
}


void PhotoEditView::OnDraw(CDC* pDC)
{
	MemDC* pMemDC = NULL;
	pDC = pMemDC = new MemDC(pDC);

	CRect rcClient;
	GetClientRect(rcClient);

	if (_pImg)
	{
		CSize szImage = GetImageSize();
		CRect rcDest = CRect(CPoint(0, 0), szImage);
		rcDest.OffsetRect( (rcClient.Width() - szImage.cx)/2, (rcClient.Height() - szImage.cy)/2);

		CPoint ptOffset = CaculateOffset();
		rcDest.OffsetRect(ptOffset);

		_pImg->Draw(pDC->GetSafeHdc(), rcDest, 0, false);
	}

	{
		pDC->SelectStockObject(NULL_BRUSH);
		CPen pen(PS_SOLID, 3, RGB(0, 255, 0));
		CPen* pOldPen = pDC->SelectObject(&pen);

		pDC->Rectangle( CRect(_ptViewFinder, CSize(_iViewFinder, _iViewFinder)) );

		pDC->SelectObject(pOldPen);

	}

	delete pMemDC;
}


CSize PhotoEditView::GetImageSize()
{
	if (!_pImg)
		return CSize(0, 0);

	return CSize( int(_pImg->GetWidth() * _fScale), int(_pImg->GetHeight() * _fScale) );
}


void PhotoEditView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!_pImg)
	{
		CView::OnMouseMove(nFlags, point);
		return;
	}


	CRect rcViewFinder( _ptViewFinder, CSize(_iViewFinder, _iViewFinder) );
	BOOL bInView = rcViewFinder.PtInRect(point);

	if ( _bLBDown && bInView && (nFlags & MK_LBUTTON) )
	{
		SetCursor(LoadCursor(0,IDC_SIZEALL));

		CRect rcClient;
		GetClientRect(rcClient);

		_ptViewFinder += point - _RefPoint;

		if (_ptViewFinder.x < 0)
			_ptViewFinder.x = 0;
		else if (_ptViewFinder.x + _iViewFinder > rcClient.Width())
			_ptViewFinder.x = rcClient.Width() - _iViewFinder;

		if (_ptViewFinder.y < 0)
			_ptViewFinder.y = 0;
		else if (_ptViewFinder.y + _iViewFinder > rcClient.Height())
			_ptViewFinder.y = rcClient.Height() - _iViewFinder;

		_RefPoint = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL PhotoEditView::OnEraseBkgnd(CDC* /*pDC*/) 
{
	return 1;
}

void PhotoEditView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rcViewFinder( _ptViewFinder, CSize(_iViewFinder, _iViewFinder) );
	BOOL bInView = rcViewFinder.PtInRect(point);

	if ( bInView )
	{
		_RefPoint = point;
		_bLBDown = true;
		
		SetCapture();
	}

	CView::OnLButtonDown(nFlags, point);
}

void PhotoEditView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (_bLBDown)
	{
		ReleaseCapture();
		_bLBDown = false;
	}

	CView::OnLButtonUp(nFlags, point);
}
