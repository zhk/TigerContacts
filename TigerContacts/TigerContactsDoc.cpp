
// TigerContactsDoc.cpp : CTigerContactsDoc 类的实现
//

#include "stdafx.h"
#include "TigerContacts.h"

#include "TigerContactsDoc.h"
#include "VCFImportDlg.h"
#include "VCFExportDlg.h"
#include "FolderDlg.h"

#include "TigerContactsViewMulti.h"
#include "TigerContactsViewSingle.h"
#include "MainFrm.h"

using namespace Cactus;

// CTigerContactsDoc

IMPLEMENT_DYNCREATE(CTigerContactsDoc, CDocument)

BEGIN_MESSAGE_MAP(CTigerContactsDoc, CDocument)
	ON_COMMAND(ID_MENU_FILE_IMPORT_SINGLE_VCF, &CTigerContactsDoc::OnMenuFileImportSingleVcf)
	ON_COMMAND(ID_MENU_FILE_IMPORT_MULTI_VCF, &CTigerContactsDoc::OnMenuFileImportMultiVcf)
	ON_COMMAND(ID_MENU_FILE_EXPORT_MULTI_VCF, &CTigerContactsDoc::OnMenuFileExportMultiVcf)
	ON_COMMAND(ID_MENU_FILE_EXPORT_SINGLE_VCF, &CTigerContactsDoc::OnMenuFileExportSingleVcf)
	ON_UPDATE_COMMAND_UI(ID_MENU_FILE_EXPORT_MULTI_VCF, &CTigerContactsDoc::OnUpdateMenuFileExportMultiVcf)
	ON_UPDATE_COMMAND_UI(ID_MENU_FILE_EXPORT_SINGLE_VCF, &CTigerContactsDoc::OnUpdateMenuFileExportSingleVcf)
	ON_COMMAND(ID_ACTION_ADD_NEW, &CTigerContactsDoc::OnActionAddNew)
END_MESSAGE_MAP()


// CTigerContactsDoc 构造/析构

CTigerContactsDoc::CTigerContactsDoc()
{
	_itCur = _vcardList.end();
}

CTigerContactsDoc::~CTigerContactsDoc()
{
}

BOOL CTigerContactsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	_vcardList.clear();
	_itCur = _vcardList.end();

	_imageList.DeleteImageList();
	_imageList.Create(96, 96, ILC_COLOR32, 0, 4);
	
	_imageListSmall.DeleteImageList();
	_imageListSmall.Create(16, 16, ILC_COLOR32, 0, 4);

	// Load default head icon
	CBitmap bmp;
	bmp.LoadBitmap(IDB_BITMAP_CONTACT);
	_imageList.Add(&bmp, RGB(0, 0, 0));
	bmp.DeleteObject();

	bmp.LoadBitmap(IDB_BITMAP_CONTACT_SMALL);
	_imageListSmall.Add(&bmp, RGB(0, 0, 0));
	bmp.DeleteObject();

	return TRUE;
}

BOOL CTigerContactsDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	GString gStr((const wchar_t*)lpszPathName);
	_vcardList = vCard::fromFile( gStr.ToMbcs() );

	_itCur = _vcardList.end();

	// Build image list
	size_t sz = _vcardList.size();
	if (sz > 0)
	{
		BuildImageList(_vcardList);
	}

	if (AfxGetMainWnd())
	{
		((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());
	}

	return sz != 0;
}

BOOL CTigerContactsDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	GString gStrCache;
	int iCount = 0;
	M_Foreach_Const(vCard, card, vCardList, _vcardList)
	{
		if (iCount != 0)
			gStrCache += VC_END_LINE_TOKEN;
		
		gStrCache += card.toGString();

		iCount++;
	}

	GString gStrFilePathName(lpszPathName);

	IFileSystem* pFS = FileManager::GetInstance().GetDefaultFileSystem();
	IDataOutStreamPtr spOutStream = pFS->OpenFileWrite(gStrFilePathName.ToMbcs());
	if (!spOutStream)
	{
		MessageBox(AfxGetMainWnd()->m_hWnd, _("文件不能写！"), _("错误"), MB_ICONHAND | MB_OK);
		return FALSE;
	}

	String strOut = gStrCache.ToUtf8();
	spOutStream->Write((void*)strOut.c_str(), strOut.length());

	SetModifiedFlag(FALSE);

	return TRUE;

	//return CDocument::OnSaveDocument(lpszPathName);
}


bool CTigerContactsDoc::DeleteOnevCard( vCard* pCard )
{
	//CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();

	for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
	{
		vCard& card = *it;
		if (&card == pCard)
		{
			if (_itCur == it)
			{
				_itCur = _vcardList.erase(it);

				// Notify PageView
				CTigerContactsViewSingle* pViewPage = (CTigerContactsViewSingle*)((CTigerContactsApp*)AfxGetApp())->GetPageView();
				if (_itCur != _vcardList.end())
					pViewPage->SetvCard( &(*_itCur) );
				else
					pViewPage->SetvCard(0);
			}
			else
				_vcardList.erase(it);

			SetModifiedFlag(TRUE);

			((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());

			return true;
		}
	}

	return false;
}

bool CTigerContactsDoc::DeleteAllvCards()
{
	_vcardList.clear();
	_itCur = _vcardList.end();

	SetModifiedFlag(TRUE);

	((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());

	return true;
}

int CTigerContactsDoc::RemoveDuplicated()
{
	int iRemovedCount = 0;

	for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
		it->ClearMask(0, true);

	for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
	{
		if (it->HasMask(0x1))	//already masked to be removed
			continue;

		vCard& card = *it;
		vCardList::iterator it2 = it;
		it2++;
		while ( it2 != _vcardList.end() )
		{
			if (it2->HasMask(0x1))	//already masked to be removed
			{
				++it2;
				continue;
			}

			vCard& card2 = *it2;
			if (card == card2)
			{
				iRemovedCount++;
				it2->AddMask(0x1);
				it2 = _vcardList.erase(it2);
				continue;
			}

			++it2;
		}

		MSG msg = { 0 };
		while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	if (iRemovedCount != 0)
	{
		SetModifiedFlag( TRUE );
		_itCur = _vcardList.begin();
		((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());
	}


	return iRemovedCount;
}

int CTigerContactsDoc::MergeDuplicated()
{
	int iCount = 0;

	for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
		it->ClearMask(0, true);

	for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
	{
		if (it->HasMask(0x1))	//already masked to be removed
			continue;

		vCard& card = *it;
		vCardList::iterator it2 = it;
		it2++;
		while ( it2 != _vcardList.end() )
		{
			if (it2->HasMask(0x1))	//already masked to be removed
			{
				++it2;
				continue;
			}

			vCard& card2 = *it2;
			if (card.getValid_FN_N_Name().Str().length() > 0
				&&
				card.getValid_FN_N_Name() == card2.getValid_FN_N_Name())
			{
				card.Merge(card2);
				card.updateReversion();

				iCount++;
				it2->AddMask(0x1);

				it2 = _vcardList.erase(it2);
				continue;
			}

			++it2;
		}

		MSG msg = { 0 };
		while(::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	if (iCount != 0)
	{
		SetModifiedFlag( TRUE );
		_itCur = _vcardList.begin();
		((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());
	}

	return iCount;
}


void CTigerContactsDoc::MergevCard( vCard* pCardUpdated, Cactus::list<vCard*>::type& removed )
{
	bool bUpdatePageView = false;
	CTigerContactsViewSingle* pViewPage = (CTigerContactsViewSingle*)((CTigerContactsApp*)AfxGetApp())->GetPageView();
	if (&(*_itCur) == pCardUpdated)
	{
		pViewPage->SetvCard(0);
		bUpdatePageView = true;
	}

	bool bModified = false;
	M_Foreach(vCard*, pRe, Cactus::list<vCard*>::type, removed)
	{
		pCardUpdated->Merge(*pRe);

		for (vCardList::iterator it = _vcardList.begin(); it != _vcardList.end(); ++it)
		{
			vCard& card = *it;
			if (&card == pRe)
			{
				if (_itCur == it)
				{
					_itCur = _vcardList.erase(it);

					if (_itCur != _vcardList.end())
						pViewPage->SetvCard( &(*_itCur) );
					else
						pViewPage->SetvCard(0);
				}
				else
					_vcardList.erase(it);

				bModified = true;

				break;
			}
		}
	}

	if(bModified)
	{
		pCardUpdated->updateReversion();

		SetModifiedFlag(TRUE);
		((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());
	}

	if (bUpdatePageView)
	{
		pViewPage->SetvCard( &(*_itCur) );
	}
}

void CTigerContactsDoc::BuildImageList(vCardList& vcardList)
{
	M_Foreach(vCard, card, vCardList, vcardList)
	{
		vCardProperty propPhoto = card.getProperty(VC_PHOTO);
		if (!propPhoto.isValid())
		{
			card.SetImageIndex(0);
			continue;
		}

		Log_Debug("CTigerContactsDoc::BuildImageList: loading PHOTO for " << card.getValid_FN_N_Name().ToUtf8());

		//PHOTO;VALUE=URL;TYPE=GIF:http://www.abc.com/dir_photos/my_photo.gif

		//PHOTO;ENCODING=BASE64;TYPE=GIF:R0lGODdhfgA4AOYAAAAAAK+vr62trVIxa6WlpZ+fnzEpCEpzlAha/0Kc74+PjyGM
		//	SuecKRhrtX9/fzExORBSjCEYCGtra2NjYyF7nDGE50JrhAg51qWtOTl7vee1MWu1
		//	50o5e3PO/3sxcwAx/4R7GBgQOcDAwFoAQt61hJyMGHuUSpRKIf8A/wAY54yMjHtz

		//PHOTO;GIF;MIME:<<JOHNSMITH.part3.960129T083020.xyzMail@host3.com>

		//PHOTO;ENCODING=B;TYPE=JPEG:/9j/4AAQSkZJRgABAQAAAQABAAD/2wCEAAUDBAgNCwgMCAs
		//	LCwsLBwgMBggICwsLBwcIBggIBwgKCAgIChAMBwkOCQcHDiENDhERHx8UCA0iJBceJBAeExIB
		//	BQUFCAcIDwgJDRQPDw0VFBIUFBUUFBQUFBUSFR4UFBQUFRQWFBUUFBQWFBQUFBQeFRQUFBQUF

		GString gStrEncoding;
		GString gstrType;

		vCardParamList params = propPhoto.params();
		M_Foreach_Const(vCardParam, param, vCardParamList, params)
		{
			if (param.group() == vCardParam::Encoding)
				gStrEncoding = param.value();
			else if (param.group() == vCardParam::Type)
				gstrType = param.value();
			else if (param.group() == vCardParam::Undefined)	// for support VC_VER_2_1
				gstrType = param.value();
		}

		gStrEncoding.ToUpper();
		gstrType.ToUpper();

		if ( gStrEncoding != _T("B") && gStrEncoding != _T("BASE64") )
			continue;

		int iFormat = -1;
		if (gstrType == _T("JPEG"))
		{
			iFormat = CXIMAGE_FORMAT_JPG;
		}
		else if (gstrType == _T("GIF"))
		{
			iFormat = CXIMAGE_FORMAT_GIF;
		}
		else if (gstrType == _T("PNG"))
		{
			iFormat = CXIMAGE_FORMAT_PNG;
		}
		else if (gstrType == _T("BMP"))
		{
			iFormat = CXIMAGE_FORMAT_BMP;
		}
		else if (gstrType == _T("GIF"))
		{
			iFormat = CXIMAGE_FORMAT_GIF;
		}
		else if (gstrType == _T("PCX"))
		{
			iFormat = CXIMAGE_FORMAT_PCX;
		}
		else if (gstrType == _T("TIFF"))
		{
			iFormat = CXIMAGE_FORMAT_TIF;
		}
		else if (gstrType == _T("TGA"))
		{
			iFormat = CXIMAGE_FORMAT_TGA;
		}

		if (iFormat == -1)
		{
			Log_Error("CTigerContactsDoc::BuildImageList: image format is unknown " << gstrType.ToUtf8());
			continue;
		}

		Cactus::String strData = base64_decode(	propPhoto.values().at(0).ToMbcs() );
		CxImage image((BYTE *)strData.c_str(), strData.size(), iFormat);

		CxImage imgBig;
		if (!image.Resample(96, 96, CxImage::IM_BILINEAR, &imgBig))
			Log_Error("CTigerContactsDoc::BuildImageList: create big image failed!");

		CxImage imgSmall;
		if (!image.Resample(16, 16, CxImage::IM_BILINEAR, &imgSmall))
			Log_Error("CTigerContactsDoc::BuildImageList: create small image failed!");


		HBITMAP hBitmap = imgBig.MakeBitmap();
		CBitmap bmp;
		bmp.Attach(hBitmap);
		int iImgIndex = _imageList.Add(&bmp, RGB(0, 0, 0));
		if( iImgIndex == -1)
		{
			Log_Error("CTigerContactsDoc::BuildImageList: add big image to CImageList failed! " << ::GetLastError() );
		}
		bmp.DeleteObject();

		card.SetImageIndex(iImgIndex);

		//GString gStrTmpFileName = "d:\\";
		//gStrTmpFileName += card.getFName() + ".jpg";
		//image.Save(gStrTmpFileName.Str().c_str(), CXIMAGE_FORMAT_JPG);


		hBitmap = imgSmall.MakeBitmap();
		bmp.Attach(hBitmap);
		iImgIndex = _imageListSmall.Add(&bmp, RGB(0, 0, 0));
		if( iImgIndex == -1)
		{
			Log_Error("CTigerContactsDoc::BuildImageList: add small image to CImageList failed! " << ::GetLastError() );
		}
		bmp.DeleteObject();
	}
}

void CTigerContactsDoc::AddVCFFile( LPCTSTR lpszPathName, EvCardUsage usage/* = VC_Usage_Android_Apple*/ )
{
	GString gStr((const wchar_t*)lpszPathName);
	vCardList vcardList = vCard::fromFile( gStr.ToMbcs(), usage );

	// Build image list
	size_t sz = vcardList.size();
	if (sz > 0)
	{
		BuildImageList(vcardList);

		size_t szOld = _vcardList.size();
		_vcardList.insert(_vcardList.end(), vcardList.begin(), vcardList.end());

		CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
		pViewMulti->OnAddvCardList(_vcardList, szOld);

		SetModifiedFlag(TRUE);
		((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());
	}
}

void CTigerContactsDoc::OnMenuFileImportSingleVcf()
{
	CVCFImportDlg dlg;
	dlg._bSingleVCF = true;
	if( dlg.DoModal() == IDOK )
	{
		AddVCFFile( (LPCTSTR)dlg._strFilePathName, EvCardUsage(dlg._iFileOrigin) );
	}
}

void CTigerContactsDoc::OnMenuFileImportMultiVcf()
{
	CVCFImportDlg dlg;
	dlg._bSingleVCF = false;
	if( dlg.DoModal() == IDOK )
	{
		GString gStr((LPCTSTR)dlg._strFilePathName);

		StringVector outs;
		if( FileManager::GetInstance().GetDefaultFileSystem()->FindFiles("*.vcf", outs, gStr.ToMbcs()) )
		{
			M_Foreach_Const(String, str, StringVector, outs)
			{
				GString gStrFile(str);
				AddVCFFile( gStrFile.Str().c_str(), EvCardUsage(dlg._iFileOrigin) );
			}
		}
	}
}

BOOL OpenFolderAndSelectFile( LPSTR lpszFilePath)
{
	//
	// GetFolder
	//
	DWORD dw = lstrlenA(lpszFilePath) - 1;
	for (;dw != -1;dw--)
	{
		if ( lpszFilePath[dw] == '\\')
		{
			break;
		}
	}
	if ( dw == -1)
	{
		return FALSE;
	}
	//
	// Get a pointer to the Desktop's IShellFolder interface.
	// 
	LPSHELLFOLDER pDesktopFolder;
	if ( SUCCEEDED(SHGetDesktopFolder( &pDesktopFolder)))
	{
		// 
		// IShellFolder::ParseDisplayName requires the file name be in
		// Unicode.
		// 
		OLECHAR oleStr[MAX_PATH];

		lpszFilePath[dw] = '\0';
		MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED,
			lpszFilePath, -1, oleStr, _countof(oleStr));
		// 
		// Convert the path to an ITEMIDLIST.
		// 
		LPITEMIDLIST     pidl;
		ULONG             chEaten;
		ULONG             dwAttributes;
		HRESULT             hr;

		hr = pDesktopFolder->ParseDisplayName(
			NULL, NULL, oleStr, &chEaten, &pidl, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return FALSE;
		}
		LPCITEMIDLIST pidlFolder = pidl;

		lpszFilePath[dw] = '\\';
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED,
			lpszFilePath, -1, oleStr, _countof(oleStr));

		hr = pDesktopFolder->ParseDisplayName(
			NULL, NULL, oleStr, &chEaten, &pidl, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return FALSE;
		}
		LPCITEMIDLIST pidlFile = pidl;

		CoInitialize( NULL);
		hr = SHOpenFolderAndSelectItems( pidlFolder, 1, &pidlFile, 0);

		pDesktopFolder->Release();

		if ( hr == S_OK)
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CTigerContactsDoc::OnMenuFileExportSingleVcf()
{
	CVCFExportDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		CString strExt;
		switch (dlg._iFileUsage)
		{
		default:
		case 0:
			strExt = _T("_android_apple");
			break;
		case 1:
			strExt = _T("_s60");
			break;
		case 2:
			strExt = _T("_outlook");
			break;
		}

		CFileDialog dlgFileDlg(FALSE, _T("vcf"), GetTitle()
			, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("vcf Files (*.vcf)|*.vcf||") );

		if( dlgFileDlg.DoModal() == IDOK)
		{
			CString strPath = dlgFileDlg.GetFolderPath();
			strPath += _T("\\");
			
			CString strNewName = strPath + dlgFileDlg.GetFileTitle() + strExt + _T(".") + dlgFileDlg.GetFileExt();
			GString gStrFinalFilePathName( (LPCTSTR)strNewName );

			IFileSystem* pFS = FileManager::GetInstance().GetDefaultFileSystem();
			IDataOutStreamPtr spOutStream = pFS->OpenFileWrite(gStrFinalFilePathName.ToMbcs());
			if (!spOutStream)
			{
				MessageBox(AfxGetMainWnd()->m_hWnd, _("文件不能写！"), _("错误"), MB_ICONHAND | MB_OK);
				return;
			}

			GString gStrCache;
			int iCount = 0;
			M_Foreach_Const(vCard, card, vCardList, _vcardList)
			{
				if (iCount != 0)
					gStrCache += VC_END_LINE_TOKEN;

				gStrCache += card.toGString(VC_VER_3_0, EvCardUsage(dlg._iFileUsage));

				iCount++;
			}

			String strOut = (dlg._iFileUsage == 2) ? gStrCache.ToMbcs() : gStrCache.ToUtf8();

			spOutStream->Write((void*)strOut.c_str(), strOut.length());

			{
				strNewName += _(" 文件导出成功！");

				MessageBox(AfxGetMainWnd()->m_hWnd, strNewName, _("提示"), MB_ICONINFORMATION | MB_OK);

				//::ShellExecute(0, _("open"), strPath, 0, 0, SW_SHOW);
				GString gStr((LPCTSTR)strPath);
				OpenFolderAndSelectFile((LPSTR)gStr.ToMbcs());
			}
		}

	}
}

void CTigerContactsDoc::OnMenuFileExportMultiVcf()
{
	CVCFExportDlg dlg;
	if( dlg.DoModal() == IDOK )
	{
		GString strExt;
		switch (dlg._iFileUsage)
		{
		default:
		case 0:
			strExt = _T("_android_apple");
			break;
		case 1:
			strExt = _T("_s60");
			break;
		case 2:
			strExt = _T("_outlook");
			break;
		}


		GString gStr(FileManager::GetInstance().GetModuleDir());
		CString strDataDir = gStr.Str().c_str();
		CFolderDialog dlgFolder(_("请选择导出的目录："), strDataDir);
		if( dlgFolder.DoModal() == IDOK )
		{
			CString strPath = dlgFolder.GetFolderPath();
			if (strPath.GetLength() != 0)
			{
				strPath += "\\";
			}

			GString gStrRoot((LPCTSTR)strPath);

			M_Foreach_Const(vCard, card, vCardList, _vcardList)
			{
				GString gStrFNName = card.getValid_FN_N_Name();
				if (gStrFNName.Str().length() == 0)
					continue;

				GString gStrPathName = gStrRoot + gStrFNName.Str() + strExt + _T(".vcf");

				IFileSystem* pFS = FileManager::GetInstance().GetDefaultFileSystem();
				IDataOutStreamPtr spOutStream = pFS->OpenFileWrite(gStrPathName.ToMbcs());
				if (!spOutStream)
				{
					MessageBox(AfxGetMainWnd()->m_hWnd, _("文件不能写！"), _("错误"), MB_ICONHAND | MB_OK);
					continue;
				}

				GString gStrCache = card.toGString(VC_VER_3_0, EvCardUsage(dlg._iFileUsage));
				String strOut = (dlg._iFileUsage == 2) ? gStrCache.ToMbcs() : gStrCache.ToUtf8();

				spOutStream->Write((void*)strOut.c_str(), strOut.length());
			}

			MessageBox(AfxGetMainWnd()->m_hWnd, _("导出到单个VCF文件完成！"), _("提示"), MB_ICONINFORMATION | MB_OK);

			::ShellExecute(0, _T("open"), strPath, 0, 0, SW_SHOW);
		}
	}
}

void CTigerContactsDoc::OnUpdateMenuFileExportMultiVcf(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(_vcardList.size() != 0);
}

void CTigerContactsDoc::OnUpdateMenuFileExportSingleVcf(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(_vcardList.size() != 0);
}

void CTigerContactsDoc::OnActionAddNew()
{
	vCard card;
	card.normalize();

	_vcardList.push_back(card);
	SetCurrentvCard(&_vcardList.back());

	CTigerContactsViewMulti* pViewMulti = (CTigerContactsViewMulti*)((CTigerContactsApp*)AfxGetApp())->GetMultiView();
	pViewMulti->OnAddvCardList(_vcardList, _vcardList.size() - 1);

	SetModifiedFlag(TRUE);
	((CMainFrame*)AfxGetMainWnd())->SetvCardCount(_vcardList.size());

	((CTigerContactsApp*)AfxGetApp())->OnViewPage();
}

vCard* CTigerContactsDoc::GetFirstvCard()
{
	if(_vcardList.size() == 0)
		return 0;

	_itCur = _vcardList.begin();

	return &(*_itCur);
}

vCard* CTigerContactsDoc::GetPrevvCard()
{
	if(_vcardList.size() == 0)
		return 0;

	if (_itCur != _vcardList.begin())
		_itCur--;

	return &(*_itCur);
}

vCard* CTigerContactsDoc::GetNextvCard()
{
	if(_vcardList.size() == 0)
		return 0;

	if (_itCur != _vcardList.end() )
		_itCur++;

	if (_itCur == _vcardList.end())
	{
		_itCur--;
		return 0;
	}

	return &(*_itCur);
}

vCard* CTigerContactsDoc::GetLastvCard()
{
	if(_vcardList.size() == 0)
		return 0;

	_itCur = _vcardList.end();
	_itCur--;

	return &(*_itCur);
}

void CTigerContactsDoc::SetCurrentvCard(vCard* pCard)
{
	if (pCard == 0)
	{
		CTigerContactsViewSingle* pViewPage = (CTigerContactsViewSingle*)((CTigerContactsApp*)AfxGetApp())->GetPageView();
		pViewPage->SetvCard( 0 );

		_itCur = _vcardList.end();

		return;
	}

	vCardList::iterator itBak = _itCur;

	for (_itCur = _vcardList.begin(); _itCur != _vcardList.end(); ++_itCur)
	{
		if ( &(*_itCur) == pCard)
		{
			if (itBak != _itCur)
			{
				CTigerContactsViewSingle* pViewPage = (CTigerContactsViewSingle*)((CTigerContactsApp*)AfxGetApp())->GetPageView();
				pViewPage->SetvCard( pCard );
			}

			return;
		}
	}
}
