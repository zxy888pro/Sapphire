#include "SampleApplication.h"
#include "Ptr.h"
#include <vld.h>


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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) 
{
		Sapphire::ParseArguments(GetCommandLineW()); 
		return RunApplication();
}