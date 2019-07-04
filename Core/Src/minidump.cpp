#include "minidump.h"



#if defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS)

#include <cstdio>
#include <io.h>
#include <fcntl.h>
#include <time.h>
#include <windows.h>
#include <dbghelp.h>
#include "CommonApi.h"

namespace Sapphire
{
	static bool miniDumpWritten = false;

	SAPPHIRE_API int WriteMiniDump(const char* applicationName, void* exceptionPointers)
	{
		if (miniDumpWritten)
			return EXCEPTION_EXECUTE_HANDLER;

		miniDumpWritten = true;

		MINIDUMP_EXCEPTION_INFORMATION info;
		info.ThreadId = GetCurrentThreadId();
		info.ExceptionPointers = (EXCEPTION_POINTERS*)exceptionPointers;
		info.ClientPointers = TRUE;

		static time_t sysTime;
		time(&sysTime);
		const char* dateTime = ctime(&sysTime);
		std::string dateTimeStr = dateTime;
		Replace(dateTimeStr, "\n", "");
		Replace(dateTimeStr, ":", "");
		Replace(dateTimeStr, "/", "");
		Replace(dateTimeStr, " ", "_");

		std::string	 pathName = GetCurrentDirA();
		std::string  appName = applicationName;
		std::wstring miniDumpDir = String2WString(pathName);
		
		std::wstring miniDumpName = miniDumpDir + String2WString(applicationName) + L"_" + String2WString(dateTimeStr) + L".dmp";
 

		CreateDirectoryW(std::wstring(miniDumpDir).c_str(), 0);
		HANDLE file = CreateFileW(std::wstring(miniDumpName).c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ,
			0, CREATE_ALWAYS, 0, 0);

		BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpWithDataSegs, &info, 0, 0);
		CloseHandle(file);

		if (success)
			ErrorDialog(applicationName, "An unexpected error occurred. A minidump was generated to " + WString2String(miniDumpName));
		else
			ErrorDialog(applicationName, "An unexpected error occurred. Could not write minidump.");

		return EXCEPTION_EXECUTE_HANDLER;
	}

}
#endif

