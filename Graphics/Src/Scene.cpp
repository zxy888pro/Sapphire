#include "Scene.h"



namespace Sapphire
{

	Scene::Scene(Core* pCore) :Node(pCore)
	{
		m_eNodeType = NodeType_Scene;
	}

	void Scene::NodeAdded(SharedPtr<Node> node)
	{
		//不能是Scene节点自身
		if (!node || node->GetScene() == this)
			return;

		//先从就场景移除这个节点
		Scene* oldScene = node->GetScene();
		if (oldScene)
			oldScene->NodeRemoved(node);

		node->SetScene(this); //设置到新场景

		unsigned id = node->GetInstanceID();
		if (!id)
		{
			SCENE_NODE_MAP_ITERATOR it = m_localNodes.find(node->GetInstanceID());
			if (it != m_localNodes.end() && it->second != node)
			{
				//id相同？节点不同
				SAPPHIRE_LOGWARNING(StringFormat("Overwriting node with Type %d", (int)node->GetInstanceID()));
				NodeRemoved(it->second);

			}
			m_localNodes[node->GetInstanceID()] = node;
		}
		 
		const std::vector<SharedPtr<Node>>& children = node->GetChildren();
		for (std::vector<SharedPtr<Node> >::const_iterator it = children.begin(); it != children.end(); ++it)
			NodeAdded(*it); //依次递归将所有子节点也纳入管理

	}

	void Scene::NodeRemoved(SharedPtr<Node> node)
	{
		//不能删除Scene节点自身
		if (!node || node->GetScene() != this)
			return;

		unsigned id = node->GetInstanceID();
		m_localNodes.erase(id); //从节点表中移除该节点
			
		//重置一下节点
		node->ResetScene();
		
	}

	Sapphire::SharedPtr<Sapphire::Node> Scene::FindSceneNode(UINT64 nodeUid)
	{
		SharedPtr<Node> ret;
		SCENE_NODE_MAP_ITERATOR it =  m_localNodes.find(nodeUid);
		if (it != m_localNodes.end())
			ret = it->second;
		return ret;
		
	}

}