#pragma once

#include "Sapphire.h"
#include "SceneNode.h"

namespace Sapphire
{

	//�����ڵ㣬���������ĸ�
	class SAPPHIRE_CLASS Scene :public Node
	{
		SAPPHIRE_OBJECT(Scene, Node)
	public:

		Scene(Core* pCore);
		virtual ~Scene();

		/// �ڵ�����ӣ�������ָ�벢����ӵ�id map
		void NodeAdded(SharedPtr<Node> node);
		void NodeRemoved(SharedPtr<Node> node);

		SharedPtr<Node> FindSceneNode(UINT64 nodeUid);

	protected:

		std::unordered_map<UINT64, SharedPtr<Node>>  m_localNodes;  //�����������еĽڵ�

	};

}

