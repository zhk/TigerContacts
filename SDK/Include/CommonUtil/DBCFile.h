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
|	DBCFile���ݿ��ļ���(������/�ͻ���ͨ��)
|---------------------------------------------------------------------
|		|  �������ļ���ʽ  |
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
|					��RecordsNum��
|	StringBlock
|				  char[StringSize]
|							  StringBlock
|---------------------------------------------------------------------
|		|  �ı��ļ���ʽ  |
|
|	Line 1:	����(INT, FLOAT, STRING)
|	Line 2: Ӣ��ע��
|	Line 3: ����ע��
|	Line 4: ������ �� ע����(#��ʼΪע��)
|--------------------------------------------------------------------*/

namespace Cactus
{

	class COMMONUTIL_API DBCFile : public Cactus::Object
	{
	public:

		static const Cactus::uint16		s_sMagicKey		= 0x0DBC;
		static const Cactus::uint16		s_sCurVersion	= 1;


		//�ļ�ͷ
		struct SDBCFileHeader
		{
			Cactus::uint16	_MagicKey;				//��ʾ	0x0DBC
			Cactus::uint16	_version;				//�汾
			int				_fieldsNum;				//����
			int				_recordsNum;			//����
			int				_stringBufferSize;		//�ַ�������С
		};

		//�ֶ���������
		enum EFieldType
		{
			eFieldInt		= 0,	//����
			eFiledFloat		= 1,	//������
			eFieldString	= 2,	//�ַ���
		};

		//���ݿ��ʽ����
		typedef Cactus::vector< EFieldType >::type	VectorFieldType;

		//���ݶ�
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

		//������
		typedef Cactus::vector< UField >::type		RecordType;

		typedef Cactus::vector< RecordType >::type	DBType;

	public:

		//���ı��ļ�������һ�����ݿ�
		bool					LoadFromFile(const char* szFileName);

		//�����ڴ��е��ļ���
		bool					LoadFromMemory(const char* pMemory, const char* pDeadEnd);

		//����Ϊ�������ļ�
		bool					SaveBinaryToFile(const char* szFileName);


		//����������
		const UField*			GetFieldByIndexValue(int nValue) const;
		const RecordType*		GetRecordByIndexValue(int nValue) const;

		//����λ�ò���
		const UField*			GetFieldValue(int nRecordLine, int nColumNum) const;

		//����ĳ�е���ָ��ֵ�ĵ�һ��(����������������λ��)
		const UField*			GetFirstColumnEqual(int nColumnNum, const UField& value, bool bIndexed = true) const;



		//ȡ������
		size_t					GetFieldsNum() const { return _vectorFieldTypes.size(); }

		//ȡ�ü�¼������
		size_t					GetRecordsNum() const { return _Database.size(); }


	protected:

		//����������
		void					_CreateIndex();

		//��ȡ�ı���ʽ����
		bool					_OpenFromMemoryImpl_Text(const char* pMemory, const char* pDeadEnd);

		//��ȡ�����Ƹ�ʽ����
		bool					_OpenFromMemoryImpl_Binary(const char* pMemory, const char* pDeadEnd);


#if Cactus_PLATFORM == Cactus_PLATFORM_WIN32
		typedef Cactus::hashmap< int, RecordType*>::type   FieldHashMapType;
#else
		typedef Cactus::map< int, RecordType*>::type   FieldHashMapType;
#endif

		//���ݿ��ʽ����
		VectorFieldType			_vectorFieldTypes;

		//������
		DBType					_Database;

		//�ַ�����
		char*					_pStringBuffer;

		//�ַ�������С
		int						_szStringBuffer;

		//������
		FieldHashMapType		_mapIndexHash;

		//������
		int						_indexColumn;

		Cactus::String			_strFileName;

	public:

		static int			_ConvertStringToVector(const char* strStrINTgSource, Cactus::vector< Cactus::String >::type& vRet, const char* szKey, bool bOneOfKey, bool bIgnoreEmpty);

		//���ڴ����ַ�����ȡһ���ı�(���ջ��з�)
		static const char*	_GetLineFromMemory(char* pStringBuf, int nBufSize, const char* pMemory, const char* pDeadEnd);

		//�Ƚ�����ֵ�Ƿ����
		template < EFieldType T>	
		static bool			_FieldEqual(const UField& a, const UField& b);

	public:
		DBCFile(int iIndexColumn);
		~DBCFile();
	};

}