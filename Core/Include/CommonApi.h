#pragma once
#include "Sapphire.h"


namespace Sapphire
{

	/// ���������в���,Ĭ��������һ�����������ǿ�ִ�г��������
	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::string& cmdLine, bool skipFirstArgument = true);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const char* cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::wstring& cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const wchar_t* cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(int argc, char** argv);
	/// ����֮ǰ�����Ĳ���
	SAPPHIRE_API const std::vector<std::string>& GetArguments();

	SAPPHIRE_API void                            ErrorDialog(const std::string& title, const std::string& message);

	SAPPHIRE_API std::string GetPlatform();

}


