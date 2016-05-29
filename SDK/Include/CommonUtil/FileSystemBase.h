/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			FileSystemBase.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "SmartPointer.h"

#ifndef M_MAX_PATH
#	define	M_MAX_PATH	260
#endif

namespace Cactus
{
	struct FileInfo : public Object
	{
		FileInfo()
			: name("")
			, ext("")
			, path("")
			, originalSize(0)
			, compressedSize(0)
		{
		}

		Cactus::String	name;
		Cactus::String	ext;
		Cactus::String	path;
		size_t		originalSize;
		size_t		compressedSize;
	};

	enum FileSeekMode
	{
		FileSeekMode_FromBegin,
		FileSeekMode_FromCur,
		FileSeekMode_FromEnd,
	};
	class COMMONUTIL_API IDataInStream : public IRefObject
	{
	public:
		virtual size_t	Read(void* buf, size_t sizeWanted) = 0; //return number of bytes actually read
		virtual size_t	GetLength() = 0;
		virtual size_t	Tell() = 0;
		virtual bool	Eof() = 0;
		virtual bool	Seek(size_t pos, FileSeekMode seekMode = FileSeekMode_FromBegin) = 0;
		virtual void	Close() = 0;
		virtual IDataInStream& operator >> (uint8& value) = 0;
		virtual IDataInStream& operator >> (uint16& value) = 0;
		virtual IDataInStream& operator >> (uint32& value) = 0;
		virtual IDataInStream& operator >> (int8& value) = 0;
		virtual IDataInStream& operator >> (int16& value) = 0;
		virtual IDataInStream& operator >> (int32& value) = 0;
		virtual IDataInStream& operator >> (float& value) = 0;
		virtual IDataInStream& operator >> (double& value) = 0;
		virtual IDataInStream& operator >> (bool& value) = 0;
		virtual IDataInStream& operator >> (Cactus::WString& value) = 0;
		virtual IDataInStream& operator >> (Cactus::String& value) = 0;
	};
	SmartPointer(IDataInStream);

	class COMMONUTIL_API IDataOutStream : public IRefObject
	{
	public:
		virtual void Write(void* buf, size_t count) = 0;
		virtual size_t Tell() = 0;
		virtual bool Seek(size_t pos, FileSeekMode seekMode = FileSeekMode_FromBegin) = 0;
		virtual void Close() = 0;
		virtual IDataOutStream& operator << (uint8 value) = 0;
		virtual IDataOutStream& operator << (uint16 value) = 0;
		virtual IDataOutStream& operator << (uint32 value) = 0;
		virtual IDataOutStream& operator << (int8 value) = 0;
		virtual IDataOutStream& operator << (int16 value) = 0;
		virtual IDataOutStream& operator << (int32 value) = 0;
		virtual IDataOutStream& operator << (float value) = 0;
		virtual IDataOutStream& operator << (double value) = 0;
		virtual IDataOutStream& operator << (bool value) = 0;
		virtual IDataOutStream& operator << (const Cactus::WString& value) = 0;
		virtual IDataOutStream& operator << (const Cactus::String& value) = 0;
	};
	SmartPointer(IDataOutStream);

	class COMMONUTIL_API IFileSystem
	{
	public:
		virtual bool				FileExist(const Cactus::String& path) = 0;
		virtual FileInfo			GetFileInfo(const Cactus::String& path) = 0;
		virtual IDataInStreamPtr	OpenFileRead(const Cactus::String& path) = 0;
		virtual IDataOutStreamPtr	OpenFileWrite(const Cactus::String& path) = 0;
		virtual	bool				FindFiles(const Cactus::String& strPattern
										, StringVector& Outs
										, const Cactus::String& strFromDir = ""
										, bool bRecursive = false
										) = 0;
		virtual bool				DeleteFile(const Cactus::String& path, bool bEvenReadOnly = false) = 0;
		virtual bool				RenameFile(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										) = 0;
		virtual bool				CopyFile(const Cactus::String& pathFrom, const Cactus::String& pathTo) = 0;
		virtual bool				MoveFile(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										) = 0;
		virtual bool				MakeDir(const Cactus::String& path, bool bRecursive = false) = 0;
		virtual bool				DeleteDir(const Cactus::String& path, bool bEvenNotEmpty = false) = 0;
	};

}


