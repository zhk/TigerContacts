/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			ResourceProvider.h
*	Description:
*			
*	Revision history:
*			2007-7-13	* Created by Zhang Kun
*				
===============================================================================*/
#pragma once

#include "DataContainer.h"
#include "Singleton.h"

namespace Cactus
{
	/*!
	\brief
	Abstract class that defines the required interface for all resource provider sub-classes.

	A ResourceProvider is used to load both XML and binary data from an external source.  This could be from a filesystem or the resource manager of a specific renderer.
	*/
	class COMMONUTIL_API ResourceProvider : public Object
	{
	public:

		ResourceProvider(){}

		virtual ~ResourceProvider(void) { }

		/*!
		\brief
		Load raw binary data.

		\param filename
		Cactus::String containing a filename of the resource to be loaded.

		\param output
		Reference to a RawDataContainer object to load the data into.

		\param resourceGroup
		Optional Cactus::String that may be used by implementations to identify the group from
		which the resource should be loaded.
		*/
		virtual void loadRawDataContainer(const Cactus::String& filename, RawDataContainer& output, const Cactus::String& resourceGroup) = 0;

		/*!
		\brief
		Unload raw binary data. This gives the resource provider a change to unload the data
		in its own way before the data container object is destroyed.  If it does nothing,
		then the object will release its memory.

		\param data
		Reference to a RawDataContainer object that is about to be destroyed.

		*/
		virtual void unloadRawDataContainer(RawDataContainer& data)  {}

		/*!
		\brief
		Return the current default resource group identifier.

		\return
		Cactus::String object containing the currently set default resource group identifier.
		*/
		const Cactus::String&   getDefaultResourceGroup(void) const     { return _strDefaultResourceGroup; }

		/*!
		\brief
		Set the default resource group identifier.

		\param resourceGroup
		Cactus::String object containing the default resource group identifier to be used.

		\return
		Nothing.
		*/
		void    setDefaultResourceGroup(const Cactus::String& resourceGroup)    { _strDefaultResourceGroup = resourceGroup; }

	protected:
		Cactus::String  _strDefaultResourceGroup;     //!< Default resource group identifier.
	};

	class COMMONUTIL_API ResourceProviderManager : public Singleton<ResourceProviderManager>
	{
	public:
		ResourceProviderManager(ResourceProvider* p = 0);
		~ResourceProviderManager();

		ResourceProvider*	getResourceProvider() const;

	private:

		ResourceProvider*	_pResourceProvider;
	};
}