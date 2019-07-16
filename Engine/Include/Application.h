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

		//�������ʼ��֮ǰ�����ø��ֹ��������������ڷǳ�ʼ��ʱ��������
		virtual void Setup() { }
		//��ʼ�����棬����ѭ����ʼ֮ǰ
		virtual void Start() { }

		//��ѭ�������������
		virtual void Stop() { }

		//��ѭ��
		virtual int Run();

		//������Ի����˳�
		void ErrorExit(const std::string& message = "");

	protected:

		SharedPtr<Engine> m_engine;

		VariantMap m_engineParameter;

		std::string m_startupErrors;

		int m_exitCode;

	};

//���崫���main�����Ļص�����RunApplication, �����ĳ������
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
