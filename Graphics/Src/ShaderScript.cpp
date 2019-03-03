#include "Graphics.h"
#include "ShaderScript.h"
#include "FileStream.h"
#include "GraphicDriver.h"


namespace Sapphire
{

	bool ShaderScript::Load(const std::string& scriptsPath)
	{
		Path fpath = scriptsPath;
		if (!FileIsExistA(fpath.c_str()))
		{
			LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
			return false;
		}
        
		FileStream fs(scriptsPath.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			m_scriptContent = fs.ReadString(MAX_SCRIPT_LENGTH);
			fs.Release();
			m_name = fpath.getFileName();
			//加载并存储所有依赖文件
			

		}
		
	}

	bool ShaderScript::UnLoad()
	{

	}

}