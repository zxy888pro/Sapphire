#pragma once



//应用程序入口
#define SAPPHIRE_MAIN(function) \
int main(int argc, char** argv) \
{ \
	Sapphire::ParseArguments(argc, argv); \
    return function; \
}
