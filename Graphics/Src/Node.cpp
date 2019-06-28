#include "SceneEventDef.h"
#include "Node.h"
#include "Variant.h"
#include "Scene.h"
#include "TransformComponent.h"


namespace Sapphire
{



	Node::Node(Core* pCore) :BaseObject(pCore),
		m_nodeName("defaultNode"),
		m_eNodeType(NodeType_Node),
		m_nodeNameHash(StringHash(m_nodeName)),
		m_parent(NULL),
		m_bActive(false)
	{
		assert(pCore);
		m_UID = pCore->GenUID();
	}

	Node::~Node()
	{
		OnDestory();
		Destroy();
	}

	 

	Sapphire::SharedPtr<Node> Node::GetChild(int index /*= 0*/) const
	{
		SharedPtr<Node> ret;
		if (index >= m_children.size() || index < 0)
			return ret;
		return m_children[index];
	}

	Sapphire::SharedPtr<Node> Node::GetChild(const char* name) const
	{
		SharedPtr<Node> child;
		for (std::vector<SharedPtr<Node>>::const_iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			child = *it;
			if (child->GetNodeName() == name)
			{
				return child;
			}
		}
 
		return child;
	}

	Sapphire::SharedPtr<Node> Node::GetChild(const std::string& name) const
	{
		SharedPtr<Node> child;
		for (std::vector<SharedPtr<Node>>::const_iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			child = *it;
			if (child->GetNodeName() == name)
			{
				return child;
			}
		}
		return child;

	}

	void Node::GetAllChildren(std::vector<SharedPtr<Node>>& dest, bool recursive /*= false*/)
	{
		dest.clear();
		for (std::vector<SharedPtr<Node>>::const_iterator it = m_children.begin(); it != m_children.end(); it++)
		{
			SharedPtr<Node> child = *it;
			if (recursive)//是否递归
			{
				child->GetAllChildren(dest, recursive);
			}
			dest.push_back(child);
		}
	}

	bool Node::AddChild(SharedPtr<Node> child, uint index)
	{
		
		//非空，不能是自己， 不能添加已经添加过的
		if (child.Null() || child == this || child->GetParent() == this)
			return false;

		WeakPtr<Node> parent = m_parent;
		while (!parent.Expired())
		{
			if (parent == child) //要添加的孩子不能是自己的父节点
				return false;
			parent = parent->GetParent();//检查所有的父节点
		}

		//检查孩子的旧父亲节点
		WeakPtr<Node> oldParent = child->GetParent();
		if (!oldParent.Expired())
		{
			//如果旧父亲的场景和当前场景不一致
			if (oldParent->GetScene() != m_scene)
				oldParent->RemoveChild(child);
			else
			{
				if (m_scene)
				{
					//发送事件
					VariantVector eventData;
					eventData.push_back(Variant(m_scene));
					eventData.push_back(Variant(oldParent));
					eventData.push_back(Variant(child));
					m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_REMOVE, &eventData);  //发送个事件
				}

				oldParent->RemoveChild(child); //从旧父亲那里移除
			}

		}

		//准备添加孩子节点
		m_children.push_back(child);
		if (m_scene && child->GetScene() != m_scene)
			m_scene->NodeAdded(child); //再添加到场景管理节点表中


		child->m_parent = this;
		child->MarkDirty();

		if (m_scene)
		{

			VariantVector eventData;
			eventData.push_back(Variant(m_scene));
			eventData.push_back(Variant(this));
			eventData.push_back(Variant(child));

			m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODEADDED, &eventData);  //发送个事件
		}
		return true;
		 
	}


	bool Node::RemoveChild(Node* node)
	{
		if (!node)
			return false;

		for (std::vector<SharedPtr<Node> >::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			//从孩子节点中找到这个节点
			if (*it == node)
			{
				SharedPtr<Node> child = *it;
				std::vector<SharedPtr<Node> >::iterator dIt = it;
				++it;
				if (Refs() > 0 && m_scene)
				{
					
					//发送节点删除事件事件
					VariantVector eventData;
					eventData.push_back(Variant(m_scene));
					eventData.push_back(Variant(this));
					eventData.push_back(Variant(child));
					m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_REMOVE, &eventData);
				}

				child->m_parent.Reset();
				child->MarkDirty(); //标记为脏
				if (m_scene)
					m_scene->NodeRemoved(child);  //从场景的管理的节点表中移除
				m_children.erase(dIt);		//如果没别的引用，就会删除
			}
		}
		return true;
	}

	void Node::RemoveAllChildren()
	{
		RemoveChildren(true); //递归移除所有子节点孙子节点
	}

	void Node::RemoveChildren(bool recursive)
	{
		unsigned numRemoved = 0;
		std::vector<SharedPtr<Node>>  deleteList = m_children; //先复制一份对照删除表，免得出错

		for (unsigned i = deleteList.size() - 1; i < deleteList.size(); --i)
		{
			bool remove = false;
			Node* childNode = deleteList[i];

			if (recursive)
			{
				childNode->RemoveChildren(true);//先移除该孩子节点下的孙子节点
				remove = true; 
			}
				

			if (remove)
			{
				RemoveChild(childNode); //再移除该孩子节点
				++numRemoved;
			}
		}
	}

	uint Node::GetNumChildren(bool recursive /*= false*/)
	{
		if (!recursive)
			return m_children.size();
		else
		{
			unsigned allChildren = m_children.size();
			for (std::vector<SharedPtr<Node> >::const_iterator i = m_children.begin(); i != m_children.end(); ++i)
				allChildren += (*i)->GetNumChildren(true);

			return allChildren;
		}
	}

	void Node::Remove()
	{
		if (m_parent.Expired()) //是否过期
			m_parent->RemoveChild(this);
	}

	void Node::SetParent(Node* parent)
	{
		if (!m_parent.Expired())
		{
			 TransformComponent* transform = GetComponent<TransformComponent>();
			 TransformComponent* parentTransform = m_parent->GetComponent<TransformComponent>();

			Matrix3x4 oldWorldTransform = transform->GetWorldTransform();
			SharedPtr<Node> curNode = m_scene->FindSceneNode(this->m_UID); //每个节点都必须添加到场景中统一管理
			parent->AddChild(curNode);

			if (parent !=  m_scene)
			{
				Matrix3x4 newTransform = parentTransform->GetWorldTransform().Inverse() * oldWorldTransform;
				transform->SetTransform(newTransform.GetTranslation(), newTransform.GetRotation(), newTransform.GetScale());
			}
			else
			{
				// 根节点，不必在转换世界空间了
				transform->SetTransform(oldWorldTransform.GetTranslation(), oldWorldTransform.GetRotation(), oldWorldTransform.GetScale());
			}
		}
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
		m_UID = 0;
		m_scene = NULL;
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
		SharedPtr<Component> ret;
		return ret;
	}

	bool Node::HasComponent(ComponentType type) const
	{
		return false;
	}

	int Node::GetNumComponents() const
	{
		return 0;
	}

	void Node::AddComponent(SharedPtr<Component> component)
	{

	}

	void Node::RemoveComponent(ComponentType type)
	{
		NODE_COMPONENT_MAP_ITERATOR it = m_componentMap.find(type);
		if (it != m_componentMap.end())
		{
			it->second->Remove();
		}
	}

	void Node::RemoveAllComponent()
	{
		for (NODE_COMPONENT_MAP_ITERATOR it = m_componentMap.begin(); it != m_componentMap.end(); it++)
		{
			it->second->Remove();
			RemoveListener(it->second); //如果是监听组件，移除它
		}
	}

	void Node::AddListener(SharedPtr<Component> component)
	{

	}

	void Node::RemoveListener(SharedPtr<Component> component)
	{

	}

	void Node::Destroy()
	{
		//移除所有的孩子节点和组件
		RemoveAllComponent();
		m_componentMap.clear();
		RemoveAllChildren();
		m_children.clear();
	}

}