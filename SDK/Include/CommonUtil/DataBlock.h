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

		// 读（写）指针重置
		void				reset(bool reset_used_flag_ = false);

		// 获取数据起始指针
		const uint8*		get_base_ptr() const;
		uint8*				get_base_ptr();


		// 获取写指针
		const uint8*		get_wr_ptr() const;
		uint8*				get_wr_ptr();


		// 获取读指针
		const uint8*		get_rd_ptr() const;
		uint8*				get_rd_ptr();

		// 将读指针下移sz字节
		uint8*				rd_ptr(size_t sz);

		// 获取数据有效大小，即写指针位置
		size_t				get_used_size() const;
		// 设置写指针位置
		void				set_used_size(size_t used_size);


		// 获取剩余可写空间大小
		size_t				get_space() const;
		// 获取剩余可读的数据大小
		size_t				get_remaining() const;


		// 写入数据
		bool				writeInto(const uint8* pdata, size_t len);

		// 读出数据
		bool				readOut(uint8* pdata, size_t len, bool MoveReadPtr = true);

	protected:

		/* 数据包可用空间大小 */
		size_t			_alloc_size;

		/* 空间的起始位置 */
		uint8*			_pbase;

		/* 实际数据块的大小，也是写指针的位置 */
		size_t			_used_size;

		/* 当前读指针的位置, 内存地址 */
		uint8*			_ptr_read;
	};

	typedef SharedPtr<DataBlock>	DataBlockPtr;

}