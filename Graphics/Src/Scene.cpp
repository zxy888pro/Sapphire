#include "Scene.h"



namespace Sapphire
{

	void Scene::NodeAdded(SharedPtr<Node> node)
	{

	}

	void Scene::NodeRemoved(SharedPtr<Node> node)
	{
		//����ɾ��Scene�ڵ�����
		if (!node || node->GetScene() != this)
			return;

		unsigned id = node->GetInstanceID();
		m_localNodes.erase(id); //�ӽڵ�����Ƴ��ýڵ�
			

		node->ResetScene();

		
	}

}