#include "CommonApi.h"
#include "Application.h"
#include "Engine.h"

namespace Sapphire
{



	


	Application::Application(Core* pCore) :BaseObject(pCore)
		, m_exitCode(EXIT_SUCCESS)
	{
		//
		m_engineParameter = Engine::ParseParameters(GetArguments());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{

	}

	void Application::ErrorExit(const std::string& message /*= ""*/)
	{

	}

}