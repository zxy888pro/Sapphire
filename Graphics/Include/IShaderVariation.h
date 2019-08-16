#pragma once
#include "Graphics.h"

namespace Sapphire
{
	struct IShaderVariation
	{

		virtual bool Create() = 0;

		virtual const std::string GetFullName() const = 0;

		virtual void SetName(const std::string&  name) = 0;

		virtual void SetDefines(const std::string& defines) = 0;

		virtual const std::string& GetName() const = 0;

		virtual const std::string& GetDefines() const = 0;

		virtual IShader* GetOwner() const = 0;


	};
}