#include "CommonApi.h"
#ifdef  SAPPHIRE_WIN
#include <windows.h>
#else

#endif
#include "stringHelper.h"

static std::vector<std::string> arguments;

namespace  Sapphire
{

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::string& cmdLine, bool skipFirstArgument /*= true*/)
	{
		arguments.clear();

		unsigned cmdStart = 0, cmdEnd = 0;
		bool inCmd = false;
		bool inQuote = false;

		for (unsigned i = 0; i < cmdLine.length(); ++i)
		{
			if (cmdLine[i] == '\"')
				inQuote = !inQuote;
			if (cmdLine[i] == ' ' && !inQuote)
			{
				if (inCmd)
				{
					inCmd = false;
					cmdEnd = i;
					// 不存第一个参数，是可执行程序的名字
					if (!skipFirstArgument)
						arguments.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
					skipFirstArgument = false;
				}
			}
			else
			{
				if (!inCmd)
				{
					inCmd = true;
					cmdStart = i;
				}
			}
		}
		if (inCmd)
		{
			cmdEnd = cmdLine.length();
			if (!skipFirstArgument)
				arguments.push_back(cmdLine.substr(cmdStart, cmdEnd - cmdStart));
		}

		// 从参数剔除双引号
		for (unsigned i = 0; i < arguments.size(); ++i)
		{
			int ret = ReplaceA(arguments[i], "\"", "");
		}

		return arguments;
	}

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const char* cmdLine)
	{
		return ParseArguments(cmdLine);
	}

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const std::wstring& cmdLine)
	{
		std::string convStr = WString2String(cmdLine);
		return ParseArguments(convStr,false);
	}

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(const wchar_t* cmdLine)
	{
		std::string convStr = WString2String(cmdLine);
		return ParseArguments(convStr, false);
	}

	SAPPHIRE_API const std::vector<std::string>& ParseArguments(int argc, char** argv)
	{
		std::string cmdLine = "";

		for (int i = 0; i < argc; ++i)
		{
			cmdLine += StringFormat("\"%s\" ", (const char*)argv[i]);
		}
			

		return ParseArguments(cmdLine,false);
	}

	const std::vector<std::string>& GetArguments()
	{
		return arguments;
	}

	

	SAPPHIRE_API std::string GetPlatform()
	{
#if defined(ANDROID)
		return "Android";
#elif defined(IOS)
		return "iOS";
#elif defined(WIN32)
		return "Windows";
#elif defined(__APPLE__)
		return "Mac OS X";
#elif defined(RPI)
		return "Raspberry Pi";
#elif defined(__EMSCRIPTEN__)
		return "HTML5";
#elif defined(__linux__)
		return "Linux";
#else
		return "";
#endif
	}

}