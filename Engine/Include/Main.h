#pragma once



//Ӧ�ó������
#define SAPPHIRE_MAIN(function) \
int main(int argc, char** argv) \
{ \
	Sapphire::ParseArguments(argc, argv); \
    return function; \
}
