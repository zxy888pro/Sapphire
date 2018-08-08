#include "GLUITest.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <freetype/ftoutln.h>
#include <mathHelper.h>
#include <Core.h>
#include <logUtil.h>
#include <fstream>
#include <stringHelper.h>
 
#if (('1234' >> 24) == '1')
#elif (('4321' >> 24) == '1')
#define BIG_ENDIAN
#else
#error "Couldn't determine the endianness!"
#endif

namespace Sapphire
{
	union Pixel32
	{
		Pixel32()
			: integer(0) { }
		Pixel32(byte bi, byte gi, byte ri, byte ai = 255)
		{
			b = bi;
			g = gi;
			r = ri;
			a = ai;
		}

		uint integer;

		struct
		{
#ifdef BIG_ENDIAN
			byte a, r, g, b;
#else // BIG_ENDIAN
			byte b, g, r, a;
#endif // BIG_ENDIAN
		};
	};


	struct Vec2
	{
		Vec2() { }
		Vec2(float a, float b)
			: x(a), y(b) { }

		float x, y;
	};


	struct Rect
	{
		Rect() { }
		Rect(float left, float top, float right, float bottom)
			: xmin(left), xmax(right), ymin(top), ymax(bottom) { }

		void Include(const Vec2 &r)
		{
			xmin = MIN(xmin, r.x);
			ymin = MIN(ymin, r.y);
			xmax = MAX(xmax, r.x);
			ymax = MAX(ymax, r.y);
		}

		float Width() const { return xmax - xmin + 1; }
		float Height() const { return ymax - ymin + 1; }

		float xmin, xmax, ymin, ymax;
	};

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(push, 1)
#pragma pack(1)               // Dont pad the following struct.
#endif

	struct TGAHeader
	{
		byte   idLength,           // Length of optional identification sequence.
			paletteType,        // Is a palette present? (1=yes)
			imageType;          // Image data type (0=none, 1=indexed, 2=rgb,
		// 3=grey, +8=rle packed).
		ushort  firstPaletteEntry,  // First palette index, if present.
			numPaletteEntries;  // Number of palette entries, if present.
		byte   paletteBits;        // Number of bits per palette entry.
		ushort  x,                  // Horiz. pixel coord. of lower left of image.
			y,                  // Vert. pixel coord. of lower left of image.
			width,              // Image width in pixels.
			height;             // Image height in pixels.
		byte   depth,              // Image color depth (bits per pixel).
			descriptor;         // Image attribute flags.
	};

#if defined(_MSC_VER) || defined(__GNUC__)
#pragma pack(pop)
#endif

	bool
		WriteTGA(const std::string &filename,
		const Pixel32 *pxl,
		ushort width,
		ushort height)
	{
		std::ofstream file(filename.c_str(), std::ios::binary);
		if (file)
		{
			TGAHeader header;
			memset(&header, 0, sizeof(TGAHeader));
			header.imageType = 2;
			header.width = width;
			header.height = height;
			header.depth = 32;
			header.descriptor = 0x20;

			file.write((const char *)&header, sizeof(TGAHeader));
			file.write((const char *)pxl, sizeof(Pixel32) * width * height);

			return true;
		}
		return false;
	}

	GLUITest::GLUITest(Shader* pShader)
	{
		m_pShader = pShader;
		VAO = 0;
		VBO = 0;
	}

	GLUITest::~GLUITest()
	{

	}

	void GLUITest::Init()
	{	
		
		//初始化freetype库
		if (FT_Init_FreeType(&ft));

		//读取一个字体
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face));
		//设定字体大小,将宽度值设为0表示我们要从字体面通过给定的高度中动态计算出字形的宽度
		FT_Set_Pixel_Sizes(face, 0, 48);

		//加载字符, FT_LOAD_RENDER, face->glyph->bitmap  FT_LOAD_RENDER 加载时渲染
		/*if (FT_Load_Char(face, 'X', FT_LOAD_RENDER ))
			LogUtil::LogMsgLn("ERROR::FREETYTPE: Failed to load Glyph");*/
		
		
			
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
		for (GLubyte c = 0; c < 128; c++)
		{
			byte* pixelBuffer = new byte[64 * 64 * 4];
			memset(pixelBuffer, 0, 64 * 64 * 4);
			// 加载字符的字形 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER  | FT_LOAD_MONOCHROME))
			//if (FT_Load_Char(face, c, FT_LOAD_NO_BITMAP))
			{
				LogUtil::LogMsgLn("ERROR::FREETYTPE: Failed to load Glyph");
				continue;
			}
			//FT_Outline* pOutline = &face->glyph->outline;
			//FT_Pos strength = 15;
			//FT_Outline_Embolden(pOutline, strength);
			//FT_Render_Mode
			//FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);//FT_RENDER_MODE_MONO/FT_RENDER_MODE_NORMAL);

			for (int i = 0; i < face->glyph->bitmap.rows; i++)
			{
				byte* src = face->glyph->bitmap.buffer + i*face->glyph->bitmap.pitch;
				uint pitch = i * 64;// face->glyph->bitmap.rows;
				for (int j = 0; j < face->glyph->bitmap.width; j++)
				{
					uint color = (src[j / 8] & (0x80 >> (j & 7))) ? 0xFFFFFF : 0;
					pixelBuffer[pitch + j] = color;
				}
			}
			
			std::string fileName = StringFormatA("%d_char.tga", c);
			WriteTGA(fileName, (Pixel32*)pixelBuffer, 64, 64);

			// 生成纹理
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			/*glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);*/
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				64,
				64,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
				pixelBuffer
				);
			// 设置纹理选项
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// 储存字符供之后使用
			Character character = {
				texture,  //纹理ID存到Character中
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			delete[] pixelBuffer;
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}
		
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//动态更新
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		
	}

	void GLUITest::Clean()
	{
		
	}

	void GLUITest::Render()
	{
		 
		 


	}

	void GLUITest::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
	{
		backupState();
		// 激活对应的渲染状态
		m_pShader->Use();
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
		m_pShader->setMat4("projection", projection);
		m_pShader->setVec3("textColor", glm::vec3(color.x, color.y, color.z));
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// 遍历文本中所有的字符
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];
			//一些字符（如’p’或’q’）需要被渲染到基准线以下，因此字形四边形也应该被摆放在RenderText的y值以下。ypos的偏移量可以从字形的度量值中得出：
			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// 对每个字符更新VBO
			GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};
			// 在四边形上绘制字形纹理
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// 更新VBO内存的内容
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// 绘制四边形
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// 更新位置到下一个字形的原点，注意单位是1/64像素
			x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);

		RestoreState();
		m_pShader->UnUse();

	}

	void GLUITest::backupState()
	{
		glGetBooleanv(GL_CULL_FACE, &bCullFace);
		glGetBooleanv(GL_BLEND, &bAlphaBlend);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void GLUITest::RestoreState()
	{
		if (bCullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
		if (bAlphaBlend)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

}