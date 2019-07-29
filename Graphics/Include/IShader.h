#pragma once
#include "Graphics.h"

struct IShaderVariation;

namespace Sapphire
{
	 
	struct IShader
	{

		virtual const std::string&  GetSourceCode(ShaderType type) const = 0;
		//���ز�ͬԤ�����shader����
		virtual  IShaderVariation* GetVariation(ShaderType type, const std::string& defines) = 0;
		virtual  IShaderVariation* GetVariation(ShaderType type, const char* defines) = 0;

	};
}