#pragma once
#include "Sapphire.h"

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

		virtual void OnCreate(){};
		//Updateǰִ��һ��
		virtual void OnStart(){};
		//ÿִ֡��
		virtual void Update() {};
		//�̶�ʱ��ִ��
		virtual void FixedUpdate() {};
		//��ʱִ��
		virtual void OnEnable(){};
		//�ر�ʱִ��
		virtual void OnDisable(){};
		//����ʱִ��
		virtual void OnDestory(){};

		SharedPtr<Node> GetChild(int index = 0) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		bool AddChild(SharedPtr<Node> child);
		bool RemoveChild(const char* childName);
		bool RemoveChild(int index);
		void RemoveAllChildren();
		void Remove(); //�Ӹ��ڵ�ɾ�������û�����ûᵼ�¶���ɾ��
		void SetParent(SharedPtr<Node> parent);
		UINT GetInstanceID() const { return m_UID; };
		bool IsActive() const { return m_bActive; }
		void SetActive(bool val) { m_bActive = val; }
		void SetDeepActive(bool enable);  //Ӱ�쵽���еĺ��ӽڵ�
		std::string GetNodeName() const { return m_nodeName; }
		void SetNodeName(std::string val);

		const StringHash& GetNameHash() const { return m_nodeNameHash; }

	protected:

		WeakPtr<Node>						m_parent;
		std::vector<SharedPtr<Node>>	    m_children;  // ������ѭ�����ã�

		std::string							m_nodeName;  //�ڵ�����
		StringHash							m_nodeNameHash;
		//�Ƿ񼤻�
		bool								m_bActive;
		NodeType							m_eNodeType;
		UINT								m_UID;   //id�ö����ַ����ʾID

	};

}