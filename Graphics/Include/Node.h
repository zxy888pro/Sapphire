#pragma once
#include "Sapphire.h"

namespace Sapphire
{
	class SAPPHIRE_CLASS Node : public BaseObject
	{
		SAPPHIRE_OBJECT(Node, BaseObject)
	public:

		Node(Core* pCore);
		virtual ~Node();

		virtual void Update(float deltaTick) {};
			

		SharedPtr<Node> GetChild(int index = 0) const;
		const std::vector<SharedPtr<Node>>& GetChildren() const { return m_children; }
		bool AddChild(Node* child);
		bool RemoveChild(Node* child);
		bool RemoveChild(int index);
		void RemoveAllChildren();
		void Remove(); //�Ӹ��ڵ�ɾ�������û�����ûᵼ�¶���ɾ��
		void SetParent(Node* parent);

		bool IsActive() const { return m_bActive; }
		void SetActive(bool val) { m_bActive = val; }

		std::string GetNodeName() const { return m_nodeName; }
		void SetNodeName(std::string val);

		const StringHash& GetNameHash() const { return m_nodeNameHash; }

	protected:

		WeakPtr<Node>  m_parent;
		std::vector<SharedPtr<Node>> m_children;  // ������ѭ�����ã�

		std::string m_nodeName;  //�ڵ�����
		StringHash  m_nodeNameHash;
		//�Ƿ񼤻�
		bool m_bActive;

	};

}