#pragma once
#include "Sapphire.h"
#include "CommonApi.h"

//声明main函数
//调试模式
#if defined(_MSC_VER) && defined(_DEBUG) && !defined(SAPPHIRE_WIN32_CONSOLE)
#define SAPPHIRE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
Sapphire::ParseArguments(GetCommandLineW()); \
return function; \
}
//Release模式
#elif defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS) && !defined(SAPPHIRE_WIN32_CONSOLE)
#define SAPPHIRE_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
    Sapphire::ParseArguments(GetCommandLineW()); \
    int exitCode; \
    __try \
			    { \
        exitCode = function; \
			    } \
    __except(Sapphire::WriteMiniDump("Sapphire", GetExceptionInformation())) \
			    { \
			    } \
    return exitCode; \
}
#else
//应用程序入口
#define SAPPHIRE_MAIN(function) \
int main(int argc, char** argv) \
{ \
	Sapphire::ParseArguments(argc, argv); \
    return function; \
}
#endif

