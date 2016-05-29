/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			BitSet.h
*	Description:
*			
*	Revision history:
*			2010-3-30	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

namespace Cactus
{
	template<size_t _Bits>
	class BitSet
	{	// store fixed-length sequence of Boolean elements
	public:

		BitSet()
		{	// construct with all false values
			_Tidy();
		}

		bool operator[](size_t _Pos) const
		{	// subscript non-mutable sequence
			return (test(_Pos));
		}

		BitSet<_Bits>& set()
		{	// set all bits true
			_Tidy((uint)~0);
			return (*this);
		}

		BitSet<_Bits>& set(size_t _Pos,
			bool _Val = true)
		{	// set bit at _Pos to _Val
			if (_Bits <= _Pos)
				_Xran();	// _Pos off end
			if (_Val)
				_Array[_Pos / _Bitsperword] |= (uint)1 << _Pos % _Bitsperword;
			else
				_Array[_Pos / _Bitsperword] &= ~((uint)1 << _Pos % _Bitsperword);
			return (*this);
		}

		BitSet<_Bits>& reset()
		{	// set all bits false
			_Tidy();
			return (*this);
		}

		BitSet<_Bits>& reset(size_t _Pos)
		{	// set bit at _Pos to false
			return (set(_Pos, false));
		}

		BitSet<_Bits>& flip()
		{	// flip all bits
			for (int _Wpos = _Words; 0 <= _Wpos; --_Wpos)
				_Array[_Wpos] = (uint)~_Array[_Wpos];

			_Trim();
			return (*this);
		}

		BitSet<_Bits>& flip(size_t _Pos)
		{	// flip bit at _Pos
			if (_Bits <= _Pos)
				_Xran();	// _Pos off end
			_Array[_Pos / _Bitsperword] ^= (uint)1 << _Pos % _Bitsperword;
			return (*this);
		}

		template<class _Elem,
		class _Tr,
		class _Alloc>
			std::basic_string<_Elem, _Tr, _Alloc>
			to_string(_Elem _E0 = (_Elem)'0') const
		{	// convert BitSet to string
			std::basic_string<_Elem, _Tr, _Alloc> _Str;
			typename std::basic_string<_Elem, _Tr, _Alloc>::size_type _Pos;
			_Str.reserve(_Bits);

			for (_Pos = _Bits; 0 < _Pos; )
				_Str += (_Elem)(_E0 + (int)test(--_Pos));
			return (_Str);
		}

		size_t count() const
		{	// count number of set bits
			static char _Bitsperhex[] = "\0\1\1\2\1\2\2\3\1\2\2\3\2\3\3\4";
			size_t _Val = 0;
			for (int _Wpos = _Words; 0 <= _Wpos; --_Wpos)
				for (uint _Wordval = _Array[_Wpos]; _Wordval != 0; _Wordval >>= 4)
					_Val += _Bitsperhex[_Wordval & 0xF];
			return (_Val);
		}

		size_t size() const
		{	// return size of BitSet
			return (_Bits);
		}

		bool operator==(const BitSet<_Bits>& _Right) const
		{	// test for BitSet equality
			for (int _Wpos = _Words; 0 <= _Wpos; --_Wpos)
				if (_Array[_Wpos] != _Right.getword(_Wpos))
					return (false);
			return (true);
		}

		bool operator!=(const BitSet<_Bits>& _Right) const
		{	// test for BitSet inequality
			return (!(*this == _Right));
		}

		bool test(size_t _Pos) const
		{	// test if bit at _Pos is set
			if (_Bits <= _Pos)
				_Xran();	// _Pos off end
			return ((_Array[_Pos / _Bitsperword]
			& ((uint)1 << _Pos % _Bitsperword)) != 0);
		}

		bool any() const
		{	// test if any bits are set
			for (int _Wpos = _Words; 0 <= _Wpos; --_Wpos)
				if (_Array[_Wpos] != 0)
					return (true);
			return (false);
		}

		bool none() const
		{	// test if no bits are set
			return (!any());
		}

		uint getword(size_t _Wpos) const
		{	// get word at _Wpos
			return (_Array[_Wpos]);
		}

		size_t getwordsize() const
		{	// get word count
			return _Words + 1;
		}

		void setword(size_t _Wpos, uint v)
		{	// set word item to v
			if (_Wpos >= _Words + 1)
				_Xran();	// _Pos off end

			_Array[_Wpos] = v;
		}

	private:
		enum
		{	// parameters for packing bits into words
			_Bitsperword = (int)(CHAR_BIT * sizeof (uint)),	// bits in each word
			_Words = (int)(_Bits == 0
			? 0 : (_Bits - 1) / _Bitsperword)
		};	// NB: number of words - 1

		void _Tidy(uint _Wordval = 0)
		{	// set all words to _Wordval
			for (int _Wpos = _Words; 0 <= _Wpos; --_Wpos)
				_Array[_Wpos] = _Wordval;
			if (_Wordval != 0)
				_Trim();
		}

		void _Trim()
		{	// clear any trailing bits in last word
			if (_Bits % _Bitsperword != 0)
				_Array[_Words] &= ((uint)1 << _Bits % _Bitsperword) - 1;
		}

		void _Xran() const
		{	// report bit index out of range
			throw std::out_of_range("invalid bitset<N> position");
		}

		uint _Array[_Words + 1];	// the set of bits
	};

}