#include "stdafx.h"
#include "BaseLight.h"
#include "BaseLightMesh.h"
#include "IGraphicDriver.h"
#include <MemoryMgr.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"

namespace Sapphire
{

	BaseLightMesh::BaseLightMesh()
	{
		m_type = MT_BaseLighMesh;
		
	}

	BaseLightMesh::~BaseLightMesh()
	{

	}

	void BaseLightMesh::Init()
	{
		LoadBaseShader("shaders\\BaseLight2Vs.glsl", "shaders\\BaseLight2Fs.glsl");
		m_vertices = _New float[216]
		{
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
		};

		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 216, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void BaseLightMesh::Render()
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


		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = pCam->GetViewMatrix();
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);
		glm::mat4 model;
		m_pShader->setMat4("model", model);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		//m_pShader->UnUse();
		RestoreRenderState();
	}

	void BaseLightMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
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
		m_pShader->setVec3("objectColor", m_color);
		m_pShader->UnUse();
	}

	void BaseLightMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}


}