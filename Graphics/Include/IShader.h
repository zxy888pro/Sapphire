#pragma once
#include "Graphics.h"



namespace Sapphire
{
	struct IShaderVariation;

	struct IShader
	{

		virtual const std::string&  GetSourceCode(ShaderType type) const = 0;
		//加载不同预定义的shader对象
		virtual  IShaderVariation* GetVariation(ShaderType type, const std::string& defines) = 0;
		virtual  IShaderVariation* GetVariation(ShaderType type, const char* defines) = 0;

	};
}