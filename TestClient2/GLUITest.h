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


	//����һ�������(x, y), ���Ϊwith,  converage �洢���ǻҶ�����(0-255)��ɨ������Ϣ
	struct Span
	{
		Span() { }
		Span(int _x, int _y, int _width, int _coverage)
			: x(_x), y(_y), width(_width), coverage(_coverage) { }

		int x, y, width, coverage;
	};
	//ɨ�����б�
	typedef std::vector<Span> Spans;

	enum FontRenderMode
	{
		FONT_RENDER_MODE_NORMAL,
		FONT_RENDER_MODE_MONO,
		FONT_RENDER_MODE_OUTLINE
	};

	class GLUITest
	{
	public:


		GLUITest(Shader* pShader);
		virtual ~GLUITest();
		void Init(FontRenderMode mode, std::string fontPath, int width, int height);
		void Clean();
		void Render();
		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		void Create(std::string fontPath, int width, int height);

		void Release();
	private:

		FT_Face face;
		FT_Library ft;
		uint m_uWidth;
		uint m_uHeight;

		Shader*  m_pShader;
		GLuint VAO, VBO;
		GLboolean bCullFace;
		GLboolean bAlphaBlend; 
		GLint nUnpackAlign = 4;
		std::map<GLchar, Character> CharactersMap;
		FontRenderMode renderMode = FontRenderMode::FONT_RENDER_MODE_NORMAL;
		//��Ⱦɨ����
		void RenderSpans(FT_Library &library,
			FT_Outline * const outline,
			Spans *spans);
		
		void backupState();
		void RestoreState();
		void Rasterzation(uint code);
		bool RasterzationOutline(uint code);
		void RasterzationNormal(uint code);
		void RasterzationMono(uint code);
	};
}