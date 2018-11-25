#include "stdafx.h"
#include "BaseUBOSimpleMesh.h"
#include <MemoryMgr.h>
#include "Camera.h"
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <gtc/type_ptr.hpp>

namespace Sapphire
{

	BaseUBOSimpleMesh::BaseUBOSimpleMesh()
	{
		m_type = MT_SimpleUBOMesh;
	}

	BaseUBOSimpleMesh::~BaseUBOSimpleMesh()
	{

	}

	void BaseUBOSimpleMesh::Init()
	{

		LoadBaseShader("shaders\\BaseUBOSimpleMeshVs.glsl", "shaders\\BaseUBOSimpleMeshFs.glsl");
		m_vertices = _New float[108]{
			-0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, -0.5f,
				0.5f, 0.5f, -0.5f,
				0.5f, 0.5f, -0.5f,
				-0.5f, 0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,

				-0.5f, -0.5f, 0.5f,
				0.5f, -0.5f, 0.5f,
				0.5f, 0.5f, 0.5f,
				0.5f, 0.5f, 0.5f,
				-0.5f, 0.5f, 0.5f,
				-0.5f, -0.5f, 0.5f,

				-0.5f, 0.5f, 0.5f,
				-0.5f, 0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, -0.5f,
				-0.5f, -0.5f, 0.5f,
				-0.5f, 0.5f, 0.5f,

				0.5f, 0.5f, 0.5f,
				0.5f, 0.5f, -0.5f,
				0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, 0.5f,
				0.5f, 0.5f, 0.5f,

				-0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, -0.5f,
				0.5f, -0.5f, 0.5f,
				0.5f, -0.5f, 0.5f,
				-0.5f, -0.5f, 0.5f,
				-0.5f, -0.5f, -0.5f,

				-0.5f, 0.5f, -0.5f,
				0.5f, 0.5f, -0.5f,
				0.5f, 0.5f, 0.5f,
				0.5f, 0.5f, 0.5f,
				-0.5f, 0.5f, 0.5f,
				-0.5f, 0.5f, -0.5f,
		};
		//m_color = glm::vec3(1.0, 1.0, 1.0);
		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//创建UBO
		//获取shader对象的Ubo对象索引
		unsigned int uniformBlockIndexRed = glGetUniformBlockIndex(m_pShader->getShaderProgram(), "Matrices");
		//绑定该对象到UBO缓冲区绑点0
		glUniformBlockBinding(m_pShader->getShaderProgram(), uniformBlockIndexRed, 0);
		//现在创建UBO对象
		m_uboObj;
		glGenBuffers(1, &m_uboObj);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboObj);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		//定义缓冲区大小，并链接到对象上
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboObj, 0, 2 * sizeof(glm::mat4));

 

	}

	void BaseUBOSimpleMesh::Render()
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

		glBindBuffer(GL_UNIFORM_BUFFER, m_uboObj);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//m_pShader->setMat4("projection", projection);
		//m_pShader->setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, m_pos);
		m_pShader->setMat4("model", model);
		m_pShader->setVec3("Color", m_color);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		m_pShader->UnUse();

		RestoreRenderState();
	}

	void BaseUBOSimpleMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
	     
	}

	void BaseUBOSimpleMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

}