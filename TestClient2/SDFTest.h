#ifndef __SDF_TEST__
#define  __SDF_TEST__

#include "stdafx.h"
#include "Shader.h"

namespace Sapphire
{
	 
	struct  SDFCharDataItem
	{
		uint id;
		uint x;
		uint y;
		uint width;
		uint height;
		float offsetX;
		float offsetY;
		float xAdvance;
		float yAdvance;
		uint page;
		uint chnl;

	};
	

	class SDFTest
	{
	public:

		SDFTest(Shader* pShader);
		virtual ~SDFTest();

		

		void init(std::string sdfFontName, int width, int height);
		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		
	private:

		std::string faceName;
		uint charCount;
		std::unordered_map<uint, SDFCharDataItem*> charDataMap;
		
		GLuint uTextureID;
		GLuint uTextureWidth;
		GLuint uTextrueHeight;
		GLuint uTextureChannel;
		Shader*  m_pShader;
		GLuint VAO, VBO;
		GLboolean bCullFace;
		GLboolean bAlphaBlend;
		GLint nUnpackAlign = 4;

		bool RenderChar(uint index, uint charcode, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
		void backupState();
		void RestoreState();
		bool ReadFaceName(std::string& src);
		bool ReadCharCount(std::string& src);
		bool ReadCharData(std::string& src);
		bool Match(std::string& src, std::string& pattarn);
		void Clear();
	};


}

#endif // __SDF_TEST__
