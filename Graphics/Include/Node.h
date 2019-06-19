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
		//����ʱִ��
		virtual void		OnDestory(){};

		SharedPtr<Node>     GetChild(int index = 0) const;   
		SharedPtr<Node>     GetChild(const char* name) const;
		SharedPtr<Node>     GetChild(const std::string& name) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		bool				AddChild(SharedPtr<Node> child);
		bool				RemoveChild(const char* childName);
		bool				RemoveChild(int index);
		void				RemoveAllChildren();
		void				Remove(); //�Ӹ��ڵ�ɾ�������û�����ûᵼ�¶���ɾ��
		void				SetParent(SharedPtr<Node> parent);
		WeakPtr<Node>		GetParent() const;



		UINT				GetInstanceID() const { return m_UID; };
		bool				IsActive() const { return m_bActive; }
		void				SetActive(bool val) { m_bActive = val; }
		void				SetDeepActive(bool enable);  //Ӱ�쵽���еĺ��ӽڵ�
		std::string			GetNodeName() const { return m_nodeName; }
		void				SetNodeName(std::string val);
		const StringHash&	 GetNameHash() const { return m_nodeNameHash; }

		//��ȡ���
		SharedPtr<Component>			 GetComponent(ComponentType type) const;
		bool							 HasComponent(ComponentType type) const;
		int								 GetComponentsNum() const;
		void                             AddComponent(SharedPtr<Component> component);
		void                             RemoveComponent(ComponentType type);
		void						     RemoveComponent(SharedPtr<ComponentType> component);
		


	protected:

		WeakPtr<Node>						m_parent;
		std::vector<SharedPtr<Node>>	    m_children;  // ������ѭ�����ã�
		std::map<ushort, SharedPtr<Component>>   m_componentMap;    //ÿ�����͵����ֻ����һ��

		std::string							m_nodeName;  //�ڵ�����
		StringHash							m_nodeNameHash;
		//�Ƿ񼤻�
		bool								m_bActive;
		NodeType							m_eNodeType;
		UINT								m_UID;   //id�ö����ַ����ʾID

	};

}