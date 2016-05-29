/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			CommonUtilConfig.h
*	Description:
*			
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

// Build type
#define M_RETAIL_VERSION			0

// TinyXML π”√stl string
#define TIXML_USE_STL

// Use TinyXML or RapidXML
#define M_XML_USE_TINYXML			0

// define the memory allocator configuration to use
#define Ogre_MemAlloc_std			1
#define Ogre_MemAlloc_Ned			2
#define Ogre_MemAlloc_User			3
#define Ogre_MemAlloc_NedPooling	4

#ifndef Ogre_Memory_Allocator
#  define Ogre_Memory_Allocator Ogre_MemAlloc_NedPooling
#endif

#define Ogre_SIMD_Alignment			16

#define Ogre_MemAlloc_Use_Tracker	1
#if !_DEBUG && Ogre_MemAlloc_Use_Tracker
#	undef	Ogre_MemAlloc_Use_Tracker
#	define Ogre_MemAlloc_Use_Tracker	0
#endif

//Strings and containers
#define Ogre_String_Use_MemAlloc	1
#define Ogre_Container_Use_MemAlloc 1