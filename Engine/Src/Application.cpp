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

		m_engine = new Engine(pCore);
	}

	Application::~Application()
	{

	}

	int Application::Run()
	{
		try
		{
			Setup(); //设置参数

			if (!m_engine->Initialize(m_engineParameter))
			{
				ErrorExit();
				return m_exitCode;
			}

			Start(); 
			if (m_exitCode)
				return m_exitCode;
#if defined(SAPPHIRE_WIN) || defined(SAPPHIRE_LINUX)
			while (!m_engine->IsExiting())
				m_engine->RunFrame();         //游戏主循环

			Stop();
#else

#endif
			return m_exitCode;  //退出

		}
		catch (std::bad_alloc&)  //内存不够，抛出异常
		{
			ErrorDialog(GetClassTypeName(), "An out-of-memory error occurred. The application will now exit.");
			return EXIT_FAILURE;
		}
		
	}

	void Application::ErrorExit(const std::string& message /*= ""*/)
	{
		m_engine->Exit(); // Close the rendering window
		m_exitCode = EXIT_FAILURE;

		if (!message.length ())
		{
			ErrorDialog(GetClassTypeName(), m_startupErrors.length() ? m_startupErrors :
				"Application has been terminated due to unexpected error.");
		}
		else
			ErrorDialog(GetClassTypeName(), message);
	}

}