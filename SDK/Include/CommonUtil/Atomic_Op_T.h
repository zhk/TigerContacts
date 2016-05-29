/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			Atomic_Op_T.h
*	Description:
*
*	Revision history:
*			2011-10-26	* Copy from ACE
===============================================================================*/
#ifndef _ATOMIC_OP_T_H
#define _ATOMIC_OP_T_H

#pragma once

#include "Mutex.h"

namespace Cactus
{
	template<typename TYPE>
	struct Cactus_Type_Traits
	{
		typedef TYPE const & parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<bool>
	{
		typedef bool parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<char>
	{
		typedef char parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<signed char>
	{
		typedef signed char parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<unsigned char>
	{
		typedef unsigned char parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<short>
	{
		typedef short parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<unsigned short>
	{
		typedef unsigned short parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<int>
	{
		typedef int parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<unsigned int>
	{
		typedef unsigned int parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<long>
	{
		typedef long parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<unsigned long>
	{
		typedef unsigned long parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<long long>
	{
		typedef long long parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<unsigned long long>
	{
		typedef unsigned long long parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<float>
	{
		typedef float parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<double>
	{
		typedef double parameter_type;
	};

	template<>
	struct Cactus_Type_Traits<long double>
	{
		typedef long double parameter_type;
	};

	template<typename TYPE>
	struct Cactus_Type_Traits<TYPE*>
	{
		typedef TYPE* parameter_type;
	};

	/**
	* @class Cactus_Atomic_Op_Ex
	*
	* @brief Transparently parameterizes synchronization into basic
	* arithmetic operations.
	*
	* This class is described in an article in the July/August 1994
	* issue of the C++ Report magazine.  It implements a
	* templatized version of the Decorator pattern from the GoF book.
	*
	* Cactus_Atomic_Op_Ex objects must be constructed with a reference
	* to an existing lock. A single lock can be shared between
	* multiple Cactus_Atomic_Op_Ex objects. If you do not require this
	* ability consider using the Cactus_Atomic_Op class instead, which
	* may be able to take advantage of platform-specific
	* optimisations to provide atomic operations without requiring a
	* lock.
	*/
	template <class LOCK, typename TYPE>
	class Cactus_Atomic_Op_Ex
	{
	public:

		typedef typename Cactus_Type_Traits<TYPE>::parameter_type arg_type;

		// = Initialization methods.

		/// Initialize @c value_ to 0.
		Cactus_Atomic_Op_Ex (LOCK & mtx);

		/// Initialize @c value_ to c.
		Cactus_Atomic_Op_Ex (LOCK & mtx, arg_type c);

		// = Accessors.

		/// Atomically pre-increment @c value_.
		TYPE operator++ (void);

		/// Atomically post-increment @c value_.
		TYPE operator++ (int);

		/// Atomically increment @c value_ by rhs.
		TYPE operator+= (arg_type rhs);

		/// Atomically pre-decrement @c value_.
		TYPE operator-- (void);

		/// Atomically post-decrement @c value_.
		TYPE operator-- (int);

		/// Atomically decrement @c value_ by rhs.
		TYPE operator-= (arg_type rhs);

		/// Atomically compare @c value_ with rhs.
		bool operator== (arg_type rhs) const;

		/// Atomically compare @c value_ with rhs.
		bool operator!= (arg_type rhs) const;

		/// Atomically check if @c value_ greater than or equal to rhs.
		bool operator>= (arg_type rhs) const;

		/// Atomically check if @c value_ greater than rhs.
		bool operator> (arg_type rhs) const;

		/// Atomically check if @c value_ less than or equal to rhs.
		bool operator<= (arg_type rhs) const;

		/// Atomically check if @c value_ less than rhs.
		bool operator< (arg_type rhs) const;

		/// Atomically assign rhs to @c value_.
		Cactus_Atomic_Op_Ex<LOCK, TYPE> &operator= (arg_type rhs);

		/// Atomically assign <rhs> to @c value_.
		Cactus_Atomic_Op_Ex<LOCK, TYPE> &operator= (
			Cactus_Atomic_Op_Ex<LOCK, TYPE> const & rhs);

		/// Explicitly return @c value_.
		TYPE value (void) const;

		// ACE_ALLOC_HOOK_DECLARE;
		// Declare the dynamic allocation hooks.

		/// Manage copying...
		Cactus_Atomic_Op_Ex (Cactus_Atomic_Op_Ex<LOCK, TYPE> const &);

		/**
		* Returns a reference to the underlying LOCK.  This makes it
		* possible to acquire the lock explicitly, which can be useful in
		* some cases if you instantiate the Cactus_Atomic_Op_Ex with an
		* ACE_Recursive_Mutex or ACE_Process_Mutex.
		*
		* @note The right name would be lock_, but HP/C++ will choke on that!
		*/
		LOCK & mutex (void);

		/**
		* Explicitly return @c value_ (by reference).  This gives the user
		* full, unrestricted access to the underlying value.  This method
		* will usually be used in conjunction with explicit access to the
		* lock.  Use with care ;-)
		*/
		TYPE & value_i (void);

	private:
		/// Type of synchronization mechanism.
		LOCK & mutex_;

		/// Current object decorated by the atomic op.
		TYPE value_;
	};

	/**
	* @class Cactus_Atomic_Op
	*
	* @brief Transparently parameterizes synchronization into basic
	* arithmetic operations.
	*
	* This class is described in an article in the July/August 1994
	* issue of the C++ Report magazine.  It implements a
	* templatized version of the Decorator pattern from the GoF book.
	*
	*/
	template <class LOCK, typename TYPE>
	class Cactus_Atomic_Op
	{
	public:

		typedef typename Cactus_Type_Traits<TYPE>::parameter_type arg_type;

		/// Initialize @c value_ to 0.
		Cactus_Atomic_Op (void);

		/// Initialize @c value_ to c.
		Cactus_Atomic_Op (arg_type c);

		/// Manage copying...
		Cactus_Atomic_Op (Cactus_Atomic_Op<LOCK, TYPE> const & c);

		/// Atomically assign @a rhs to @c value_.
		Cactus_Atomic_Op<LOCK, TYPE> & operator= (arg_type rhs);

		/// Atomically assign @a rhs to @c value_.
		Cactus_Atomic_Op<LOCK, TYPE> & operator= (
			Cactus_Atomic_Op<LOCK, TYPE> const & rhs);

		/// Atomically pre-increment @c value_.
		TYPE operator++ (void);

		/// Atomically post-increment @c value_.
		TYPE operator++ (int);

		/// Atomically increment @c value_ by rhs.
		TYPE operator+= (arg_type rhs);

		/// Atomically pre-decrement @c value_.
		TYPE operator-- (void);

		/// Atomically post-decrement @c value_.
		TYPE operator-- (int);

		/// Atomically decrement @c value_ by @a rhs.
		TYPE operator-= (arg_type rhs);

		/// Atomically compare @c value_ with @a rhs.
		bool operator== (arg_type rhs) const;

		/// Atomically compare @c value_ with @a rhs.
		bool operator!= (arg_type rhs) const;

		/// Atomically check if @c value_ greater than or equal to @a rhs.
		bool operator>= (arg_type rhs) const;

		/// Atomically check if @c value_ greater than @a rhs.
		bool operator> (arg_type rhs) const;

		/// Atomically check if @c value_ less than or equal to @a rhs.
		bool operator<= (arg_type rhs) const;

		/// Atomically check if @c value_ less than @a rhs.
		bool operator< (arg_type rhs) const;

		/// Explicitly return @c value_.
		TYPE value (void) const;

		/**
		* Explicitly return @c value_ (by reference).  This gives the user
		* full, unrestricted access to the underlying value.  This method
		* will usually be used in conjunction with explicit access to the
		* lock.  Use with care ;-)
		*/
		TYPE & value_i (void);

	private:
		/// Type of synchronization mechanism.
		LOCK own_mutex_;

		/// Underlying atomic op implementation.
		Cactus_Atomic_Op_Ex <LOCK, TYPE> impl_;
	};


	//
	// Cactus_Atomic_Op_Ex inline functions
	//
	template <class LOCK, class TYPE>
	inline LOCK &
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::mutex (void)
	{
		return this->mutex_;
	}

	template <class LOCK, class TYPE>
	inline
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::Cactus_Atomic_Op_Ex (LOCK & mtx)
		: mutex_ (mtx)
		, value_ (0)
	{
	}

	template <class LOCK, class TYPE>
	inline 
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::Cactus_Atomic_Op_Ex (
		LOCK & mtx,
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type c)
		: mutex_ (mtx)
		, value_ (c)
	{
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator++ (void)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return ++this->value_;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator+= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return this->value_ += rhs;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator-- (void)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return --this->value_;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator-= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return this->value_ -= rhs;
	}

	template <class LOCK, class TYPE>
	inline
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::Cactus_Atomic_Op_Ex (
		Cactus_Atomic_Op_Ex<LOCK, TYPE> const & rhs)
		: mutex_ (rhs.mutex_)
		, value_ (rhs.value ())  // rhs.value() returns atomically
	{
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator++ (int)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return this->value_++;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator-- (int)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return this->value_--;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator== (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, false);
		return this->value_ == rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator!= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		return !(*this == rhs);
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator>= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, false);
		return this->value_ >= rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator> (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, false);
		return this->value_ > rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator<= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, false);
		return this->value_ <= rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator< (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, false);
		return this->value_ < rhs;
	}

	template <class LOCK, class TYPE>
	inline Cactus_Atomic_Op_Ex<LOCK, TYPE> &
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator= (
		Cactus_Atomic_Op_Ex<LOCK, TYPE> const & rhs)
	{
		Cactus_Atomic_Op_Ex<LOCK, TYPE> tmp (rhs);

		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, *this);
		std::swap (this->value_, tmp.value_);

		return *this;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::value (void) const
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, this->value_);
		return this->value_;
	}

	template <class LOCK, class TYPE>
	inline TYPE &
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::value_i (void)
	{
		// Explicitly return <value_> (by reference).  This gives the user
		// full, unrestricted access to the underlying value.  This method
		// will usually be used in conjunction with explicit access to the
		// lock.  Use with care ;-)
		return this->value_;
	}

	template <class LOCK, class TYPE>
	inline Cactus_Atomic_Op_Ex<LOCK, TYPE> &
		Cactus_Atomic_Op_Ex<LOCK, TYPE>::operator= (
		typename Cactus_Atomic_Op_Ex<LOCK, TYPE>::arg_type rhs)
	{
		CACTUS_GUARD_RETURN (LOCK, ace_mon, this->mutex_, *this);
		this->value_ = rhs;
		return *this;
	}

	//
	// Cactus_Atomic_Op inline functions
	//
	template <class LOCK, class TYPE>
	inline
		Cactus_Atomic_Op<LOCK, TYPE>::Cactus_Atomic_Op (void)
		: impl_ (this->own_mutex_)
	{
	}

	template <class LOCK, class TYPE>
	inline 
		Cactus_Atomic_Op<LOCK, TYPE>::Cactus_Atomic_Op (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type c)
		: impl_ (own_mutex_, c)
	{
	}



	template <class LOCK, class TYPE> inline
		Cactus_Atomic_Op<LOCK, TYPE>::Cactus_Atomic_Op (
		Cactus_Atomic_Op<LOCK, TYPE> const & rhs)
		: impl_ (own_mutex_, rhs.value ())
	{
	}

	template <class LOCK, class TYPE>
	inline Cactus_Atomic_Op<LOCK, TYPE> &
		Cactus_Atomic_Op<LOCK, TYPE>::operator= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type i)
	{
		this->impl_ = i;
		return *this;
	}

	template <class LOCK, class TYPE>
	inline Cactus_Atomic_Op<LOCK, TYPE> &
		Cactus_Atomic_Op<LOCK, TYPE>::operator= (
		Cactus_Atomic_Op<LOCK, TYPE> const & rhs)
	{
		this->impl_ = rhs.impl_;
		return *this;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator++ (void)
	{
		return ++this->impl_;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator++ (int)
	{
		return this->impl_++;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator+= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs)
	{
		return this->impl_ += rhs;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator-- (void)
	{
		return --this->impl_;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator-- (int)
	{
		return this->impl_--;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::operator-= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs)
	{
		return this->impl_ -= rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator== (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ == rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator!= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ != rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator>= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ >= rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator> (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ > rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator<= (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ <= rhs;
	}

	template <class LOCK, class TYPE>
	inline bool
		Cactus_Atomic_Op<LOCK, TYPE>::operator< (
		typename Cactus_Atomic_Op<LOCK, TYPE>::arg_type rhs) const
	{
		return this->impl_ < rhs;
	}

	template <class LOCK, class TYPE>
	inline TYPE
		Cactus_Atomic_Op<LOCK, TYPE>::value (void) const
	{
		return this->impl_.value ();
	}

	template <class LOCK, class TYPE>
	inline TYPE &
		Cactus_Atomic_Op<LOCK, TYPE>::value_i (void)
	{
		return this->impl_.value_i ();
	}

}

#endif /*_ATOMIC_OP_T_H*/
