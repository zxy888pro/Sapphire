#include "Sapphire.h"
#include <ITexture.h>
#include "BaseScene.h"
#include "BaseRenderTarget.h"
#include "BaseRenderSurface.h"
#include "SkyBoxMesh.h"


namespace Sapphire
{

	Scene::Scene() :
		m_bEnablePost(false),
		m_PostRtt(NULL)
	{
		
	}

	Scene::~Scene()
	{
		for (int i = m_lights.size() - 1; i >= 0; --i)
		{
			m_lights.erase(m_lights.begin() + i);
		}
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.begin();
		while (it != m_meshMap.end())
		{
			std::map<std::string, SharedPtr<BaseMesh>>::iterator _it = it;
			++it;
			m_meshMap.erase(_it);
		}
		m_PostRtt->Release();
		safeDelete(m_PostRtt);
	}

	void Scene::Initialize()
	{
		m_PostRtt = new BaseRenderTarget();
		m_PostRtt->Initialize(800, 600);
		m_PostRs = new BaseRenderSurface();
		m_PostRs->Init();
		m_PostRs->SetTexture(m_PostRtt->GetTexture(0)->getUID());
	}

	void Scene::Upate()
	{
		for (int i = 0; i < m_lights.size(); ++i)
		{
			m_lights[i]->Update();
		}
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.begin();
		while (it != m_meshMap.end())
		{
			it->second->Update(m_lights);
			++it;
		}

	}

	void Scene::Render()
	{
		//第一次渲染
		if (m_bEnablePost)
		{
			m_PostRtt->BindRT(); //绑定渲染目标，渲染到m_PostRtt的FBO里
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // 暂时不使用模板缓冲
		}
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.begin();
		while (it != m_meshMap.end())
		{
			it->second->Render();
			++it;
		}
		if (m_bEnablePost)
		{
			m_PostRtt->UnBindRT(); //恢复默认的FBO
			m_PostRs->SetTexture(m_PostRtt->GetTexture(0)->getUID()); //将m_postRtt的Fbo的内容作为纹理
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//后期渲染
			m_PostRs->Render(); //渲染到默认的FBO中
		}
		
	}

	void Scene::DrawPostEffect()
	{

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

	uint Scene::GetSkyBoxMap()
	{
		std::map<std::string, SharedPtr<BaseMesh>>::iterator it = m_meshMap.find("skybox");
		if (it != m_meshMap.end())
		{
			SharedPtr<SkyBoxMesh> pSMesh;
			pSMesh.DynamicCast(it->second);
			return pSMesh->GetSkyBoxUID();
		}
		return 0;
	}

	void Scene::SetSkyBox(std::string cubemapPath)
	{
		RemoveMesh("skybox");
		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new SkyBoxMesh());
		SharedPtr<SkyBoxMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		AddMesh("skybox", pMesh);
		pSMesh->LoadCubeMap(cubemapPath);
	}

}