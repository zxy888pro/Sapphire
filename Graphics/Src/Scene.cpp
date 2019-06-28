#include "Scene.h"



namespace Sapphire
{

	void Scene::NodeAdded(SharedPtr<Node> node)
	{

	}

	void Scene::NodeRemoved(SharedPtr<Node> node)
	{
		//不能删除Scene节点自身
		if (!node || node->GetScene() != this)
			return;

		unsigned id = node->GetInstanceID();
		m_localNodes.erase(id); //从节点表中移除该节点
			

		node->ResetScene();

		
	}

}