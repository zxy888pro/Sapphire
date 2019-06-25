#include "SceneEventDef.h"
#include "Node.h"
#include "Variant.h"


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

	 

	Sapphire::SharedPtr<Node> Node::GetChild(int index /*= 0*/) const
	{

	}

	Sapphire::SharedPtr<Node> Node::GetChild(const char* name) const
	{

	}

	Sapphire::SharedPtr<Node> Node::GetChild(const std::string& name) const
	{

	}

	bool Node::AddChild(SharedPtr<Node> child, uint index)
	{
		//�ǿգ��������Լ��� ��������Ѿ���ӹ���
		if (!child.Null() || child == this || child->GetParent() == this)
			return;

		WeakPtr<Node> parent = m_parent;
		while (!parent.Expired())
		{
			if (parent == child)
				return;
			parent = parent->GetParent();
		}

		 
	}

	bool Node::RemoveChild(const char* childName)
	{
		/* 
		for (std::vector<SharedPtr<Node>>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			 
		 
		}*/
		std::vector<SharedPtr<Node>>::iterator it = m_children.begin();
		 
	}

	bool Node::RemoveChild(int index)
	{

	}

	bool Node::RemoveChild(Node* node)
	{
		if (!node)
			return;

		for (std::vector<SharedPtr<Node> >::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			if (*it == node)
			{
				std::vector<SharedPtr<Node> >::iterator dIt = it;
				++it;
				//�����¼�
				/*VariantMap& eventData = GetEventDataMap();
				eventData[P_SCENE] = scene_;
				eventData[P_PARENT] = this;
				eventData[P_NODE] = child;

				scene_->SendEvent(E_NODEREMOVED, eventData);*/
				//m_scene->FireEvent()

				m_children.erase(dIt);
				//RemoveChild(*it);
				return;
			}
		}
	}

	void Node::RemoveAllChildren()
	{

	}

	uint Node::GetNumChildren(bool recursive /*= false*/)
	{
		if (m_parent)
			m_parent->RemoveChild(this);
	}

	void Node::Remove()
	{
		
	}

	void Node::SetParent(SharedPtr<Node> parent)
	{

	}

	Sapphire::WeakPtr<Node> Node::GetParent() const
	{
		 
		return m_parent;
	}

	void Node::SetScene(Scene* scene)
	{

	}

	void Node::ResetScene()
	{

	}

	void Node::SetDeepActive(bool enable)
	{

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

	Sapphire::SharedPtr<Sapphire::Component> Node::GetComponent(ComponentType type) const
	{

	}

	bool Node::HasComponent(ComponentType type) const
	{

	}

	int Node::GetNumComponents() const
	{

	}

	void Node::AddComponent(SharedPtr<Component> component)
	{

	}

	void Node::RemoveComponent(ComponentType type)
	{

	}

	void Node::RemoveComponent(SharedPtr<ComponentType> component)
	{

	}

	void Node::RemoveAllComponent()
	{

	}

	void Node::AddListener(SharedPtr<Component> component)
	{

	}

	void Node::RemoveListener(SharedPtr<Component> component)
	{

	}

}