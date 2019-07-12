#pragma once

#include "Sapphire.h"
#include "XML/XMLElement.h"

namespace pugi
{

	class xml_document;
	class xml_node;
	class xpath_node;

}

namespace Sapphire
{

	class SAPPHIRE_CLASS XMLFile : public BaseResource
	{
		SAPPHIRE_OBJECT(XMLFile, BaseResource)

	public: 
		XMLFile(Core* pcore);
		virtual ~XMLFile();

		/// Deserialize from a string. Return true if successful.
		bool FromString(const String& source);
		/// Clear the document and create a root element.
		XMLElement CreateRoot(const String& name);

		/// Return the root element, with optionally specified name. Return null element if not found.
		XMLElement GetRoot(const String& name = String::EMPTY);

		/// Return the pugixml document.
		pugi::xml_document* GetDocument() const { return document_; }

		/// Serialize the XML content to a string.
		String ToString(const String& indentation = "\t") const;

		/// Patch the XMLFile with another XMLFile. Based on RFC 5261.
		void Patch(XMLFile* patchFile);
		/// Patch the XMLFile with another XMLElement. Based on RFC 5261.
		void Patch(XMLElement patchElement);

	private:

		/// Add an node in the Patch.
		void PatchAdd(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		/// Replace a node or attribute in the Patch.
		void PatchReplace(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		/// Remove a node or attribute in the Patch.
		void PatchRemove(const pugi::xpath_node& original) const;

		/// Add a node in the Patch.
		void AddNode(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// Add an attribute in the Patch.
		void AddAttribute(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// Combine two text nodes.
		bool CombineText(const pugi::xml_node& patch, const pugi::xml_node& original, bool prepend) const;

		pugi::xml_document* document_;
	};

}