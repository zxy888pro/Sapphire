#pragma once
#include "Sapphire.h"
#include "Variant.h"
#include "Main.h"


namespace Sapphire
{
	class Engine;

	class SAPPHIRE_CLASS Application : public BaseObject
	{
		SAPPHIRE_OBJECT(Application, BaseObject)
	public: 
		Application(Core* pCore);
		virtual ~Application();

		//在引擎初始化之前的设置各种工作参数，并且在非初始化时创建引擎
		virtual void Setup() { }
		//初始化引擎，在主循环开始之前
		virtual void Start() { }

		//主循环结束后的清理
		virtual void Stop() { }

		//主循环
		virtual int Run();

		//弹报错对话框退出
		void ErrorExit(const std::string& message = "");

	protected:

		SharedPtr<Engine> m_engine;

		VariantMap m_engineParameter;

		std::string m_startupErrors;

		int m_exitCode;

	};

//定义传入给main函数的回调函数RunApplication, 真正的程序入口
#if defined(SAPPHIRE_WIN) || defined(SAPPHIRE_LINUX)
#define SAPPHIRE_APPLICATION_MAIN(className) \
int RunApplication() \
		{ \
    Sapphire::SharedPtr<Sapphire::Core> pCore(new Sapphire::Core()); \
    Sapphire::SharedPtr<className> application(new className(pCore)); \
    return application->Run(); \
		} \
	SAPPHIRE_MAIN(RunApplication());

#endif

}
