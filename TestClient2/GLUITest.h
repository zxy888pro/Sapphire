#pragma once

#include "stdafx.h"
#include <glm.hpp>
#include "Shader.h"

namespace Sapphire
{
	struct Character {
		GLuint     TextureID;  // ���������ID
		glm::ivec2 Size;       // ���δ�С
		glm::ivec2 Bearing;    // �ӻ�׼�ߵ�������/������ƫ��ֵ
		GLuint     Advance;    // ԭ�����һ������ԭ��ľ���
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