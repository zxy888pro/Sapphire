#include "Graphics.h"
#include "ShaderMgr.h"
#include "ShaderScript.h"


namespace Sapphire
{

	void ShaderMgr::StoreScriptDependency(std::string name, ShaderScript* pScripts)
	{

	}

	std::string ShaderMgr::GetFileName(const std::string Name, ShaderType type)
	{
		std::string fileName = Name + "_" + ENUM2STR(type);
		fileName += ".glsl";
		return fileName;
	}

}