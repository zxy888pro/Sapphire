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
		
		//��ʼ��freetype��
		if (FT_Init_FreeType(&ft));

		//��ȡһ������
		if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face));
		//�趨�����С,�����ֵ��Ϊ0��ʾ����Ҫ��������ͨ�������ĸ߶��ж�̬��������εĿ��
		FT_Set_Pixel_Sizes(face, 0, 48);

		//�����ַ�, FT_LOAD_RENDER, face->glyph->bitmap  FT_LOAD_RENDER ����ʱ��Ⱦ
		/*if (FT_Load_Char(face, 'X', FT_LOAD_RENDER ))
			LogUtil::LogMsgLn("ERROR::FREETYTPE: Failed to load Glyph");*/
		
		
			
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //�����ֽڶ�������
		for (GLubyte c = 0; c < 128; c++)
		{
			byte* pixelBuffer = new byte[64 * 64 * 4];
			memset(pixelBuffer, 0, 64 * 64 * 4);
			// �����ַ������� 
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

			// ��������
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
			// ��������ѡ��
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// �����ַ���֮��ʹ��
			Character character = {
				texture,  //����ID�浽Character��
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
		//��̬����
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
		// �����Ӧ����Ⱦ״̬
		m_pShader->Use();
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
		m_pShader->setMat4("projection", projection);
		m_pShader->setVec3("textColor", glm::vec3(color.x, color.y, color.z));
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		// �����ı������е��ַ�
		std::string::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];
			//һЩ�ַ����硯p����q������Ҫ����Ⱦ����׼�����£���������ı���ҲӦ�ñ��ڷ���RenderText��yֵ���¡�ypos��ƫ�������Դ����εĶ���ֵ�еó���
			GLfloat xpos = x + ch.Bearing.x * scale;
			GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

			GLfloat w = ch.Size.x * scale;
			GLfloat h = ch.Size.y * scale;
			// ��ÿ���ַ�����VBO
			GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },

				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};
			// ���ı����ϻ�����������
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);
			// ����VBO�ڴ������
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			// �����ı���
			glDrawArrays(GL_TRIANGLES, 0, 6);
			// ����λ�õ���һ�����ε�ԭ�㣬ע�ⵥλ��1/64����
			x += (ch.Advance >> 6) * scale; // λƫ��6����λ����ȡ��λΪ���ص�ֵ (2^6 = 64)
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