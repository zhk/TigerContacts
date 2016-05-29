/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			PkgFileSystem.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/

#pragma once

#include "PkgFile.h"

namespace Cactus
{

	class COMMONUTIL_API DataInStreamPkg : public IDataInStream, public RefObject
	{
		ImplementIObject
	public:
		DataInStreamPkg();
		DataInStreamPkg(PkgFilePtr sp, SFATItem* pItem);
		~DataInStreamPkg();

		virtual size_t	Read(void* buf, size_t sizeWanted);
		virtual size_t	GetLength();
		virtual size_t	Tell();
		virtual bool	Eof();
		virtual bool	Seek(size_t pos, FileSeekMode seekMode = FileSeekMode_FromCur);
		virtual void	Close();
		virtual IDataInStream& operator >> (uint8& value);
		virtual IDataInStream& operator >> (uint16& value);
		virtual IDataInStream& operator >> (uint32& value);
		virtual IDataInStream& operator >> (int8& value);
		virtual IDataInStream& operator >> (int16& value);
		virtual IDataInStream& operator >> (int32& value);
		virtual IDataInStream& operator >> (float& value);
		virtual IDataInStream& operator >> (double& value);
		virtual IDataInStream& operator >> (bool& value);
		virtual IDataInStream& operator >> (Cactus::WString& value);
		virtual IDataInStream& operator >> (Cactus::String& value);

	protected:
		PkgFilePtr		_spPkgFile;
		SFATItem*		_pItem;
	};
	SmartPointer(DataInStreamPkg);

	/**
	Note : All write operations are ignored.
	*/
	class COMMONUTIL_API PkgFileSystem : public IFileSystem
	{
	public:
		PkgFileSystem();
		~PkgFileSystem();

		bool						AddPkgFile(const Cactus::String& path);
		SFATItem*					SearchInPkgs(const Cactus::String& path, PkgFilePtr& sp);

		virtual bool				FileExist(const Cactus::String& path);
		virtual FileInfo			GetFileInfo(const Cactus::String& path);
		virtual IDataInStreamPtr	OpenFileRead(const Cactus::String& path);
		virtual IDataOutStreamPtr	OpenFileWrite(const Cactus::String& path);
		virtual	bool				FindFiles(const Cactus::String& strPattern
										, StringVector& Outs
										, const Cactus::String& strFromDir = ""
										, bool bRecursive = false
										);
		virtual bool				Delete(const Cactus::String& path, bool bEvenReadOnly = false);
		virtual bool				Rename(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										);
		virtual bool				Copy(const Cactus::String& pathFrom, const Cactus::String& pathTo);
		virtual bool				Move(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										);
		virtual bool				MakeDir(const Cactus::String& path, bool bRecursive = false);
		virtual bool				DeleteDir(const Cactus::String& path, bool bEvenNotEmpty = false);

	protected:
		Cactus::list<PkgFilePtr>::type		_spPkgFiles;	//We can have more PkgFiles for one PkgFileSystem
	};
}