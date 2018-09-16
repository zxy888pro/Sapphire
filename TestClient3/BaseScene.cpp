#include "Sapphire.h"
#include "BaseScene.h"

namespace Sapphire
{

	Scene::Scene()
	{

	}

	Scene::~Scene()
	{
		for (int i = m_lights.size() - 1; i >= 0; --i)
		{
			m_lights.erase(m_lights.begin() + i);
		}
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.begin();
		if (it != m_meshMap.end())
		{
			std::map<std::string, SharedPtr<BaseMesh>>::iterator _it = it;
			++it;
			m_meshMap.erase(_it);
		}
		
	}

	void Scene::Upate()
	{
		for (int i = 0; i < m_lights.size(); ++i)
		{
			m_lights[i]->Update();
		}
		
	}

	void Scene::Render()
	{
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.begin();
		while (it != m_meshMap.end())
		{
			it->second->Render();
			++it;
		}
	}

	int Scene::GetLightCount()
	{
		return m_lights.size();
	}

	bool Scene::AddLight(SharedPtr<BaseLight> pLight)
	{
		pLight->Init();
		m_lights.push_back(pLight);
		return true;
	}

	bool Scene::AddMesh(std::string name, SharedPtr<BaseMesh> pMesh)
	{
		std::pair<std::string, SharedPtr<BaseMesh>> it(name, pMesh);
		pMesh->Init();
		m_meshMap.insert(it);
		return true;
	}

	bool Scene::RemoveLight(int index)
	{
		if (index < m_lights.size())
		{
			m_lights.erase(m_lights.begin() + index);	
			return true;
		}
		return false;

	}

	bool Scene::RemoveLight(SharedPtr<BaseLight> pLight)
	{
		for (int i = 0; i < m_lights.size(); ++i)
		{
			if (m_lights[i] == pLight)
			{
				m_lights.erase(m_lights.begin() + i);
			}
		}
		return true;
	}

	bool Scene::RemoveMesh(std::string name)
	{
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.find(name);
		if (it != m_meshMap.end())
		{
			m_meshMap.erase(it);
			return true;
		}
		return false;
	}

}