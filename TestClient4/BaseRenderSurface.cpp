#include "BaseRenderSurface.h"
#include <MemoryMgr.h>
#include <GraphicDriver.h>
#include <GraphicsDefs.h>
#include "BaseRenderTarget.h"


namespace Sapphire
{

	BaseRenderSurface::BaseRenderSurface()
	{

	}

	BaseRenderSurface::~BaseRenderSurface()
	{

	}

	void BaseRenderSurface::Init()
	{
		LoadBaseShader("shaders\\PostEffectVs.glsl", "shaders\\PostEffectFs.glsl");
		m_vertices = _New float[24]{
			 // positions   // texCoords
			    -1.0f, 1.0f, 0.0f, 1.0f,
				-1.0f, -1.0f, 0.0f, 0.0f,
				1.0f, -1.0f, 1.0f, 0.0f,

				-1.0f, 1.0f, 0.0f, 1.0f,
				1.0f, -1.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f
		};
		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 24, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

	}

	void BaseRenderSurface::Render()
	{
		if (!m_pShader)
		{
			LogUtil::LogMsgLn("ShaderProgram 未能初始化！ 可能加载错误!");
			return;
		}
		m_pShader->Use();
		BackupRenderState();
		glDisable(GL_DEPTH_TEST);
		glBindVertexArray(m_mVao);
		glActiveTexture(GL_TEXTURE0 + TU_POSTEFFECT);
		m_pShader->setInt("screenTexture", TU_POSTEFFECT);
		glBindTexture(GL_TEXTURE_2D, m_texObj);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
		m_pShader->UnUse();
		RestoreRenderState();
	}

	void BaseRenderSurface::SetTexture(GLuint texture)
	{
		m_texObj = texture;
	}

	GLuint BaseRenderSurface::GetTexture()
	{
		return m_texObj;
	}

	void BaseRenderSurface::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		 
	}

	void BaseRenderSurface::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

}