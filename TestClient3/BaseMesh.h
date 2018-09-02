#pragma once

#include <Sapphire.h>
#include "Shader.h"
#include <glm.hpp>
#include "RenderState.h"

namespace Sapphire
{

	class BaseMesh
	{
	public:

		BaseMesh();
		virtual ~BaseMesh();
		virtual void LoadBaseShader(const char* vs, const char* ps);
		virtual void Init();
		virtual void Render();
		

	protected:

		virtual void Release();

		GLuint m_mVbo;
		GLuint m_mVao;
		GLuint m_mEbo;
		glm::vec3  m_pos;
		glm::vec3  m_rot;
		glm::vec3  m_scale;
		RenderState  m_state;
		Shader* m_pShader;

	private:

		
		virtual void BackupRenderState();
		virtual void RestoreRenderState();

	};

}