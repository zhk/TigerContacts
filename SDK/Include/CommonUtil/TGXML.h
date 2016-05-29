#pragma once

#include "PreInclude.h"
#include "tinyxml/tinyxml.h"

namespace Cactus
{
	class COMMONUTIL_API GXMLAttribute : public Object
	{
		friend class GXMLDoc;
		friend class GXMLElement;
	public:
		GXMLAttribute( TiXmlAttribute * element = 0 );
		GXMLAttribute(const GXMLAttribute& src);
		virtual ~GXMLAttribute();
		Cactus::String		Name();
		Cactus::String		Value();
		bool HasNext();
		bool Valid() const;
		GXMLAttribute Next();
	private:
		TiXmlAttribute* _pAttribute;
	};

	class COMMONUTIL_API GXMLElement : public Object
	{
		friend class GXMLDoc;
	public:

		GXMLElement( TiXmlElement * element = 0 );
		GXMLElement(const GXMLElement& src);
		virtual ~GXMLElement();
		GXMLElement& operator=(const GXMLElement& src);
		GXMLElement FirstChild();
		GXMLElement NextSibling();
		GXMLElement FirstChildByName(const Cactus::String& childName);
		GXMLElement NextSiblingByName(const Cactus::String& childName);
		GXMLElement Parent();
		Cactus::String		Name();
		Cactus::String		Value();
		bool		IsNull();
		Cactus::String		GetAttribute(const Cactus::String& attrName);	
		bool		HasChildNodes() const;
		bool		HasAttributes() const;
		GXMLAttribute FirstAttribute();
	private:
		TiXmlElement* _pElement;
	};

	class COMMONUTIL_API GXMLDoc : public Object
	{
	public:
		GXMLDoc();
		virtual ~GXMLDoc();
		// Load from disk file.
		bool Load(const Cactus::String& file);
		// From default FileSystem.
		bool LoadFromFS(const Cactus::String& path);
		bool LoadFromXML(const char* pBuffer);
		GXMLElement GetRoot();
		bool LoadFromXML(const Cactus::String& xmlCode);

	private:
		TiXmlDocument *_doc;
	};

	float COMMONUTIL_API	GetFloatAttribuate(const Cactus::String& attrName, GXMLElement& element, float defaultVal = 0.0f);
	int   COMMONUTIL_API	GetIntAttribuate(const Cactus::String& attrName,  GXMLElement&  element, int defaultVal = 0);
	bool  COMMONUTIL_API	GetBoolAttribuate(const Cactus::String& attrName, GXMLElement& element, bool defaultVal = false);

}