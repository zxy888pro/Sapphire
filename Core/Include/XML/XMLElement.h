#pragma once

#include "Sapphire.h"


namespace pugi
{

	struct xml_node_struct;
	class xpath_node;
	class xpath_node_set;
	class xpath_query;
	class xpath_variable_set;

}


namespace Sapphire
{

	class XMLFile;
	class XPathQuery;
	class XPathResultSet;


	class SAPPHIRE_CLASS XMLElement
	{
	public:
		XMLElement();
		~XMLElement();
	};

}
