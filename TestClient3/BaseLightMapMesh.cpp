#include "BaseLightMapMesh.h"
#include <Sapphire.h>
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "Camera.h"


namespace Sapphire
{

	BaseLightMapMesh::BaseLightMapMesh()
	{
		m_type = MT_BaseLightMapMesh;
	}

	BaseLightMapMesh::~BaseLightMapMesh()
	{

	}

	void BaseLightMapMesh::Init()
	{
			LoadBaseShader("shaders\\BaseLightMapVs.glsl", "shaders\\BaseLightMapFs.glsl");
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
	 
		m_pShader->setVec3("light.position", m_lightPos);
		m_pShader->setVec3("viewPos", pCam->getPosition());

		m_pShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
		m_pShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
		m_pShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("material.shininess", 34.0f);



		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = pCam->GetViewMatrix();
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, m_pos);
		m_pShader->setMat4("model", model);

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
		//// bind diffuse map
		//glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//// bind specular map
		//glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
		//glBindTexture(GL_TEXTURE_2D, 0);
		RestoreRenderState();
	}

	void BaseLightMapMesh::SetDiffuseMap(const char* fileName)
	{
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		m_assert(pGd);
		ITextureMgr* pTexMgr = pGd->getTextureMgr();
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
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		m_assert(pGd);
		ITextureMgr* pTexMgr = pGd->getTextureMgr();
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

	void BaseLightMapMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

	void BaseLightMapMesh::BackupRenderState()
	{
		m_state.LoadCurRenderState();
	}

	void BaseLightMapMesh::RestoreRenderState()
	{
		m_state.SetState();
	}

}