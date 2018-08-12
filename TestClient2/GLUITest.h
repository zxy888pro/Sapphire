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


	//描述一条起点在(x, y), 宽度为with,  converage 存储的是灰度数据(0-255)的扫描线信息
	struct Span
	{
		Span() { }
		Span(int _x, int _y, int _width, int _coverage)
			: x(_x), y(_y), width(_width), coverage(_coverage) { }

		int x, y, width, coverage;
	};
	//扫描线列表
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
		//渲染扫描线
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