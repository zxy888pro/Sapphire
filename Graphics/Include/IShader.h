#pragma once
#include "Graphics.h"



namespace Sapphire
{
	struct IShaderVariation;

	struct IShader
	{

		virtual const std::string&  GetSourceCode(ShaderType type) const = 0;
		//���ز�ͬԤ�����shader����
		virtual  IShaderVariation* GetVariation(ShaderType type, const std::string& defines) = 0;
		virtual  IShaderVariation* GetVariation(ShaderType type, const char* defines) = 0;

	};
}