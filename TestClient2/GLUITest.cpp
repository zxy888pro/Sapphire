#include "GLUITest.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <freetype/ftoutln.h>
#include <freetype/ftstroke.h>
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

	void RasterCallback(const int y, const int count, const FT_Span * const spans, void * const user)
	{
		Spans *sptr = (Spans *)user;
		for (int i = 0; i < count; ++i)
			sptr->push_back(Span(spans[i].x, y, spans[i].len, spans[i].coverage));
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
			if (FT_Load_Char(face, c, FT_LOAD_RENDER | FT_LOAD_MONOCHROME))
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
			//WriteTGA(fileName, (Pixel32*)pixelBuffer, 64, 64);

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
			CharactersMap.insert(std::pair<GLchar, Character>(c, character));
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
			Character ch = CharactersMap[*c];
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

	void GLUITest::Create(std::string fontPath, int width, int height)
	{
		//读取一个字体
		if (FT_New_Face(ft, fontPath.c_str(), 0, &face));
		//设定字体大小,将宽度值设为0表示我们要从字体面通过给定的高度中动态计算出字形的宽度
		FT_Set_Pixel_Sizes(face, width, height);
	}

	void GLUITest::Release()
	{
		FT_Done_Face(face);
		FT_Done_FreeType(ft);
		face = 0;
		ft = 0;
	}

	void GLUITest::RenderSpans(FT_Library &library, FT_Outline * const outline, Spans *spans)
	{
		//设置光栅化参数
		FT_Raster_Params params;
		memset(&params, 0, sizeof(params));
		params.flags = FT_RASTER_FLAG_AA | FT_RASTER_FLAG_DIRECT;
		params.gray_spans = RasterCallback;
		params.user = spans;
		// 在回调函数RasterCallback中实现像素化到位图中
		FT_Outline_Render(library, outline, &params);
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

	void GLUITest::RasterzationOutline(uint code)
	{
		//设置加粗
		FT_Outline* pOutline = &face->glyph->outline;
		FT_Pos strength = 30;
		FT_Outline_Embolden(pOutline, strength);

		//字模的扫描线信息
		Spans spans;
		//先渲染字模扫描线
		RenderSpans(ft, &face->glyph->outline, &spans);
		//轮廓的扫描线信息
		Spans outlineSpans;
		//创建一个笔触
		FT_Stroker stroker;
		if (0 != FT_Stroker_New(ft, &stroker))
		{
			FT_Stroker_Set(stroker,
				(int)(3 * 64),
				FT_STROKER_LINECAP_ROUND,
				FT_STROKER_LINEJOIN_ROUND,
				0);
		}
		//字模
		FT_Glyph glyph;
		//复制一份字模
		if (FT_Get_Glyph(face->glyph, &glyph) == 0)
		{
			//设置字模描边渲染
			FT_Glyph_StrokeBorder(&glyph, stroker, 0, 1);
			//判断字模格式是否是描边
			if (glyph->format == FT_GLYPH_FORMAT_OUTLINE)
			{
				//渲染轮廓的Span到Span列表中
				FT_Outline *o =
					&reinterpret_cast<FT_OutlineGlyph>(glyph)->outline;
				RenderSpans(ft, o, &outlineSpans);
			}
		}
		// 清理
		FT_Stroker_Done(stroker);
		FT_Done_Glyph(glyph);

		GLuint texture;
		//轮廓颜色
		const Pixel32 outlineCol(0xff, 0xff, 0x00, 0x00);
		const Pixel32 fontCol(0xff, 0x00, 0xff, 0x00);
		// 已经收集到所有需要信息
		if (!spans.empty())
		{
			//估计出span列表的边界范围
			Rect rect(spans.front().x,
				spans.front().y,
				spans.front().x,
				spans.front().y);
			for (Spans::iterator s = spans.begin();
				s != spans.end(); ++s)
			{
				rect.Include(Vec2(s->x, s->y));
				rect.Include(Vec2(s->x + s->width - 1, s->y));
			}
			for (Spans::iterator s = outlineSpans.begin();
				s != outlineSpans.end(); ++s)
			{
				rect.Include(Vec2(s->x, s->y));
				rect.Include(Vec2(s->x + s->width - 1, s->y));
			}

#if 1
			//这些用不到，除非你要绘制多于一个的字模0
			/*float bearingX = face->glyph->metrics.horiBearingX >> 6;
			float bearingY = face->glyph->metrics.horiBearingY >> 6;
			float advance = face->glyph->advance.x >>6;*/
			float bearingX = face->glyph->metrics.horiBearingX >> 6;
			float bearingY = face->glyph->metrics.horiBearingY >> 6;
			float advance = face->glyph->advance.x;
			//advance = 25;
#endif

			// 取得图像的宽高
			int imgWidth = rect.Width(),
				imgHeight = rect.Height(),
				imgSize = imgWidth * imgHeight;

			// 分配内存缓冲区
			Pixel32 *pxl = new Pixel32[imgSize];
			memset(pxl, 0, sizeof(Pixel32) * imgSize);

			// 循环边框扫描线数据绘制它们到图像缓冲区中
			for (Spans::iterator s = outlineSpans.begin();
				s != outlineSpans.end(); ++s)
				for (int w = 0; w < s->width; ++w)
					pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
					+ s->x - rect.xmin + w)] =
					Pixel32(outlineCol.r, outlineCol.g, outlineCol.b,
					s->coverage);

			// 循环字模扫描线数据绘制它们到图像缓冲区中
			for (Spans::iterator s = spans.begin();
				s != spans.end(); ++s)
				for (int w = 0; w < s->width; ++w)
				{
					Pixel32 &dst =
						pxl[(int)((imgHeight - 1 - (s->y - rect.ymin)) * imgWidth
						+ s->x - rect.xmin + w)];
					Pixel32 src = Pixel32(fontCol.r, fontCol.g, fontCol.b,
						s->coverage);
					dst.r = (int)(dst.r + ((src.r - dst.r) * src.a) / 255.0f);
					dst.g = (int)(dst.g + ((src.g - dst.g) * src.a) / 255.0f);
					dst.b = (int)(dst.b + ((src.b - dst.b) * src.a) / 255.0f);
					dst.a = MIN(255, dst.a + src.a);
				}

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				imgWidth,
				imgHeight,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				pxl
				);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			Character character = {
				texture,  //纹理ID存到Character中
				glm::ivec2(imgWidth, imgWidth),
				glm::ivec2(bearingX, bearingY),
				advance
			};
			std::string fileName = StringFormatA("char_%d.tga", code);
			CharactersMap.insert(std::pair<GLchar, Character>(code, character));
			//WriteTGA(fileName, pxl, imgWidth, imgHeight);
			delete[] pxl;
		}

	}

	void GLUITest::RasterzationNormal(uint code)
	{

		if (FT_Load_Char(face, code, FT_LOAD_RENDER | FT_RENDER_MODE_NORMAL))
		{
			LogUtil::LogMsgLn("ERROR::FREETYTPE: Failed to load Glyph");
			return;
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
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
			face->glyph->advance.x };
		CharactersMap.insert(std::pair<GLchar, Character>(code, character));
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


	}

	void GLUITest::RasterzationMono(uint code)
	{
		if (FT_Load_Char(face, code, FT_LOAD_RENDER | FT_LOAD_MONOCHROME))
		{
			LogUtil::LogMsgLn("ERROR::FREETYTPE: Failed to load Glyph");
			return;
		}
		byte* pixelBuffer = new byte[64 * 64 * 4];
		memset(pixelBuffer, 0, 64 * 64 * 4);
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
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
		// 生成纹理
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
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
		CharactersMap.insert(std::pair<GLchar, Character>(code, character));

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

	}

	
}