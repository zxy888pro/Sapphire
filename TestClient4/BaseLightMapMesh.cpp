#include "BaseLightMapMesh.h"
#include <Sapphire.h>
#include <MemoryMgr.h>
#include <Graphics.h>
#include <IGraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "Camera.h"
#include "BaseLight.h"


namespace Sapphire
{

	BaseLightMapMesh::BaseLightMapMesh()
	{
		m_type = MT_BaseLightMapMesh;
		m_pDriver = dynamic_cast<IGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
	}

	BaseLightMapMesh::~BaseLightMapMesh()
	{

	}

	void BaseLightMapMesh::Init()
	{
		LoadBaseShader("shaders\\BaseLightMapMesh2Vs.glsl", "shaders\\BaseLightMapMesh2Fs.glsl");

		m_vertices = _New float[288]{
			// positions          // normals   // texture coords
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		glGenVertexArrays(1, &m_mVao);


		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 288, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);


	}

	void BaseLightMapMesh::Render()
	{
		if (!m_pShader)
		{
			LogUtil::LogMsgLn("ShaderProgram 未能初始化！ 可能加载错误!");
			return;
		}
		Camera* pCam = Camera::GetSingletonPtr();
		if (pCam == NULL)
		{
			return;
		}
		m_pShader->Use();
		BackupRenderState();
		glEnable(GL_DEPTH_TEST);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
		glBindTexture(GL_TEXTURE_2D, m_diffMapObj);
		// bind specular map
		glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
		glBindTexture(GL_TEXTURE_2D, m_sepcMapObj);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		m_pShader->UnUse();
		RestoreRenderState();
	}

	void BaseLightMapMesh::SetDiffuseMap(const char* fileName)
	{
		 
		m_assert(m_pDriver);
		ITextureMgr* pTexMgr = m_pDriver->getTextureMgr();
		m_assert(pTexMgr);
		ITexture* pTexture = pTexMgr->CreateTexture2DFromFile(fileName);

		if (pTexture == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Load %s Failed!", fileName));
		}
		else
		{
			m_pShader->Use();
			pTexMgr->SetTexture(pTexture, TU_DIFFUSE);
			m_diffMapObj = pTexture->getUID();
			m_pShader->setInt("material.diffuse", TU_DIFFUSE);
		}

	}

	void BaseLightMapMesh::SetSepcularMap(const char* fileName)
	{
	 
		m_assert(m_pDriver);
		ITextureMgr* pTexMgr = m_pDriver->getTextureMgr();
		m_assert(pTexMgr);
		ITexture* pTexture = pTexMgr->CreateTexture2DFromFile(fileName);
		if (pTexture == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Load %s Failed!", fileName));
		}
		else
		{
			m_pShader->Use();
			pTexMgr->SetTexture(pTexture, TU_SPECULAR);
			m_sepcMapObj = pTexture->getUID();
			m_pShader->setInt("material.specular", TU_SPECULAR);
		}
	}

	void BaseLightMapMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		std::vector<SharedPtr<BaseLight>> pointLights;

		for (int i = 0; i < lightVec.size(); ++i)
		{
			if (lightVec[i]->getType() == LT_POINT)
			{
				if (pointLights.size() >= 8)
				{
					break;
				}
				pointLights.push_back(lightVec[i]);
			}
		}
		m_pShader->Use();
		//动态设置光源数目
		if (pointLights.size() > 0)
		{
			m_pShader->setInt("pointLightCount", pointLights.size());
			std::string tmp;
			for (int i = 0; i < pointLights.size(); ++i)
			{
				tmp = StringFormatA("pointLights[%d].position", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getPos());
				tmp = StringFormatA("pointLights[%d].direction", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getDirection());
				tmp = StringFormatA("pointLights[%d].constant", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getConstant());
				tmp = StringFormatA("pointLights[%d].linear", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getLinear());
				tmp = StringFormatA("pointLights[%d].quadratic", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getQuadratic());
				tmp = StringFormatA("pointLights[%d].ambient", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getAmbient());
				tmp = StringFormatA("pointLights[%d].diffuse", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getDiffuse());
				tmp = StringFormatA("pointLights[%d].specular", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getSpecular());
			}
		}
		Camera* pCam = Camera::GetSingletonPtr();
		if (pCam == NULL)
		{
			return;
		}
		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = pCam->GetViewMatrix();
		m_pShader->setVec3("viewPos", pCam->getPosition());
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, m_pos);
		m_pShader->setMat4("model", model);
		m_pShader->setFloat("material.shininess", m_shinniess);
		m_pShader->UnUse();

	}

	void BaseLightMapMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

}