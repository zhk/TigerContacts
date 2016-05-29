/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			OSFileSystem.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "FileSystemBase.h"

namespace Cactus
{

	class COMMONUTIL_API InvalidDataInFile : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{
			return "Invalid data in file!";
		}
	};

	class COMMONUTIL_API DataInStreamOS : public IDataInStream, public RefObject
	{
		ImplementIObject
	public:
		DataInStreamOS();
		~DataInStreamOS();
		bool Open(const Cactus::String& path);

		virtual size_t Read(void* buf, size_t sizeWanted);
		virtual size_t GetLength();
		virtual size_t Tell();
		virtual bool Eof();
		virtual bool Seek(size_t pos, FileSeekMode seekMode = FileSeekMode_FromCur);
		virtual void Close();
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
		FILE* _file;
		size_t _length;
	};
	SmartPointer(DataInStreamOS);

	class COMMONUTIL_API DataOutStreamOS : public IDataOutStream, public RefObject
	{
		ImplementIObject
	public:
		DataOutStreamOS();
		~DataOutStreamOS();
		bool Open(const Cactus::String& path);

		virtual void Write(void* buf, size_t count);
		virtual size_t Tell();
		virtual bool Seek(size_t pos, FileSeekMode seekMode = FileSeekMode_FromCur);
		virtual void Close();
		virtual IDataOutStream& operator << (uint8 value);
		virtual IDataOutStream& operator << (uint16 value);
		virtual IDataOutStream& operator << (uint32 value);
		virtual IDataOutStream& operator << (int8 value);
		virtual IDataOutStream& operator << (int16 value);
		virtual IDataOutStream& operator << (int32 value);
		virtual IDataOutStream& operator << (float value);
		virtual IDataOutStream& operator << (double value);
		virtual IDataOutStream& operator << (bool value);
		virtual IDataOutStream& operator << (const Cactus::WString& value);
		virtual IDataOutStream& operator << (const Cactus::String& value);

	protected:
		FILE* _file;
		size_t _length;
	};
	SmartPointer(DataOutStreamOS);

	class COMMONUTIL_API OSFileSystem : public IFileSystem
	{
	public:
		void SetRootPath(const Cactus::String& path);

		virtual bool				FileExist(const Cactus::String& path);
		virtual FileInfo			GetFileInfo(const Cactus::String& path);
		virtual IDataInStreamPtr	OpenFileRead(const Cactus::String& path);
		virtual IDataOutStreamPtr	OpenFileWrite(const Cactus::String& path);
		/**
		Note : If recursive, strFromDir does not support relative directory like ../
		*/
		virtual	bool				FindFiles(const Cactus::String& strPattern
										, StringVector& Outs
										, const Cactus::String& strFromDir = ""
										, bool bRecursive = false
										);
		virtual bool				DeleteFile(const Cactus::String& path, bool bEvenReadOnly = false);
		virtual bool				RenameFile(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										);
		virtual bool				CopyFile(const Cactus::String& pathFrom, const Cactus::String& pathTo);
		virtual bool				MoveFile(const Cactus::String& pathFrom
										, const Cactus::String& pathTo
										, bool bEvenReadOnly = false
										);
		virtual bool				MakeDir(const Cactus::String& path, bool bRecursive = false);
		virtual bool				DeleteDir(const Cactus::String& path, bool bEvenNotEmpty = false);

	protected:
		Cactus::String _rootPath;
	};
}
