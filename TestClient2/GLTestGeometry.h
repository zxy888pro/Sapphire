#pragma once
#include "stdafx.h"
#include <vector>
#include "Shader.h"

namespace Sapphire
{
	class GLTestGeometry
	{
	public:
		typedef std::vector<GLuint> GLHandleVec;

		GLTestGeometry(Shader* pShader);
		virtual ~GLTestGeometry();

		virtual void Init();
		virtual void Render();
		virtual void Clean();

		glm::vec3 getPosition() const { return m_position; }
		void setPosition(glm::vec3 val) { m_position = val; }

	private:

		GLfloat* m_vertices;
		GLuint* m_indices;
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;
		GLHandleVec m_textures;
		glm::vec3  m_position;
		Shader* m_shader;

		GLboolean bDepthTest;

		void backupState();
		void RestoreState();
	};
}
