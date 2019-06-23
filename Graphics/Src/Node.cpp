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

	 

	Sapphire::WeakPtr<Node> Node::GetParent() const
	{
		 
		return m_parent;
	}

	void Node::SetNodeName(std::string val)
	{
		if (m_nodeName != val)
		{
			m_nodeName = val;
			m_nodeNameHash = val; //�������캯��
		}
		//���ͽڵ��������¼�
		FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_RENAME, this);		
	}

	void Node::MarkDirty()
	{
		Node *cur = this;
		for (;;)
		{
			// ǰ��:
			// a) ÿ��һ���ڵ㱻���Ϊ�࣬ ���������ӽڵ㶼����Ϊ��
			// b) ÿ��һ���ڵ���������ǣ��������ĸ���Ҳ���뱻����
			// ��ˣ�����ݹ�ı�ǽڵ�Ϊ�࣬��ô���еĺ��ӽڵ�Ҳ����Ϊ�࣬���Ҳ���Ҫ�����ٱ������
			if (cur->m_bDirty)
				return;
			cur->m_bDirty = true;

			// ����֪ͨ����������� Ȼ���ٱ���ӽڵ�
			for (std::vector<WeakPtr<Component>>::iterator i = cur->m_listeners.begin(); i != cur->m_listeners.end();)
			{
				Component *c = *i;
				if (c)
				{
					c->OnMarkedDirty(cur); //��ǽڵ�����
					++i;
				}
				// ������������ڣ����б��Ƴ����ǣ�����β����Ԫ�ؽ���������O(n^2)�Ŀ�����
				else
				{
					*i = cur->m_listeners.back();
					cur->m_listeners.pop_back();
				}
			}

			// �Ż�:��Ҫ�ݹ�ȥ��ǵ�һ������Ϊ�࣬�ĵ���ǰ������������ж���һ���ĺ��ӣ����������ĵݹ�
			std::vector<SharedPtr<Node>>::iterator i = cur->m_children.begin();
			if (i != cur->m_children.end())
			{
				Node *next = *i;
				for (++i; i != cur->m_children.end(); ++i)
					(*i)->MarkDirty();
				cur = next;
			}
			else
				return;
		}
	}

}