#pragma once
#include "Sapphire.h"
#include "Component.h"

namespace Sapphire
{
	enum NodeType
	{
		NodeType_Node,
		NodeType_SceneNode, //�����ڵ�
		NodeType_UIElement,  //UIԪ�ؽڵ�
		NodeType_Scene,    //�����������ڵ�
		NodeType_Canvas,   //UI���ڵ�
		NodeType_MaxCount
	};

	class Scene;

	class SAPPHIRE_CLASS Node : public BaseObject
	{
		SAPPHIRE_OBJECT(Node, BaseObject)
	public:

		Node(Core* pCore);
		virtual ~Node();

		virtual void		OnCreate(){};		
		//��ʱִ��
		virtual void		OnEnable(){};
		//�ر�ʱִ��
		virtual void		OnDisable(){};
		//����֮ǰִ��
		virtual void		OnDestory(){};  

		SharedPtr<Node>     GetChild(int index = 0) const;   
		SharedPtr<Node>     GetChild(const char* name) const;
		SharedPtr<Node>     GetChild(const std::string& name) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		void                GetAllChildren(std::vector<SharedPtr<Node>>& dest, bool recursive = false);
		bool				AddChild(SharedPtr<Node> child, uint index = 0);
		bool                RemoveChild(Node* node); //�Ӻ��ӽڵ����ҵ�node���Ƴ�
		void				RemoveAllChildren();  //�Ƴ����еĺ��ӽڵ�
		void				RemoveChildren(bool recursive);
		uint				GetNumChildren(bool recursive = false); //�Ƿ�ݹ��ȡ���к�����Ŀ
		void				Remove(); //�Ӹ��ڵ�ɾ�������û�����ûᵼ�¶���ɾ��
		void				SetParent(Node* parent);
		WeakPtr<Node>		GetParent() const;

		//���ó������ڵ�, ��Scene����
		void SetScene(Scene* scene);
		//���ó���,ID��������  ��Scene����
		void ResetScene();

		Scene* GetScene() const { return m_scene; }

		UINT64				GetInstanceID() const { return m_UID; };
		bool				IsActive() const { return m_bActive; }
		void				SetActive(bool val) { m_bActive = val; }
		void				SetDeepActive(bool enable);  //Ӱ�쵽���еĺ��ӽڵ�
		std::string			GetNodeName() const { return m_nodeName; }
		void				SetNodeName(std::string val);
		const StringHash&	 GetNameHash() const { return m_nodeNameHash; }


		//��ǽڵ���ӽڵ�Ϊ�࣬��Ҫ��д��������ռ�任�� ֪ͨ���м������
		void MarkDirty();
		//�Ƿ�Ϊ��
		bool IsDirty() const { return m_bDirty; }
		void ClearDirtyFlag() { m_bDirty = false; }

		//��ȡ���
		SharedPtr<Component>			 GetComponent(ComponentType type) const;
		bool							 HasComponent(ComponentType type) const;
		int								 GetNumComponents() const;
		void                             AddComponent(SharedPtr<Component> component);
		void                             RemoveComponent(ComponentType type);
		void							 RemoveAllComponent();   //�Ƴ��������
		

		void							 AddListener(SharedPtr<Component> component);   //����һ�����������
		void							 RemoveListener(SharedPtr<Component> component);   //����һ�����������
		virtual void                     Destroy();  //���ٽڵ㣬��������
		
		template <class T>  T*		 GetComponent() const;

		typedef std::map<ushort, SharedPtr<Component>>  NODE_COMPONENT_MAP;
		typedef std::map<ushort, SharedPtr<Component>>::iterator  NODE_COMPONENT_MAP_ITERATOR;
		typedef std::map<ushort, SharedPtr<Component>>::const_iterator  NODE_COMPONENT_MAP_CITERATOR;

		typedef std::vector<SharedPtr<Node>>						NODE_CHILDREN_LIST;
		typedef std::vector<SharedPtr<Node>>::iterator				NODE_CHILDREN_LIST_ITERATOR;
		typedef std::vector<SharedPtr<Node>>::const_iterator		NODE_CHILDREN_LIST_CITERATOR;

	protected:

		WeakPtr<Node>						m_parent;
		std::vector<SharedPtr<Node>>	    m_children;  // ������ѭ�����ã�
		std::vector<WeakPtr<Component>>   m_listeners;    //�ڵ������
		std::map<ushort, SharedPtr<Component>>   m_componentMap;    //ÿ�����͵����ֻ����һ��

		std::string							m_nodeName;  //�ڵ�����
		StringHash							m_nodeNameHash;
		//�Ƿ񼤻�
		bool								m_bActive;
		NodeType							m_eNodeType;
		UINT64								m_UID;   //id�ö����ַ����ʾID
		Scene*								m_scene;   //�������ڵ�
	    mutable bool						m_bDirty;   //���־
	};


	template <class T>
	 T* Sapphire::Node::GetComponent() const
	{
		for (std::map<ushort, SharedPtr<Component>>::const_iterator it = m_componentMap.begin(); it != m_componentMap.end(); it++)
		{
			
			T* component = dynamic_cast<T*>(it->second.Get());
			if (component)
				return component;
		}
		return NULL;
	}

}