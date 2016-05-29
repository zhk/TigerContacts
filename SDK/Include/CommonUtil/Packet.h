/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Packet.h
*	Description:
*			
*	Revision history:
*			2007-7-13	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "DataBlock.h"
#include "NatureTime.h"
#include "BitSet.h"

/*
	base_ptr	rd_ptr		   wr_ptr
	|			|			   |
	|___________|___remaining__|____________________
	|_________used_size________|_______space_______|
	|_________________alloc_space__________________|
*/

#define M_Packet_Default_Size	2048
#define M_Packet_Inc_Size		512

namespace Cactus
{

	/** Notice: size in buffer is in uint.
	*/
	class COMMONUTIL_API Packet : public Object
	{
	public:
		Packet(size_t size_ = M_Packet_Default_Size);
		Packet(const Packet &packet_);
		Packet(const DataBlockPtr& spDataBlock, uint8* pos, size_t size);
		virtual ~Packet();

		// ������ֵ
		Packet& operator = (const Packet &packet_);

		// ����д��ָ������
		void				reset(bool reset_used_flag_ = false);

		// ׷��һ��Packet
		void				append(const Packet &packet_);

		// ��Packet���ݷָoutPkt���棬�������δ���귵��true
		// ���bTruncΪtrue���Ƴ��ָ��������
		bool				split(Packet& outPkt, size_t len_, bool bTrunc = false);

		// ������ָ��֮ǰ������
		void				truncate();

		// ���Ż�����
		bool				resize(size_t new_size_);

		// ����дָ��λ��
		void				set_used_size(size_t used_size_);

		// ��ȡ������ʼָ��
		const uint8*		get_base_ptr() const;
		uint8*				get_base_ptr();

		// ��ȡ��ָ��
		const uint8*		get_rd_ptr() const;
		uint8*				get_rd_ptr();

		// ����ָ������sz�ֽ�
		uint8*				rd_ptr(size_t sz);

		// ��ȡдָ��
		const uint8*		get_wr_ptr() const;
		uint8*				get_wr_ptr();

		// ���ö�ָ��
		bool				set_rd_ptr(uint8* ptr);

		// ��ȡ������Ч��С����дָ��λ��
		size_t				get_used_size() const;

		// ��ȡʣ��ɶ������ݴ�С
		size_t				get_remaining() const;

		// ��Packetд������
		bool				writeInto(const uint8* pdata_, size_t len_);

		// ��Packetд�����ݣ����ǲ�Ӱ��д��ǰ�ķ����С��д��λ�ú����ݳ���
		size_t				patchInto(const uint8* pdata_, size_t len_, size_t pos = 0);

		// ��Packet��������
		bool				readOut(uint8* pdata_, size_t len_, bool MoveReadPtr = true);

		// ��Packet�������ݣ�pos��ƫ������ͷ�ľ���λ��
		bool				peekOut(uint8* pdata_, size_t len_, size_t pos = 0) const;

		// ��Packet����ת��ΪBase64��д��String
		void				readOutToBase64String(Cactus::String& str, bool MoveReadPtr = true);

		// ��Base6�����Stringд��Packet
		void				writeIntoBase64String(const Cactus::String& str);



		/* put data */
		Packet& operator << (const int& v);
		Packet& operator << (const long& v);
		Packet& operator << (const uint& v);
		Packet& operator << (const int8& v);
		Packet& operator << (const uint8& v);
		Packet& operator << (const int16& v);
		Packet& operator << (const uint16& v);
		Packet& operator << (const int64& v);
		Packet& operator << (const uint64& v);
		Packet& operator << (const bool& v);
		Packet& operator << (const float& v);
		Packet& operator << (const double& v);
		Packet& operator << (const wchar_t& v);
		Packet& operator << (const Cactus::String& str);
		Packet& operator << (const Cactus::WString& str);
		Packet& operator << (const Packet& r);
		Packet& operator << (const NatureTime& t);


		/* get data */
		Packet& operator >> (int& v);
		Packet& operator >> (long& v);
		Packet& operator >> (uint& v);
		Packet& operator >> (int8& v);
		Packet& operator >> (uint8& v);
		Packet& operator >> (int16& v);
		Packet& operator >> (uint16& v);
		Packet& operator >> (int64& v);
		Packet& operator >> (uint64& v);
		Packet& operator >> (bool& v);
		Packet& operator >> (float& v);
		Packet& operator >> (double& v);
		Packet& operator >> (wchar_t& v);
		Packet& operator >> (Cactus::String& str);
		Packet& operator >> (Cactus::WString& str);
		Packet& operator >> (Packet& r);
		Packet& operator >> (NatureTime& t);


		/* peek data, read data at pos offset without affect the rd_ptr */
		template<class T>
		bool Peek(T& v, size_t pos = 0)
		{
			return peekOut((uint8*)&v, sizeof(v), pos);
		}


		/* patch data, write data at position and not affect the wr_ptr, return the end position after data patched */
		template<class T>
		size_t Patch(const T& v, size_t pos = 0)
		{
			return patchInto((const uint8*)&v, sizeof(v), pos);
		}

	protected:

		void			_init();

		/* ���ݰ����ÿռ��С */
		size_t			_alloc_size;

		/* �ռ����ʼλ�� */
		uint8*			_pbase;

		/* ʵ�����ݿ�Ĵ�С��Ҳ��дָ���λ�� */
		size_t			_used_size;

		/* ��ǰ��ָ���λ��, �ڴ��ַ */
		uint8*			_ptr_read;

		bool			_growable;

		DataBlockPtr	_spDataBlock;
	};

	typedef SharedPtr<Packet>	PacketPtr;


	/* ��BitSetд��Packet */
	template<size_t bits>
	inline Cactus::Packet& operator << (Cactus::Packet& os, const BitSet<bits>& bt)
	{
		if (bt.any())
		{
			uint16 sz = uint16(bt.getwordsize());
			os << sz;

			for (uint16 t = 0; t < sz; ++t)
			{
				os << bt.getword(t);
			}
		}
		else
		{
			uint16 sz = 0;
			os << sz;
		}

		return os;
	}

	/* ��Packet��ȡBitSet */
	template<size_t bits>
	inline Cactus::Packet& operator >> (Cactus::Packet& is, BitSet<bits>& bt)
	{
		uint16 sz = 0;
		is >> sz;

		if (sz == 0)
		{
			bt.reset();
		}
		else
		{
			for (uint16 t = 0; t < sz; ++t)
			{
				uint val = 0;
				is >> val;

				bt.setword(t, val);
			}
		}

		return is;
	}


	//------------------------------------------------------------------------------------------------
	/* peek data */
	template<>
	inline bool Packet::Peek(Cactus::String& str, size_t pos)
	{
		uint8* rd_ptr = get_rd_ptr();

		if( set_rd_ptr(_pbase + pos) )
		{
			*this >> str;

			return set_rd_ptr(rd_ptr);
		}

		return false;
	}

	template<>
	inline bool Packet::Peek(Cactus::WString& str, size_t pos)
	{
		uint8* rd_ptr = get_rd_ptr();

		if( set_rd_ptr(_pbase + pos) )
		{
			*this >> str;

			return set_rd_ptr(rd_ptr);
		}

		return false;
	}

	template<>
	inline bool Packet::Peek(Packet& p, size_t pos)
	{
		uint8* rd_ptr = get_rd_ptr();

		if( set_rd_ptr(_pbase + pos) )
		{
			*this >> p;

			return set_rd_ptr(rd_ptr);
		}

		return false;
	}

	template<>
	inline bool Packet::Peek(NatureTime& t, size_t pos)
	{
		int iSeconds = 0;

		if ( Peek(iSeconds) )
		{
			t = iSeconds;
			return true;
		}

		return false;
	}

	/* ��Packet͵ȡBitSet */
	template<size_t bits>
	inline bool Peek(Cactus::Packet& is, BitSet<bits>& bt, size_t pos)
	{
		uint8* rd_ptr = is.get_rd_ptr();

		if( is.set_rd_ptr(is._pbase + pos) )
		{
			is >> bt;

			return is.set_rd_ptr(rd_ptr);
		}

		return false;
	}

	//------------------------------------------------------------------------------------------------
	/* patch data */
	template<>
	inline size_t Packet::Patch(const Cactus::String& str, size_t pos)
	{
		uint len = uint( str.size() * sizeof(Cactus::String::value_type) );

		uint lenOld;
		if( Peek(lenOld, pos) && len == lenOld )
		{
			pos += sizeof(uint);
			return patchInto((const uint8*)str.c_str(), len, pos);
		}

		return 0;
	}

	template<>
	inline size_t Packet::Patch(const Cactus::WString& str, size_t pos)
	{
		uint len = uint( str.size() * sizeof(Cactus::WString::value_type) );

		uint lenOld;
		if( Peek(lenOld, pos) && len == lenOld )
		{
			pos += sizeof(uint);
			return patchInto((const uint8*)str.c_str(), len, pos);
		}

		return 0;
	}

	template<>
	inline size_t Packet::Patch(const Packet& p, size_t pos)
	{
		uint len = uint( p.get_used_size() );

		uint lenOld;
		if( Peek(lenOld, pos) && len == lenOld )	//��֤�����ľɵ�Packet���Ⱥ��µ�Packet����һ��
		{
			pos += sizeof(uint);
			return patchInto(p.get_base_ptr(), len, pos);
		}

		return 0;
	}

	template<>
	inline size_t Packet::Patch(const NatureTime& t, size_t pos)
	{
		int iSeconds = t;

		return patchInto((const uint8*)&t, sizeof(int), pos);
	}

	template<size_t bits>
	inline size_t Patch(Cactus::Packet& os, const BitSet<bits>& bt, size_t pos)
	{
		if (!bt.any())
		{
			// only need move pos when bt is zero
			return pos + sizeof(uint16);
		}

		uint16 sz = bt.getwordsize();
			
		uint16 lenOld;
		if( os.Peek(lenOld, pos) && sz == lenOld )
		{
			pos += sizeof(uint16);

			for (uint16 t = 0; t < sz; ++t)
			{
				if( !os.Patch(bt.getword(t), pos) )
				{
					return 0;
				}

				pos += sizeof(uint);
			}

			return pos;
		}

		return 0;
	}

}