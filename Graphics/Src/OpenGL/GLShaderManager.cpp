#include "GLShaderManager.h"
#include "IGraphicDriver.h"
#include "ResourceCache.h"
#include "ResourceLoader.h"
#include "GLShader.h"


namespace Sapphire
{

	GLShaderManager::GLShaderManager(Core* pcore, IGraphicDriver* pDriver) :BaseObject(pcore), m_pGraphicDriver(pDriver)
	{
		 
	}

	GLShaderManager::~GLShaderManager()
	{

	}

	Sapphire::IShaderVariation* GLShaderManager::GetShader(ShaderType type, const char* name, const char* defines, bool bAsync)
	{
		ResourceCache* cache = m_pCore->GetSubSystem<ResourceCache>();
		ResourceLoader* loader = m_pCore->GetSubSystem<ResourceLoader>();
		m_assert(cache && m_pGraphicDriver && loader);
		GLShader*  pShader = NULL;
		if (cache)
		{
			Path path = m_pGraphicDriver->GetShaderPath();
			path.addTailSlash();
			path += name;
			path += ".shader";
			BaseResource* pRes = cache->GetResource(path.c_str());
			//要判断加载完成
			if (pRes)
			{
				if (pRes->GetType() == ResourceType_Shader && pRes->GetState() == ResourceState::ResourceState_Loaded)
				{
					pShader = dynamic_cast<GLShader*>(pRes);
					return pShader->GetVariation(type, defines);
				}
			}
			else
			{
				//異步加載
				//交由资源加载器去异步加载
				pShader = new GLShader(m_pCore, path.c_str());
				loader->LoadResource(pShader, bAsync);
				if (!bAsync)
					return pShader->GetVariation(type, defines);
				
			}
			
		}
		return NULL;
	}

	 

}