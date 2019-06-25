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
		//非空，不能是自己， 不能添加已经添加过的
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
				//发送事件
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
			m_nodeNameHash = val; //拷贝构造函数
		}
		//发送节点重命名事件
		FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_RENAME, this);		
	}

	void Node::MarkDirty()
	{
		Node *cur = this;
		for (;;)
		{
			// 前提:
			// a) 每当一个节点被标记为脏， 所有他的子节点都会标记为脏
			// b) 每当一个节点清理掉脏标记，所有他的父亲也必须被清理
			// 因此，如果递归的标记节点为脏，那么所有的孩子节点也必须为脏，并且不需要重新再标记它们
			if (cur->m_bDirty)
				return;
			cur->m_bDirty = true;

			// 首先通知监听器组件， 然后再标记子节点
			for (std::vector<WeakPtr<Component>>::iterator i = cur->m_listeners.begin(); i != cur->m_listeners.end();)
			{
				Component *c = *i;
				if (c)
				{
					c->OnMarkedDirty(cur); //标记节点脏了
					++i;
				}
				// 如果监听器到期，从列表移除它们，（与尾部的元素交换，避免O(n^2)的开销）
				else
				{
					*i = cur->m_listeners.back();
					cur->m_listeners.pop_back();
				}
			}

			// 优化:不要递归去标记第一个孩子为脏，改到当前函数处理，如果有多于一个的孩子，会产生多余的递归
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