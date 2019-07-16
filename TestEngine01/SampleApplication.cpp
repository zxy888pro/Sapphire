#include "SampleApplication.h"
#include "Ptr.h"


#define new  new(_CLIENT_BLOCK, __FILE__, __LINE__)

namespace Sapphire
{
	SampleApplication::SampleApplication(Sapphire::Core* pCore) :Application(pCore)
	{

	}

	SampleApplication::~SampleApplication()
	{

	}

	void SampleApplication::Setup()
	{

	}

	void SampleApplication::Start()
	{
		_CrtDumpMemoryLeaks();
	}

	void SampleApplication::Stop()
	{

	}

}



//SAPPHIRE_APPLICATION_MAIN(SampleApplication);


int RunApplication()
{
	Sapphire::SharedPtr<Sapphire::Core> pCore(new Sapphire::Core());
	Sapphire::SharedPtr<Sapphire::SampleApplication> application(new Sapphire::SampleApplication(pCore));
	return application->Run();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); \
		Sapphire::ParseArguments(GetCommandLineW()); \
		return RunApplication();
}