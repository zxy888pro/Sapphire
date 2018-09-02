#include "stdafx.h"
#include "BaseLightMesh.h"
#include <MemoryMgr.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"

namespace Sapphire
{

	BaseLightMesh::BaseLightMesh()
	{
		m_lightPos = glm::vec3(0, 0, 0);
	}

	BaseLightMesh::~BaseLightMesh()
	{

	}

	void BaseLightMesh::Init()
	{
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
		m_pShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		m_pShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
		m_pShader->setVec3("lightPos", m_lightPos);
		m_pShader->setVec3("viewPos", pCam->getPosition());


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

	void BaseLightMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

	void BaseLightMesh::BackupRenderState()
	{
		m_state.LoadCurRenderState();
		
	}

	void BaseLightMesh::RestoreRenderState()
	{
		m_state.SetState();
	}

}