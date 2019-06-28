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
			if (recursive)//�Ƿ�ݹ�
			{
				child->GetAllChildren(dest, recursive);
			}
			dest.push_back(child);
		}
	}

	bool Node::AddChild(SharedPtr<Node> child, uint index)
	{
		
		//�ǿգ��������Լ��� ��������Ѿ���ӹ���
		if (child.Null() || child == this || child->GetParent() == this)
			return false;

		WeakPtr<Node> parent = m_parent;
		while (!parent.Expired())
		{
			if (parent == child) //Ҫ��ӵĺ��Ӳ������Լ��ĸ��ڵ�
				return false;
			parent = parent->GetParent();//������еĸ��ڵ�
		}

		//��麢�ӵľɸ��׽ڵ�
		WeakPtr<Node> oldParent = child->GetParent();
		if (!oldParent.Expired())
		{
			//����ɸ��׵ĳ����͵�ǰ������һ��
			if (oldParent->GetScene() != m_scene)
				oldParent->RemoveChild(child);
			else
			{
				if (m_scene)
				{
					//�����¼�
					VariantVector eventData;
					eventData.push_back(Variant(m_scene));
					eventData.push_back(Variant(oldParent));
					eventData.push_back(Variant(child));
					m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_REMOVE, &eventData);  //���͸��¼�
				}

				oldParent->RemoveChild(child); //�Ӿɸ��������Ƴ�
			}

		}

		//׼����Ӻ��ӽڵ�
		m_children.push_back(child);
		if (m_scene && child->GetScene() != m_scene)
			m_scene->NodeAdded(child); //����ӵ���������ڵ����


		child->m_parent = this;
		child->MarkDirty();

		if (m_scene)
		{

			VariantVector eventData;
			eventData.push_back(Variant(m_scene));
			eventData.push_back(Variant(this));
			eventData.push_back(Variant(child));

			m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODEADDED, &eventData);  //���͸��¼�
		}
		return true;
		 
	}


	bool Node::RemoveChild(Node* node)
	{
		if (!node)
			return false;

		for (std::vector<SharedPtr<Node> >::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			//�Ӻ��ӽڵ����ҵ�����ڵ�
			if (*it == node)
			{
				SharedPtr<Node> child = *it;
				std::vector<SharedPtr<Node> >::iterator dIt = it;
				++it;
				if (Refs() > 0 && m_scene)
				{
					
					//���ͽڵ�ɾ���¼��¼�
					VariantVector eventData;
					eventData.push_back(Variant(m_scene));
					eventData.push_back(Variant(this));
					eventData.push_back(Variant(child));
					m_scene->FireEvent(ET_SCENE_EVENT, EVENT_SCENE_NODE_REMOVE, &eventData);
				}

				child->m_parent.Reset();
				child->MarkDirty(); //���Ϊ��
				if (m_scene)
					m_scene->NodeRemoved(child);  //�ӳ����Ĺ���Ľڵ�����Ƴ�
				m_children.erase(dIt);		//���û������ã��ͻ�ɾ��
			}
		}
		return true;
	}

	void Node::RemoveAllChildren()
	{
		RemoveChildren(true); //�ݹ��Ƴ������ӽڵ����ӽڵ�
	}

	void Node::RemoveChildren(bool recursive)
	{
		unsigned numRemoved = 0;
		std::vector<SharedPtr<Node>>  deleteList = m_children; //�ȸ���һ�ݶ���ɾ������ó���

		for (unsigned i = deleteList.size() - 1; i < deleteList.size(); --i)
		{
			bool remove = false;
			Node* childNode = deleteList[i];

			if (recursive)
			{
				childNode->RemoveChildren(true);//���Ƴ��ú��ӽڵ��µ����ӽڵ�
				remove = true; 
			}
				

			if (remove)
			{
				RemoveChild(childNode); //���Ƴ��ú��ӽڵ�
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
		if (m_parent.Expired()) //�Ƿ����
			m_parent->RemoveChild(this);
	}

	void Node::SetParent(Node* parent)
	{
		if (!m_parent.Expired())
		{
			 TransformComponent* transform = GetComponent<TransformComponent>();
			 TransformComponent* parentTransform = m_parent->GetComponent<TransformComponent>();

			Matrix3x4 oldWorldTransform = transform->GetWorldTransform();
			SharedPtr<Node> curNode = m_scene->FindSceneNode(this->m_UID); //ÿ���ڵ㶼������ӵ�������ͳһ����
			parent->AddChild(curNode);

			if (parent !=  m_scene)
			{
				Matrix3x4 newTransform = parentTransform->GetWorldTransform().Inverse() * oldWorldTransform;
				transform->SetTransform(newTransform.GetTranslation(), newTransform.GetRotation(), newTransform.GetScale());
			}
			else
			{
				// ���ڵ㣬������ת������ռ���
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
			RemoveListener(it->second); //����Ǽ���������Ƴ���
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
		//�Ƴ����еĺ��ӽڵ�����
		RemoveAllComponent();
		m_componentMap.clear();
		RemoveAllChildren();
		m_children.clear();
	}

}