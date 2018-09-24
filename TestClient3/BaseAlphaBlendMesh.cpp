#include "BaseAlphaBlendMesh.h"
#include <Sapphire.h>
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "Camera.h"

namespace Sapphire
{

	BaseAlphaBlendMesh::BaseAlphaBlendMesh()
	{

	}

	BaseAlphaBlendMesh::~BaseAlphaBlendMesh()
	{

	}

	void BaseAlphaBlendMesh::Init()
	{
		LoadBaseShader("shaders\\BaseAlphaBlendVs.glsl", "shaders\\BaseAlphaBlendFs.glsl");
		m_vertices = _New float[30]{
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

			0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.5f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 30, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void BaseAlphaBlendMesh::Render()
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = pCam->GetViewMatrix();
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, m_pos);
		m_pShader->setMat4("model", model);



		glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
		glBindTexture(GL_TEXTURE_2D, m_diffMapObj);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_pShader->UnUse();
		RestoreRenderState();
	}

	void BaseAlphaBlendMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		 
	}

	void BaseAlphaBlendMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

	void BaseAlphaBlendMesh::BackupRenderState()
	{
		m_state.LoadCurRenderState();
	}

	void BaseAlphaBlendMesh::RestoreRenderState()
	{
		m_state.SetState();
	}

}