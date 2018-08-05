#pragma once

#include "stdafx.h"
#include <glm.hpp>
#include "Shader.h"

namespace Sapphire
{
	struct Character {
		GLuint     TextureID;  // 字形纹理的ID
		glm::ivec2 Size;       // 字形大小
		glm::ivec2 Bearing;    // 从基准线到字形左部/顶部的偏移值
		GLuint     Advance;    // 原点距下一个字形原点的距离
	};

	class GLUITest
	{
	public:

		GLUITest(Shader* pShader);
		virtual ~GLUITest();
		void Init();
		void Clean();
		void Render();
		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		
	private:

		FT_Face face;
		FT_Library ft;

		Shader*  m_pShader;
		GLuint VAO, VBO;
		GLboolean bCullFace;
		GLboolean bAlphaBlend; 
		std::map<GLchar, Character> Characters;


		void backupState();
		void RestoreState();
	};
}