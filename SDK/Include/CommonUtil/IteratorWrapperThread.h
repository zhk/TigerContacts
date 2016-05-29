/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2009 Torus Knot Software Ltd

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/
#pragma once

#include "Mutex.h"

namespace Cactus
{

	/** 
	* 
	* \brief Base functionality for IteratorWrappers
	*
	* 
	* \param T a Container like vector list map ...
	* \param IteratorType  T::iterator or T::const_iterator
	* \param ValType  T::mapped_type in case of a map, T::value_type for vector, list,...
	* 
	* have a look at VectorIteratorThreadWrapper and MapIteratorThreadWrapper for a concrete usage
	*/
	template <typename T, typename IteratorType, typename ValType, typename MutexType>
	class IteratorWrapperThread
	{

	private:
		/// Private constructor since only the parameterized constructor should be used
		IteratorWrapperThread();

	protected:
		IteratorType mBegin;
		IteratorType mCurrent;
		IteratorType mEnd;

	public:

		/// type you expect to get by functions like peekNext(Value)
		typedef ValType ValueType;
		/// type you expect to get by functions like peekNext(Value)Ptr
		typedef ValType* PointerType;

		/**
		\brief typedef to fulfill container interface

		Useful if you want to use BOOST_FOREACH
		\note there is no distinction between const_iterator and iterator.
		\n keep this in mind if you want to derivate from this class. 
		*/
		typedef IteratorType iterator;

		/**
		\brief typedef to fulfill container interface

		Useful if you want to use BOOST_FOREACH
		\note there is no distinction between const_iterator and iterator.
		\n keep this in mind if you want to derivate from this class. 
		*/
		typedef IteratorType const_iterator;


		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/
		IteratorWrapperThread ( IteratorType start, IteratorType end, MutexType& mutex)
			: mBegin( start ), mCurrent ( start ), mEnd ( end ), _mutex(mutex)
		{
			_mutex.Lock();
		}

		~IteratorWrapperThread()
		{
			_mutex.Unlock();
		}


		/** Returns true if there are more items in the collection. */
		bool hasMoreElements ( ) const
		{
			return mCurrent != mEnd;
		}

		/** Moves the iterator on one element. */
		void moveNext (  )
		{
			++mCurrent;
		}

		/** bookmark to the begin of the underlying collection */
		const IteratorType& begin() {return mBegin;}


		/** full access to the current  iterator */
		IteratorType& current(){return mCurrent;}

		/** bookmark to the end (one behind the last element) of the underlying collection */
		const IteratorType& end() {return mEnd;}

		private:
			MutexType& _mutex;
	};



	/** 
	* 
	* \brief prepared IteratorWrapperThread for container like std::vector 
	*
	* 
	* \param T = Container eg vector 
	* \param IteratorType  T::iterator or T::const_iterator
	* 
	* have a look at VectorIterator and ConstVectorThreadIterator for a more concrete usage
	*/
	template <typename T, typename IteratorType, typename MutexType>
	class VectorIteratorThreadWrapper : public IteratorWrapperThread<T, IteratorType, typename  T::value_type, MutexType>
	{
	public:
		typedef typename IteratorWrapperThread<T, IteratorType, typename  T::value_type, MutexType>::ValueType ValueType ; 
		typedef typename IteratorWrapperThread<T, IteratorType, typename  T::value_type, MutexType>::PointerType PointerType ;


		/**
		* \brief c'tor
		* 
		* Constructor that provide a start and end iterator to initialize.
		* 
		* @param start start iterator 
		* @param end end iterator 
		*/
		VectorIteratorThreadWrapper ( IteratorType start, IteratorType end, MutexType& mutex )
			: IteratorWrapperThread<T, IteratorType, typename T::value_type, MutexType>( start, end, mutex ) 
		{
		}


		/** Returns the next(=current) element in the collection, without advancing to the next. */
		ValueType peekNext (  ) const
		{
			return *(this->mCurrent);
		}

		/** Returns a pointer to the next(=current) element in the collection, without advancing to the next afterwards. */
		PointerType peekNextPtr (  )  const
		{
			return &(*(this->mCurrent) );
		}

		/** Returns the next(=current) value element in the collection, and advances to the next. */
		ValueType getNext (  ) 
		{
			return *(this->mCurrent++);
		}	

	};


	/** 
	* 
	* \brief Concrete IteratorWrapperThread for nonconst access to the underlying container
	* 
	* \param T  Container 
	* 
	*/
	template <typename T, typename MutexType = ThreadMutex>
	class VectorIteratorThread : public VectorIteratorThreadWrapper<T,  typename T::iterator, MutexType>{
	public:
		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/	
		VectorIteratorThread( typename T::iterator start, typename T::iterator end, MutexType& mutex )
			: VectorIteratorThreadWrapper<T,  typename T::iterator, MutexType>(start , end, mutex )
		{
		}

		/** Constructor.
		@remarks
		Provide a container to initialize.
		*/
		explicit VectorIteratorThread( T& c, MutexType& mutex )
			: VectorIteratorThreadWrapper<T,  typename T::iterator> ( c.begin(), c.end(), mutex )
		{
		}

	};

	/** 
	* 
	* \brief Concrete IteratorWrapperThread for const access to the underlying container
	*
	* 
	* \param T = Container 
	* 
	*/
	template <typename T, typename MutexType = ThreadMutex>
	class ConstVectorThreadIterator : public VectorIteratorThreadWrapper<T,  typename T::const_iterator, MutexType>{
	public:
		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/	
		ConstVectorThreadIterator( typename T::const_iterator start, typename T::const_iterator end, MutexType& mutex )
			: VectorIteratorThreadWrapper<T,  typename T::const_iterator, MutexType> (start , end, mutex )
		{
		}

		/** Constructor.
		@remarks
		Provide a container to initialize.
		*/
		explicit ConstVectorThreadIterator ( const T& c, MutexType& mutex )
			: VectorIteratorThreadWrapper<T,  typename T::const_iterator, MutexType> (c.begin() , c.end(), mutex)
		{
		}
	};



	/** 
	* 
	* \brief Prepared IteratorWrapperThread for key-value container
	*
	* 
	* \param T  Container  (map - or also set )
	* \param  IteratorType T::iterator or T::const_iterator
	* 
	* have a look at MapIterator and ConstMapIterator for a concrete usage
	*/
	template <typename T, typename IteratorType, typename MutexType>
	class MapIteratorThreadWrapper  : public IteratorWrapperThread<T, IteratorType, typename T::mapped_type, MutexType>
	{

	public:
		/// redefined ValueType for a map/set
		typedef typename IteratorWrapperThread<T, IteratorType, typename  T::mapped_type, MutexType>::ValueType ValueType ; 
		/// redefined PointerType for a map/set
		typedef typename IteratorWrapperThread<T, IteratorType, typename  T::mapped_type, MutexType>::PointerType PointerType ;	

		///unused, just to make it clear that map/set::value_type is not a ValueType
		typedef typename T::value_type PairType ; 
		/// type you expect to get by functions like peekNextKey
		typedef typename T::key_type KeyType;

		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/
		MapIteratorThreadWrapper ( IteratorType start, IteratorType end, MutexType& mutex )
			: IteratorWrapperThread<T, IteratorType, typename T::mapped_type, MutexType>( start, end, mutex ) 
		{
		}

		/** Returns the next(=current) key element in the collection, without advancing to the next. */
		KeyType peekNextKey(void) const
		{
			return this->mCurrent->first;
		}


		/** Returns the next(=current) value element in the collection, without advancing to the next. */
		ValueType peekNextValue (  ) const
		{
			return this->mCurrent->second;
		}


		/** Returns a pointer to the next/current value element in the collection, without 
		advancing to the next afterwards. */
		const PointerType peekNextValuePtr (  )  const
		{
			return &(this->mCurrent->second);
		}


		/** Returns the next(=current) value element in the collection, and advances to the next. */
		ValueType getNext()
		{
			return ((this->mCurrent++)->second) ;
		}	


	};




	/** 
	* 
	* \brief Concrete IteratorWrapperThread for nonconst access to the underlying key-value container
	*
	* 
	* \param T key-value container
	* 
	*/
	template <typename T, typename MutexType = ThreadMutex>
	class MapIteratorThread : public MapIteratorThreadWrapper<T,  typename T::iterator, MutexType>{
	public:

		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/	
		MapIteratorThread( typename T::iterator start, typename T::iterator end, MutexType& mutex )
			: MapIteratorThreadWrapper<T,  typename T::iterator, MutexType>(start , end, mutex )
		{
		}

		/** Constructor.
		@remarks
		Provide a container to initialize.
		*/
		explicit MapIteratorThread( T& c, MutexType& mutex )
			: MapIteratorThreadWrapper<T,  typename T::iterator, MutexType> ( c.begin(), c.end(), mutex )
		{
		}

	};


	/** 
	* 
	* \brief Concrete IteratorWrapperThread for const access to the underlying key-value container
	*
	* 
	* \param T key-value container
	* 
	*/
	template <typename T, typename MutexType = ThreadMutex>
	class ConstMapIterator : public MapIteratorThreadWrapper<T,  typename T::const_iterator, MutexType>{
	public:

		/** Constructor.
		@remarks
		Provide a start and end iterator to initialize.
		*/	
		ConstMapIterator( typename T::const_iterator start, typename T::const_iterator end, MutexType& mutex )
			: MapIteratorThreadWrapper<T,  typename T::const_iterator, MutexType> (start , end, mutex )
		{
		}

		/** Constructor.
		@remarks
		Provide a container to initialize.
		*/
		explicit ConstMapIterator ( const T& c, MutexType& mutex )
			: MapIteratorThreadWrapper<T,  typename T::const_iterator, MutexType> (c.begin() , c.end(), mutex )
		{
		}
	};
}
