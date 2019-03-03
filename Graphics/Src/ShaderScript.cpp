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
			LogUtil::LogMsgLn(StringFormatA("����ʧ�ܣ� %s �����ڣ�", fpath.c_str()));
			return false;
		}
        
		FileStream fs(scriptsPath.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			m_scriptContent = fs.ReadString(MAX_SCRIPT_LENGTH);
			fs.Release();
			m_name = fpath.getFileName();
			//���ز��洢���������ļ�
			

		}
		
	}

	bool ShaderScript::UnLoad()
	{

	}

}