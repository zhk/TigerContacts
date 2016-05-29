/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			PkgFile.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/

#pragma once

#include "FileSystemBase.h"
#include "StringHelper.h"

namespace Cactus
{
	struct COMMONUTIL_API SPkgFileHeader : public Object
	{
		SPkgFileHeader();

		char				_strMagicKey[8];
		char				_strCodeVersion[8];
		const static char*	s_strMagicKey;
		const static char*	s_strCodeVersion;
	};

	struct COMMONUTIL_API SFATItem : public Object
	{
		SFATItem()
			: _strName("")
			, _strPath("")
			, _szOriginal(0)
			, _szCompressed(0)
			, _szOffset(0)
			, _szPosition(0)
		{
		}

		Cactus::String		_strName;
		Cactus::String		_strPath;
		size_t		_szOriginal;
		size_t		_szCompressed;
		size_t		_szOffset;
		size_t		_szPosition;
	};

	/**
	A facility class for PkgFile format file.
	*/ 
	class COMMONUTIL_API PkgFile : public RefObject
	{
		ImplementIObject
	public:
		PkgFile();
		~PkgFile();

		/**
		There will be a int saved in PkgFile for version 1.01 or upper.
		*/
		int				GetCreationTime();

		/**
		Load the whole package file.
		*/ 
		bool			Load(const Cactus::String& strPathName);
		/**
		Close the package file handle.
		*/ 
		void			Unload();

		/**
		Pack a disk directory.
		@param strPath	Directory path name to pack, like      D:/RealProject/IgglePop/Data/.
		@param strRemoveRoot	Root directory to be removed for the pack file, like D:/RealProject/.
		File item saved in the pack file will be like IgglePop/Data/config.xml...
		@param IgnoredPaths	Ignored SubDirectory path name.
		@param IgnoredFiles	Ignored file path name.
		*/ 
		bool			Pack(const Cactus::String& strPath
						, const Cactus::String& strRemoveRoot
						, const StringVector& IgnoredPaths
						, const StringVector& IgnoredFiles);

		/**
		Save packed info and data to a package file. Notice : will in the highest version!
		*/ 
		bool			Save(const Cactus::String& strPathName);


		enum EPkgExtract
		{
			eExtractOK,
			eExtractPartialOK,
			eExtractFailed
		};

		/**
		Extract packed data back to directories and files into a directory.
		Return value : 0 success; 1 : some is OK; 2 : total failed
		*/ 
		EPkgExtract		Extract(const Cactus::String& strPath);

		/**
		*/ 
		SFATItem*		OpenItem(const Cactus::String& strPathName);
		size_t			Read(SFATItem* pItem, void* buf, size_t sizeWanted);
		size_t			GetLength(SFATItem* pItem);
		size_t			Tell(SFATItem* pItem);
		bool			Eof(SFATItem* pItem);
		bool			Seek(SFATItem* pItem, size_t pos, FileSeekMode seekMode = FileSeekMode_FromCur);

	private:
		FILE*			_pFile;
		float			_fVersion;
		int				_lCreationTime;

		typedef Cactus::map<Cactus::String, SFATItem, StringNoCaseLess >::type		FATItemMapType;
		FATItemMapType	_FATItems;

		struct SFATItemInteral : public AllocatedObject<GeneralAllocPolicy>
		{
			SFATItemInteral()
			{
				memset(_strPathName, 0, M_MAX_PATH);
			}
			char		_strPathName[M_MAX_PATH];
			size_t		_szOriginal;
			size_t		_szCompressed;
			size_t		_szOffset;
		};
	};
	SmartPointer(PkgFile);
}
