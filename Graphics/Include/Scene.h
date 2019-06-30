#pragma once

#include "Sapphire.h"
#include "SceneNode.h"

namespace Sapphire
{

	//场景节点，整个场景的根
	class SAPPHIRE_CLASS Scene :public Node
	{
		SAPPHIRE_OBJECT(Scene, Node)
	public:

		Scene(Core* pCore);
		virtual ~Scene();

		/// 添加节点到场景中
		void NodeAdded(SharedPtr<Node> node);
		//	从场景中移除节点
		void NodeRemoved(SharedPtr<Node> node);

		SharedPtr<Node> FindSceneNode(UINT64 nodeUid);

		typedef std::unordered_map<UINT64, SharedPtr<Node>>  SCENE_NODE_MAP;
		typedef std::unordered_map<UINT64, SharedPtr<Node>>::iterator  SCENE_NODE_MAP_ITERATOR;
		typedef std::unordered_map<UINT64, SharedPtr<Node>>::const_iterator  SCENE_NODE_MAP_CITERATOR;


	protected:

		std::unordered_map<UINT64, SharedPtr<Node>>  m_localNodes;  //管理场景里所有的节点

	};

}

