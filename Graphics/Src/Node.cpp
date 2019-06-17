#include "SceneEventDef.h"
#include "Node.h"


namespace Sapphire
{



	Node::Node(Core* pCore) :BaseObject(pCore),
		m_nodeName("defaultNode"),
		m_eNodeType(NodeType_Node),
		m_nodeNameHash(StringHash(m_nodeName)),
		m_parent(NULL),
		m_bActive(false)
	{
		m_UID = reinterpret_cast<UINT>(this);
	}

	Node::~Node()
	{
		OnDestory();
	}

	void Node::SetNodeName(std::string val)
	{
		if (m_nodeName != val)
		{
			m_nodeName = val;
			m_nodeNameHash = val; //拷贝构造函数
		}
		//发送节点重命名事件
		FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_RENAME, this);		
	}

}