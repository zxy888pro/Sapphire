#include "SkyBoxMesh.h"
#include "IGraphicDriver.h"
#include "ITextureMgr.h"
#include "Camera.h"

namespace Sapphire
{

	SkyBoxMesh::SkyBoxMesh()
	{
		m_type = MT_BaseLighMesh;
		m_pDriver = dynamic_cast<IGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
	}

	SkyBoxMesh::~SkyBoxMesh()
	{

	}

	void SkyBoxMesh::LoadBaseShader(const char* vs, const char* ps)
	{
		m_pShader = new Shader(vs, ps, "");
		if (m_pShader == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", vs, ps));
			return;
		}
	}

	void SkyBoxMesh::Init()
	{
		LoadBaseShader("shaders\\SkyboxVs.glsl", "shaders\\SkyboxFs.glsl");
		m_cubeMapObj = 0;
		m_vertices = new float[108]{
			// positions          
			-1.0f, 1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, -1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f,
			-1.0f, -1.0f, 1.0f,

			-1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, -1.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, 1.0f,
			-1.0f, 1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f, 1.0f,
			1.0f, -1.0f, 1.0f
		};

		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 108, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SkyBoxMesh::Render()
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
		glDepthFunc(GL_LEQUAL);

		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = glm::mat4(glm::mat3(pCam->GetViewMatrix())); // remove translation from the view matrix
		m_pShader->setMat4("view", view);
		m_pShader->setMat4("projection", projection);
		glBindVertexArray(m_mVao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeMapObj);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); 
		m_pShader->UnUse();
		RestoreRenderState();
	}

	void SkyBoxMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{

	}

	void SkyBoxMesh::LoadCubeMap(std::string filePath)
	{
		m_assert(m_pDriver);
		ITextureMgr* pTexMgr = m_pDriver->getTextureMgr();
		m_assert(pTexMgr);
		ITexture* pTexture = pTexMgr->CreateCubeTextureFromFile(filePath.c_str());
		if (pTexture == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Load %s Failed!", filePath));
		}
		else
		{
			m_pShader->Use();
			pTexMgr->SetTexture(pTexture, TU_CUBEMAP);
			m_cubeMapObj = pTexture->getUID();
			m_pShader->setInt("skybox", TU_CUBEMAP);
		}
	}

	uint SkyBoxMesh::GetSkyBoxUID()
	{
		return m_cubeMapObj;
	}

	void SkyBoxMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			delete[] m_vertices;
		}
	}

}