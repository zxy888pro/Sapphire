#include "stdafx.h"
#include "BaseMesh.h"
#include "BaseLight.h"

namespace Sapphire
{

	BaseMesh::BaseMesh()
	{
		setPos(glm::vec3(0, 0, 0));
		setRot(glm::vec3(0, 0, 0));
		setScale(glm::vec3(1, 1, 1));
		m_type = MT_BaseMesh;
	}

	BaseMesh::~BaseMesh()
	{

	}

	void BaseMesh::LoadBaseShader(const char* vs, const char* ps)
	{
	   m_pShader = new Shader(vs, ps, "");
	   if (m_pShader == NULL)
	   {
		   LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", vs, ps));
		   return;
	   }
	}

	void BaseMesh::Init()
	{

	}

	void BaseMesh::Render()
	{

	}

	void BaseMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		return;
	}

	void BaseMesh::Release()
	{
		if (m_pShader)
		{
			glDeleteProgram(m_pShader->getShaderProgram());
			safeDelete(m_pShader);
			
		}
		glBindVertexArray(m_mVao);
		glDeleteVertexArrays(1, &m_mVao);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glDeleteBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		m_pShader = NULL;
		m_mVbo = 0;
		m_mVao = 0;
		m_mEbo = 0;
	}

	void BaseMesh::BackupRenderState()
	{
		m_state.LoadCurRenderState();
	}

	void BaseMesh::RestoreRenderState()
	{
		m_state.SetState();
	}

}