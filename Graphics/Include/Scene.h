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

		/// ��ӽڵ㵽������
		void NodeAdded(SharedPtr<Node> node);
		//	�ӳ������Ƴ��ڵ�
		void NodeRemoved(SharedPtr<Node> node);

		SharedPtr<Node> FindSceneNode(UINT64 nodeUid);

		typedef std::unordered_map<UINT64, SharedPtr<Node>>  SCENE_NODE_MAP;
		typedef std::unordered_map<UINT64, SharedPtr<Node>>::iterator  SCENE_NODE_MAP_ITERATOR;
		typedef std::unordered_map<UINT64, SharedPtr<Node>>::const_iterator  SCENE_NODE_MAP_CITERATOR;


	protected:

		std::unordered_map<UINT64, SharedPtr<Node>>  m_localNodes;  //�����������еĽڵ�

	};

}

