#include "Scene.h"



namespace Sapphire
{

	Scene::Scene(Core* pCore) :Node(pCore)
	{
		m_eNodeType = NodeType_Scene;
	}

	void Scene::NodeAdded(SharedPtr<Node> node)
	{
		//������Scene�ڵ�����
		if (!node || node->GetScene() == this)
			return;

		//�ȴӾͳ����Ƴ�����ڵ�
		Scene* oldScene = node->GetScene();
		if (oldScene)
			oldScene->NodeRemoved(node);

		node->SetScene(this); //���õ��³���

		unsigned id = node->GetInstanceID();
		if (!id)
		{
			SCENE_NODE_MAP_ITERATOR it = m_localNodes.find(node->GetInstanceID());
			if (it != m_localNodes.end() && it->second != node)
			{
				//id��ͬ���ڵ㲻ͬ
				SAPPHIRE_LOGWARNING(StringFormat("Overwriting node with Type %d", (int)node->GetInstanceID()));
				NodeRemoved(it->second);

			}
			m_localNodes[node->GetInstanceID()] = node;
		}
		 
		const std::vector<SharedPtr<Node>>& children = node->GetChildren();
		for (std::vector<SharedPtr<Node> >::const_iterator it = children.begin(); it != children.end(); ++it)
			NodeAdded(*it); //���εݹ齫�����ӽڵ�Ҳ�������

	}

	void Scene::NodeRemoved(SharedPtr<Node> node)
	{
		//����ɾ��Scene�ڵ�����
		if (!node || node->GetScene() != this)
			return;

		unsigned id = node->GetInstanceID();
		m_localNodes.erase(id); //�ӽڵ�����Ƴ��ýڵ�
			
		//����һ�½ڵ�
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