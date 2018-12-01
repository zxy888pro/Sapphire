#include "BaseUIMesh.h"
#include "Camera.h"
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>

namespace Sapphire
{

	BaseUIMesh::BaseUIMesh()
	{
		m_type = MT_UI;
	}

	BaseUIMesh::~BaseUIMesh()
	{

	}

	void BaseUIMesh::Init()
	{
		LoadBaseShader("shaders\\TestGSShaderVs.glsl", "shaders\\TestGSShaderFs.glsl", "shaders\\TestGSShaderGs.glsl");
		m_vertices = _New float[20]{
			-0.5f, 0.5f, 1.0f, 0.0f, 0.0f, // top-left
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, // top-right
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
			-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
		};

		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 20, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void BaseUIMesh::Render()
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
		//glEnable(GL_DEPTH_TEST);

		//glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		//glm::mat4 view = pCam->GetViewMatrix();
		//m_pShader->setMat4("projection", projection);
		//m_pShader->setMat4("view", view);
		//glm::mat4 model;
		//model = glm::translate(model, m_pos);
		//m_pShader->setMat4("model", model);
		//m_pShader->setVec3("Color", m_color);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);
		m_pShader->UnUse();

		RestoreRenderState();
	}


}