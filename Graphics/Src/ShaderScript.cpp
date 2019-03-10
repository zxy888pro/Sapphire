#include "Graphics.h"
#include "ShaderScript.h"
#include "FileStream.h"
#include "GraphicDriver.h"


namespace Sapphire
{

	bool ShaderScript::Load(const std::string& scriptsPath)
	{
		Path fpath = scriptsPath;
		if (!FileIsExist(fpath.c_str()))
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
			//通过后缀名，区分shader类型
			std::string ext = fpath.getFileExt();
			if (ext == "vs")
			{
				m_type = ShaderType::VS;
			}
			else if (ext == "ps")
			{
				m_type = ShaderType::PS;
			}
			else if (ext == "gs")
			{
				m_type = ShaderType::GS;
			}
			else if (ext == "cs")
			{
				m_type = ShaderType::CS;
			}
			else
			{
				m_type = ShaderType::UNKNOWN;
			}
			

		}
		return true;
		
	}

	bool ShaderScript::UnLoad()
	{
		return false;
	}

}