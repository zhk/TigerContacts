/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			DefaultResourceProvider.h
*	Description:
*			
*	Revision history:
*			2007-7-13	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "ResourceProvider.h"
#include "StringHelper.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

namespace Cactus
{

	class COMMONUTIL_API DefaultResourceProvider : public ResourceProvider
	{
	public:

		DefaultResourceProvider() {}
		~DefaultResourceProvider(void) {}

		/*!
		\brief
		Set the directory associated with a given resource group identifier.

		\param resourceGroup
		The resource group identifier whose directory is to be set.

		\param directory
		The directory to be associated with resource group identifier
		\a resourceGroup

		\return
		Nothing.
		*/
		void setResourceGroupDirectory(const Cactus::String& resourceGroup, const Cactus::String& directory);

		/*!
		\brief
		Return the directory associated with the specified resource group
		identifier.

		\param resourceGroup
		The resource group identifier for which the associated directory is to
		be returned.

		\return
		Cactus::String object describing the directory currently associated with resource
		group identifier \a resourceGroup.

		\note
		This member is not defined as being const because it may cause
		creation of an 'empty' directory specification for the resourceGroup
		if the resourceGroup has not previously been accessed.
		*/
		const Cactus::String& getResourceGroupDirectory(const Cactus::String& resourceGroup);

		/*!
		\brief
		clears any currently set directory for the specified resource group
		identifier.

		\param resourceGroup
		The resource group identifier for which the associated directory is to
		be cleared.
		*/
		void clearResourceGroupDirectory(const Cactus::String& resourceGroup);

		void loadRawDataContainer(const Cactus::String& filename, RawDataContainer& output, const Cactus::String& resourceGroup);
		void unloadRawDataContainer(RawDataContainer& data);

	protected:
		/*!
		\brief
		Return the final path and filename, taking into account the given
		resource group identifier that should be used when attempting to
		load the data.
		*/
		Cactus::String getFinalFilename(const Cactus::String& filename, const Cactus::String& resourceGroup) const;

		typedef Cactus::map<Cactus::String, Cactus::String, FastLessCompare>::type ResourceGroupMap;
		ResourceGroupMap    _ResourceGroups;
	};
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

