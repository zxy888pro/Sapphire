#include "stdafx.h"


namespace Sapphire
{
	class Shader;

	class LightAndColor
	{
	public:

		LightAndColor();
		~LightAndColor();


		void Init(const char* cubeVs, const char* cubeFs, const char* lampVs, const char* lampFs);
		void Render();
		void Release();
	private:

		Shader*  m_uCubeShader;
		Shader*  m_uLampShader;
		GLuint  m_uVAO;
		GLuint  m_uLightVAO;
		GLuint  m_uVBO;
		GLuint  m_uLightVBO;
		GLuint  m_uEBO;

	};

}