#pragma once
#include "Sapphire.h"


namespace Sapphire
{

	/// 解析命令行参数,默认跳过第一个参数，它是可执行程序的名字
	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::string& cmdLine, bool skipFirstArgument = true);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const char* cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::wstring& cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const wchar_t* cmdLine);

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(int argc, char** argv);
	/// 返回之前解析的参数
	SAPPHIRE_API const std::vector<std::string>& GetArguments();

	SAPPHIRE_API void                            ErrorDialog(const std::string& title, const std::string& message);

	SAPPHIRE_API std::string GetPlatform();

}


