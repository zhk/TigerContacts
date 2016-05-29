/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			MemoryAllocatorConfig.h
*	Description:
*			
*	Revision history:
*			2010-7-13	* Created by Zhang Kun
*				Refactor from OGRE1.71
===============================================================================*/
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

#ifndef __MemoryAllocatorConfig_H__
#define __MemoryAllocatorConfig_H__

#pragma once

// Anything that has done a #define new <blah> will screw operator new definitions up so undefine.
#ifdef new
#  undef new
#endif

#ifdef delete
#  undef delete
#endif


#include "MemoryAllocators.h"

#if Ogre_Memory_Allocator == Ogre_MemAlloc_NedPooling

namespace Cactus
{
	// configure default allocators based on the options above
	// notice how we're not using the memory categories here but still roughing them out
	// in your allocators you might choose to create different policies per category

	// configurable for general malloc
	// notice how we ignore the category here, you could specialise
	class GeneralAllocPolicy : public NedPoolingPolicy{};
	template <size_t align> class MemAlignAllocPolicy : public NedPoolingAlignedPolicy<align>{};
}

#elif Ogre_Memory_Allocator == Ogre_MemAlloc_Ned

namespace Cactus
{
	// configure default allocators based on the options above
	// notice how we're not using the memory categories here but still roughing them out
	// in your allocators you might choose to create different policies per category

	// configurable for general malloc
	// notice how we ignore the category here, you could specialise
	class GeneralAllocPolicy : public NedAllocPolicy{};
	template <size_t align> class MemAlignAllocPolicy : public NedAlignedAllocPolicy<align>{};
}

#elif Ogre_Memory_Allocator == Ogre_MemAlloc_std

namespace Cactus
{
	// configure default allocators based on the options above
	// notice how we're not using the memory categories here but still roughing them out
	// in your allocators you might choose to create different policies per category

	// configurable for general malloc
	// notice how we ignore the category here
	class GeneralAllocPolicy : public StdAllocPolicy{};
	template <size_t align> class MemAlignAllocPolicy : public StdAlignedAllocPolicy<align>{};


}

#else
	
// your allocators here?

#endif

// Util functions
namespace Cactus
{
	/** \addtogroup Core
	*  @{
	*/
	/** \addtogroup Memory
	*  @{
	*/

	/** Utility function for constructing an array of objects with placement new,
		without using new[] (which allocates an undocumented amount of extra memory
		and so isn't appropriate for custom allocators).
	*/
	template<typename T>
	T* constructN(T* basePtr, size_t count)
	{
		for (size_t i = 0; i < count; ++i)
		{
			new ((void*)(basePtr+i)) T();
		}
		return basePtr;
	}
	/** @} */
	/** @} */

}
// define macros 

/** \addtogroup Core
*  @{
*/
/** \addtogroup Memory
*  @{
*/

#if _DEBUG


//继承于AllocatedObject的类对象分配内存和回收
// new / delete for classes deriving from AllocatedObject (alignment determined by per-class policy).
// Can only be used with classes that derive from AllocatedObject since customised new/delete needed.
#	define Mem_New		new (__FILE__, __LINE__, __FUNCTION__)
#	define Mem_Delete	delete



//为一般用途的数据块分配内存(如unsigned char*, void*)
/// Allocate a block of raw memory.
#	define Mem_Alloc(bytes) \
	::Cactus::GeneralAllocPolicy::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)

/// Allocate a block of memory for a primitive type.
#	define Mem_Alloc_T(T, count) \
	static_cast<T*>(::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))

/// Free the memory allocated with Mem_Alloc or Mem_Alloc_T.
#	define Mem_Free(ptr) \
	::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr)



//为不是继承于AllocatedObject的类对象分配内存
/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters.
#	define Mem_New_T(T) \
	new (::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T

/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject
#	define Mem_New_Array_T(T, count) \
	::Cactus::constructN(static_cast<T*>(::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 

/// Free the memory allocated with Mem_New_T.
#	define Mem_Delete_T(ptr, T) \
	if(ptr) \
	{ \
		(ptr)->~T(); \
		::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr); \
	}

/// Free the memory allocated with Mem_New_Array_T. uses count and type to call destructor.
#	define Mem_Delete_Array_T(ptr, T, count) \
	if(ptr) \
	{ \
		for (size_t b = 0; b < count; ++b) \
		{ \
			(ptr)[b].~T(); \
		} \
		::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr); \
	}


//为不是继承于AllocatedObject的类对象分配内存, 支持特定的内存对齐
/// Allocate a block of raw memory aligned to user defined boundaries.
#	define Mem_Alloc_Align(bytes, align) \
	::Cactus::MemAlignAllocPolicy<align>::allocateBytes(bytes, __FILE__, __LINE__, __FUNCTION__)

/// Allocate a block of memory for a primitive type aligned to user defined boundaries.
#	define Mem_Alloc_T_Align(T, count, align) \
	static_cast<T*>(::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__))

/// Free the memory allocated with either Mem_Alloc_Align or Mem_Alloc_T_Align.
#	define Mem_Free_Align(ptr, align) \
	::Cactus::MemAlignAllocPolicy<align>::deallocateBytes(ptr)

/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries
#	define Mem_New_T_Align(T, align) \
	new (::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T), __FILE__, __LINE__, __FUNCTION__)) T

/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries
/// - do not use for classes that inherit from AllocatedObject
#	define Mem_New_Array_T_Align(T, count, align) \
	::Cactus::constructN(static_cast<T*>(::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T)*(count), __FILE__, __LINE__, __FUNCTION__)), count) 

/// Free the memory allocated with Mem_New_T_Align.
#	define Mem_Delete_T_Align(ptr, T, align) \
	if(ptr) \
	{ \
		(ptr)->~T(); \
		::Cactus::MemAlignAllocPolicy<align>::deallocateBytes(ptr); \
	}

/// Free the memory allocated with Mem_New_Array_T_Align, use count and type to call destructor.
#	define Mem_Delete_Array_T_Align(ptr, T, count, align) \
	if(ptr) \
	{ \
		for (size_t _b = 0; _b < count; ++_b) \
		{ \
			(ptr)[_b].~T(); \
		} \
		::Cactus::MemAlignAllocPolicy<align>::deallocateBytes(ptr); \
	}



#else // !_DEBUG

/// new / delete for classes deriving from AllocatedObject (alignment determined by per-class policy).
#	define Mem_New		new 
#	define Mem_Delete	delete

/// Allocate a block of raw memory.
#	define Mem_Alloc(bytes)			::Cactus::GeneralAllocPolicy::allocateBytes(bytes)
/// Allocate a block of memory for a primitive type.
#	define Mem_Alloc_T(T, count)	static_cast<T*>(::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with Mem_Alloc or Mem_Alloc_T.
#	define Mem_Free(ptr)			::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr)

/// Allocate space for one primitive type, external type or non-virtual type with constructor parameters.
#	define Mem_New_T(T)							new (::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types - do not use for classes that inherit from AllocatedObject.
#	define Mem_New_Array_T(T, count)			::Cactus::constructN(static_cast<T*>(::Cactus::GeneralAllocPolicy::allocateBytes(sizeof(T)*(count))), count) 
/// Free the memory allocated with Mem_New_T.
#	define Mem_Delete_T(ptr, T)					if(ptr){(ptr)->~T(); ::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr);}
/// Free the memory allocated with Mem_New_Array_T, use count and type to call destructor.
#	define Mem_Delete_Array_T(ptr, T, count)	if(ptr){for (size_t b = 0; b < count; ++b) { (ptr)[b].~T();} ::Cactus::GeneralAllocPolicy::deallocateBytes((void*)ptr);}

// aligned allocation
/// Allocate a block of raw memory aligned to user defined boundaries.
#	define Mem_Alloc_Align(bytes, align)		::Cactus::MemAlignAllocPolicy<align>::allocateBytes(bytes)
/// Allocate a block of memory for a primitive type aligned to user defined boundaries.
#	define Mem_Alloc_T_Align(T, count, align)	static_cast<T*>(::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T)*(count)))
/// Free the memory allocated with either Mem_Alloc_Align or Mem_Alloc_T_Align.
#	define Mem_Free_Align(ptr, align)			::Cactus::MemAlignAllocPolicy<align>::deallocateBytes((void*)ptr)

/// Allocate space for one primitive type, external type or non-virtual type aligned to user defined boundaries.
#	define Mem_New_T_Align(T, align)						new (::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T))) T
/// Allocate a block of memory for 'count' primitive types aligned to user defined boundaries - do not use for classes that inherit from AllocatedObject.
#	define Mem_New_Array_T_Align(T, count, align)			::Cactus::constructN(static_cast<T*>(::Cactus::MemAlignAllocPolicy<align>::allocateBytes(sizeof(T)*(count))), count) 
/// Free the memory allocated with Mem_New_T_Align.
#	define Mem_Delete_T_Align(ptr, T, align)				if(ptr){(ptr)->~T(); ::Cactus::MemAlignAllocPolicy<align>::deallocateBytes((void*)ptr);}
/// Free the memory allocated with Mem_New_Array_T_Align, uses count and type to call destructor.
#	define Mem_Delete_Array_T_Align(ptr, T, count, align)	if(ptr){for (size_t _b = 0; _b < count; ++_b) { (ptr)[_b].~T();} ::Cactus::MemAlignAllocPolicy<align>::deallocateBytes((void*)ptr);}

#endif // _DEBUG


namespace Cactus
{
	/** Function which invokes Mem_Delete on a given pointer. 
	@remarks
		Useful to pass custom deletion policies to external libraries (e. g. boost).
	*/
	template<typename T>
	void deletePtr(T* ptr)
	{
		Mem_Delete ptr;
	}
}

/** @} */
/** @} */

#endif
