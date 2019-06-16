#include "Node.h"


namespace Sapphire
{



	void Node::SetNodeName(std::string val)
	{
		if (m_nodeName != val)
		{
			m_nodeName = val;
			m_nodeNameHash = val; //¿½±´¹¹Ôìº¯Êý
		}
		//
		
	}

}