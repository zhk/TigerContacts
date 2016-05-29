// PhotoEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TigerContacts.h"
#include "PhotoEditDlg.h"
#include "PhotoEditView.h"


// PhotoEditDlg dialog

IMPLEMENT_DYNAMIC(PhotoEditDlg, CDialog)


#define M_CMD_Separator		0


static CString g_s_tbb_imge_files[] =
{
	_T("data/Photo_Update.bmp"),			// 0
	_T("data/file_open.bmp"),				// 1
	//_T("data/Photo_ZoomIn.bmp"),			// 2
	//_T("data/Photo_ZoomOut.bmp"),			// 3
	//_T("data/Photo_ZoomNone.bmp"),			// 4
};

static TBBUTTON g_s_tbb[] =
{
	{  1,  ID_MENU_PHOTO_OPENFILE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	{  0,  ID_MENU_PHOTO_UPDATE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	//{ -1,  M_CMD_Separator,				TBSTATE_ENABLED, TBSTYLE_SEP,		0, 0 },
	//{  2,  ID_MENU_PHOTO_ZOOM_IN,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	//{  3,  ID_MENU_PHOTO_ZOOM_OUT,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 },
	//{  4,  ID_MENU_PHOTO_ZOOM_NONE,		TBSTATE_ENABLED, TBSTYLE_BUTTON,	0, 0 }
};


PhotoEditDlg::PhotoEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(PhotoEditDlg::IDD, pParent)
	, _iSlider(50)
	, _iViewFinder(3)
{

}

PhotoEditDlg::~PhotoEditDlg()
{
}

void PhotoEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Slider(pDX, IDC_SLIDER_ZOOM, _iSlider);
	DDX_Control(pDX, IDC_SLIDER_ZOOM, _sliderCtrl);

	DDX_Slider(pDX, IDC_SLIDER_VIEW_FINDER, _iViewFinder);
	DDX_Control(pDX, IDC_SLIDER_VIEW_FINDER, _sliderViewFinder);
}


BEGIN_MESSAGE_MAP(PhotoEditDlg, CDialog)
	ON_COMMAND(ID_MENU_PHOTO_OPENFILE, &PhotoEditDlg::OnMenuPhotoOpen)
	ON_COMMAND(ID_MENU_PHOTO_UPDATE, &PhotoEditDlg::OnMenuPhotoUpdate)
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// PhotoEditDlg message handlers

BOOL PhotoEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_sliderCtrl.SetRange(0, 100);
	_sliderCtrl.SetTicFreq(10);

	_sliderViewFinder.SetRange(0, 5);
	_sliderViewFinder.SetTicFreq(1);

	// 工具条
	if( !m_wndToolBar.CreateEx(this, TBSTYLE_TOOLTIPS, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS) )
		return -1;

	// Load image list
	m_imgListToolBar.Create(32, 32, ILC_COLOR32|ILC_MASK, 0, 0);

	int	iButtons = sizeof(g_s_tbb_imge_files) / sizeof(g_s_tbb_imge_files[0]);
	for(int i = 0; i < iButtons; ++i)
	{
		HBITMAP hBitmap = (HBITMAP)LoadImage(AfxGetResourceHandle(), g_s_tbb_imge_files[i], IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_LOADFROMFILE);
		CBitmap bmp;
		bmp.Attach(hBitmap);
		m_imgListToolBar.Add(&bmp, RGB(0, 0, 0));
		bmp.DeleteObject();
	}

	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_imgListToolBar);
	m_wndToolBar.SetSizes(CSize(32 + 10, 32 + 6), CSize(32, 32));

	iButtons = sizeof(g_s_tbb) / sizeof(g_s_tbb[0]);
	for(int i = 0; i < iButtons; ++i)
	{
		m_wndToolBar.GetToolBarCtrl().AddButtons(1, &g_s_tbb[i]);
	}
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	CRect rcToolBar;
	m_wndToolBar.GetWindowRect(rcToolBar);
	ScreenToClient(rcToolBar);

	// ScrollView
	CWnd* pWnd = this->GetDlgItem(IDC_STATIC_SCROLLVIEW);
	CRect RectTargetCtrl;
	pWnd->GetWindowRect(RectTargetCtrl);
	pWnd->DestroyWindow();
	this->ScreenToClient(RectTargetCtrl);

	RectTargetCtrl.top = rcToolBar.bottom + 8;

	//在目标位置动态创建CScrollView
	_pPhotoView = (PhotoEditView*)RUNTIME_CLASS(PhotoEditView)->CreateObject();
	_pPhotoView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, RectTargetCtrl, this, IDC_STATIC_SCROLLVIEW);
	_pPhotoView->OnInitialUpdate();
	_pPhotoView->ShowWindow(SW_SHOW);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void PhotoEditDlg::OnMenuPhotoOpen()
{
	CxImage* pImage = 0;

	{
		CFileDialog dlg(TRUE);
		if( dlg.DoModal() == IDOK)
		{
			pImage = new CxImage;
			if( !pImage->Load(dlg.GetPathName()) )
			{
				MessageBox(_("不支持的图像格式！"), _("错误"), MB_OK | MB_ICONHAND);
				delete pImage;
				return;
			}
		}
	}

	if (pImage)
	{
		//_strResName = dlg.GetPathName();
		_pPhotoView->SetImage(pImage);
		_pPhotoView->Invalidate();
	}

}

void PhotoEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int iZoomOld = _iSlider;
	int iViewOld = _iViewFinder;

	UpdateData(TRUE);

	if (iZoomOld != _iSlider)
	{
		_iSlider = 100 - _iSlider;

		double scale = 1.0;
		if (_iSlider == 50)
		{
		}
		else if (_iSlider > 50)
		{
			scale = 1.0 + 7.0 * (_iSlider - 50) / 50;
		}
		else
		{
			scale = (1.0 - 1.0/8) * _iSlider / 50 + 1.0/8;
		}

		_pPhotoView->DoZoom(scale);
	}
	
	if (iViewOld != _iViewFinder)
	{
		_iViewFinder = 5 - _iViewFinder;

		static int s_sizes[] = {60, 72, 96, 128, 192, 256};

		_pPhotoView->SetViewFinderSize(s_sizes[_iViewFinder]);
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")

void PhotoEditDlg::OnMenuPhotoUpdate()
{
	PlaySound(MAKEINTRESOURCE(IDR_WAVE_CAMERA), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT/* | SND_LOOP*/);

	CBitmap bmp;
	if ( _pPhotoView->SnapShoot(bmp) )
	{
		if( _img.CreateFromHBITMAP(bmp) )
		{
			//_img.Save(_T("f:\\a.jpg"), CXIMAGE_FORMAT_JPG);

			long size = 0;
			BYTE* pBuffer = 0;

			_img.Encode(pBuffer, size, CXIMAGE_FORMAT_JPG);

			_strPhotoBytes = Cactus::base64_encode(pBuffer, size);

			OnOK();
		}
	}
}