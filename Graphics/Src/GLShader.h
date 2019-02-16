#pragma once
#include "Graphics.h"

namespace Sapphire
{
	class IShaderVariation;
	

	class GLShader : public RefCounter
	{
	public:
		GLShader();
		virtual ~GLShader();

		IShaderVariation* GetVariation(ShaderType type, const std::string& defines);
		IShaderVariation* GetVariation(ShaderType type, const char* defines);

	private:

		std::string m_vsSource;
		std::string m_psSource;
		std::string m_gsSource;

		uint  m_timeStamp;
		uint  m_numVariation;

		std::unordered_map<std::string, IShaderVariation*> vsVariation;
		std::unordered_map<std::string, IShaderVariation*> psVariation;
		std::unordered_map<std::string, IShaderVariation*> gsVariation;

	};
}