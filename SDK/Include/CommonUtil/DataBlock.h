/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			DataBlock.h
*	Description:
*			
*	Revision history:
*			2011-3-26	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "PreInclude.h"
#include "SharedPtr.h"

/*
	base_ptr	rd_ptr		   wr_ptr
	|			|			   |
	|___________|___remaining__|____________________
	|_________used_size________|_______space_______|
	|_________________alloc_space__________________|
*/


namespace Cactus
{

	class COMMONUTIL_API OutOfBoundException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{	
			return "Read exceeds the size of the buffer!";
		}
	};

	class COMMONUTIL_API UnGrowableException : public std::exception
	{
	public:
		virtual const char *what() const throw()
		{	
			return "The buffer is NOT grow able!";
		}
	};

	class COMMONUTIL_API DataBlock : public Object
	{
	public:
		DataBlock(size_t alloc_space);
		virtual ~DataBlock();

		// ����д��ָ������
		void				reset(bool reset_used_flag_ = false);

		// ��ȡ������ʼָ��
		const uint8*		get_base_ptr() const;
		uint8*				get_base_ptr();


		// ��ȡдָ��
		const uint8*		get_wr_ptr() const;
		uint8*				get_wr_ptr();


		// ��ȡ��ָ��
		const uint8*		get_rd_ptr() const;
		uint8*				get_rd_ptr();

		// ����ָ������sz�ֽ�
		uint8*				rd_ptr(size_t sz);

		// ��ȡ������Ч��С����дָ��λ��
		size_t				get_used_size() const;
		// ����дָ��λ��
		void				set_used_size(size_t used_size);


		// ��ȡʣ���д�ռ��С
		size_t				get_space() const;
		// ��ȡʣ��ɶ������ݴ�С
		size_t				get_remaining() const;


		// д������
		bool				writeInto(const uint8* pdata, size_t len);

		// ��������
		bool				readOut(uint8* pdata, size_t len, bool MoveReadPtr = true);

	protected:

		/* ���ݰ����ÿռ��С */
		size_t			_alloc_size;

		/* �ռ����ʼλ�� */
		uint8*			_pbase;

		/* ʵ�����ݿ�Ĵ�С��Ҳ��дָ���λ�� */
		size_t			_used_size;

		/* ��ǰ��ָ���λ��, �ڴ��ַ */
		uint8*			_ptr_read;
	};

	typedef SharedPtr<DataBlock>	DataBlockPtr;

}