#include "stdafx.h"
#include "BaseMesh.h"

namespace Sapphire
{

	BaseMesh::BaseMesh()
	{
		m_pos = glm::vec3(0, 0, 0);
		m_rot = glm::vec3(0, 0, 0);
		m_scale = glm::vec3(1, 1, 1);
			 
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
		
	}

	void BaseMesh::RestoreRenderState()
	{

	}

}