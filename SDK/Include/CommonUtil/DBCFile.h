/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			DBCFile.h
*	Description:
*			
*	Revision history:
*			2011-1-11	* Created by Zhang Kun
===============================================================================*/
#pragma once

#include "PreInclude.h"

/*--------------------------------------------------------------------
|	DBCFile数据库文件类(服务器/客户端通用)
|---------------------------------------------------------------------
|		|  二进制文件格式  |
|
|		Offset |	Type  |  Description  
|		-------+----------+------------------
|	Head
|		0X000	  uint		DBC File Magic Key, always 0XDDBBCC00
|		0X004	  uint      Number of records in the file 
|		0X008     uint      Number of 4-byte fields per record
|		0X010     uint      String block size 
|   FieldType
|		0X014     uint[FieldNum]   
|							  The type of fields(0-int, 1-float, 2-string)
|   FieldBlock
|				  UField[FieldNum]
|				  UField[FieldNum]
|					共RecordsNum行
|	StringBlock
|				  char[StringSize]
|							  StringBlock
|---------------------------------------------------------------------
|		|  文本文件格式  |
|
|	Line 1:	类型(INT, FLOAT, STRING)
|	Line 2: 英文注释
|	Line 3: 中文注释
|	Line 4: 数据行 或 注释行(#开始为注释)
|--------------------------------------------------------------------*/

namespace Cactus
{

	class COMMONUTIL_API DBCFile : public Cactus::Object
	{
	public:

		static const Cactus::uint16		s_sMagicKey		= 0x0DBC;
		static const Cactus::uint16		s_sCurVersion	= 1;


		//文件头
		struct SDBCFileHeader
		{
			Cactus::uint16	_MagicKey;				//标示	0x0DBC
			Cactus::uint16	_version;				//版本
			int				_fieldsNum;				//列数
			int				_recordsNum;			//行数
			int				_stringBufferSize;		//字符串区大小
		};

		//字段数据类型
		enum EFieldType
		{
			eFieldInt		= 0,	//整数
			eFiledFloat		= 1,	//浮点数
			eFieldString	= 2,	//字符串
		};

		//数据库格式描述
		typedef Cactus::vector< EFieldType >::type	VectorFieldType;

		//数据段
		union UField
		{
			float		_fValue;
			int			_iValue;
			const char*	_strValue;	// Just for runtime!

			//Construct
			UField() {}
			UField(int v) { _iValue = v; }
			UField(float v) { _fValue = v; }
			UField(const char* v) { _strValue = v; }
		};

		//数据区
		typedef Cactus::vector< UField >::type		RecordType;

		typedef Cactus::vector< RecordType >::type	DBType;

	public:

		//打开文本文件，生成一个数据库
		bool					LoadFromFile(const char* szFileName);

		//根据内存中的文件打开
		bool					LoadFromMemory(const char* pMemory, const char* pDeadEnd);

		//保存为二进制文件
		bool					SaveBinaryToFile(const char* szFileName);


		//按索引查找
		const UField*			GetFieldByIndexValue(int nValue) const;
		const RecordType*		GetRecordByIndexValue(int nValue) const;

		//按照位置查找
		const UField*			GetFieldValue(int nRecordLine, int nColumNum) const;

		//查找某列等于指定值的第一行(按照索引或者物理位置)
		const UField*			GetFirstColumnEqual(int nColumnNum, const UField& value, bool bIndexed = true) const;



		//取得列数
		size_t					GetFieldsNum() const { return _vectorFieldTypes.size(); }

		//取得记录的条数
		size_t					GetRecordsNum() const { return _Database.size(); }


	protected:

		//生成索引列
		void					_CreateIndex();

		//读取文本格式内容
		bool					_OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd);

		//读取二进制格式内容
		bool					_OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd);


#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32
		typedef Cactus::hashmap< int, RecordType*>::type   FieldHashMapType;
#else
		typedef Cactus::map< int, RecordType*>::type   FieldHashMapType;
#endif

		//数据库格式描述
		VectorFieldType			_vectorFieldTypes;

		//数据区
		DBType					_Database;

		//字符串区
		char*					_pStringBuffer;

		//字符串区大小
		int						_szStringBuffer;

		//索引表
		FieldHashMapType		_mapIndexHash;

		//索引列
		int						_indexColumn;

		Cactus::String			_strFileName;

	public:

		static int			_ConvertStringToVector(const char* strStrINTgSource, Cactus::vector< Cactus::String >::type& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);

		//从内存中字符串读取一行文本(按照换行符)
		static const char*	_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);

		//比较两个值是否相等
		template < EFieldType T>	
		static bool			_FieldEqual(const UField& a, const UField& b);

	public:
		DBCFile(int iIndexColumn);
		~DBCFile();
	};

}