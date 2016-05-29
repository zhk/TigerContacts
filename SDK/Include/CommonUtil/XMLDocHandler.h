/*===============================================================================
*	Copyright (C) 2006-2011, Zhang Kun(zhk.tiger@gmail.com). All Rights Reserved.
*
*	File name:
*			XMLDocHandler.h
*	Description:
*			A simple way to read xml document(From CEGUI.)
*	Revision history:
*			2006-7-13	* Created by Zhang Kun
*				Created
===============================================================================*/
#pragma once

#include "PreInclude.h"

#if M_XML_USE_TINYXML
#	include "tinyxml/tinyxml.h"
#else
#	include "rapidxml/rapidxml.hpp"
#endif

namespace Cactus
{
	//----------------------------------------------------------------------------------------------
	class COMMONUTIL_API XMLAttributes : public Cactus::Object
	{
		friend COMMONUTIL_API std::ostream&	operator<<(std::ostream& s, const XMLAttributes& str);

	public:
		XMLAttributes();
		virtual ~XMLAttributes();

		void			add(const Cactus::String& attrName, const Cactus::String& attrValue);
		void			remove(const Cactus::String& attrName);
		bool			exists(const Cactus::String& attrName) const;
		size_t			getCount(void) const;
		Cactus::String	getName(size_t index) const;
		Cactus::String	getValue(size_t index) const;
		Cactus::String	getValue(const Cactus::String& attrName) const;
		Cactus::String	getValueAsString(const Cactus::String& attrName, const Cactus::String& def = "") const;
		bool			getValueAsBool(const Cactus::String& attrName, bool def = false) const;
		int				getValueAsInteger(const Cactus::String& attrName, int def = 0) const;
		float			getValueAsFloat(const Cactus::String& attrName, float def = 0.0f) const;


	protected:
		typedef Cactus::map<Cactus::String, Cactus::String>::type AttributeMap;
		AttributeMap    d_attrs;
	};

	COMMONUTIL_API std::ostream&	operator<<(std::ostream& s, const XMLAttributes& str);


	//----------------------------------------------------------------------------------------------
	class COMMONUTIL_API XMLHandler : public Cactus::Object
	{
		friend class XMLParser;
	public:
		XMLHandler(){}
		virtual void elementStart(const Cactus::String& element, const XMLAttributes& attributes) = 0;
		virtual void elementEnd(const Cactus::String& element) = 0;
		virtual void text(const Cactus::String& element, const Cactus::String& content) = 0;

		// root/childElement/childElement/
		bool	currentElementMatch(const Cactus::String& path);

	protected:
		Cactus::StringVector	_elements;
	};

	//----------------------------------------------------------------------------------------------
	class COMMONUTIL_API XMLParser : public Cactus::Object
	{
	public:
		XMLParser(XMLHandler& handler, const Cactus::String& filename, const Cactus::String& schemaName);
		~XMLParser(){}
	protected:

#if M_XML_USE_TINYXML
		void processElement(const TiXmlElement* element);
#else
		void processElement(const rapidxml::xml_node<>* element);
#endif

	private:
		XMLHandler* d_handler;
	};
}
