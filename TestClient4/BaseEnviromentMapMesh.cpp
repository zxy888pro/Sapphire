#include "BaseEnviromentMapMesh.h"
#include "Sapphire.h"
#include "GraphicDriver.h"
#include "ITextureMgr.h"
#include "Camera.h"

namespace Sapphire
{

	void BaseEnviromentMapMesh::LoadBaseShader(const char* vs, const char* ps)
	{
		m_pShader = new Shader(vs, ps, "");
		if (m_pShader == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", vs, ps));
			return;
		}
	}

	void BaseEnviromentMapMesh::Init()
	{
	
		LoadBaseShader("shaders\\EnviromentMapVs.glsl", "shaders\\EnviromentMapFs.glsl");
		m_vertices = new float[216]{
			// positions          // normals
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

	void BaseEnviromentMapMesh::Render()
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

		// bind cubemap
		glActiveTexture(GL_TEXTURE0 + GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_2D, m_cubeMapObj);

		glBindVertexArray(m_mVao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		m_pShader->UnUse();
		RestoreRenderState();
	}

	void BaseEnviromentMapMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
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
		m_pShader->UnUse();
	}

	void BaseEnviromentMapMesh::SetCubeMap(uint uid)
	{
		 
		m_cubeMapObj = uid;
		if (glIsTexture(uid) == GL_FALSE)
		{
			SAPPHIRE_LOG("BaseEnviromentMapMesh::SetCubeMap Failed! uid is not a SkyBox");
		}
	}

	void BaseEnviromentMapMesh::Release()
	{
		BaseMesh::Release();
	}

	BaseEnviromentMapMesh::BaseEnviromentMapMesh()
	{
		m_type = MT_EmissionMesh;
	}

	BaseEnviromentMapMesh::~BaseEnviromentMapMesh()
	{

	}

}