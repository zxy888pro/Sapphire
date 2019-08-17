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

		//���ó���ĳ�ʼ������
		m_engineParameter["FullScreen"] = false;
		m_engineParameter["WindowTitle"] = "SampleApplication";
		m_engineParameter["WindowWidth"] = 1024;
		m_engineParameter["WindowHeight"] = 768;
		m_engineParameter["Vsync"] = false;
		m_engineParameter["MultiSample"] = false;
		m_engineParameter["X"] = 400;
		m_engineParameter["Y"] = 200;
		 

		
	}

	void SampleApplication::Start()
	{
		
	}

	void SampleApplication::Stop()
	{

	}

}



SAPPHIRE_APPLICATION_MAIN(Sapphire::SampleApplication);

