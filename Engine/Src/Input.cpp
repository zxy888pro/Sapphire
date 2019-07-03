#include "Input.h"
#include "IGraphicDriver.h"
#include "IDisplayContext.h"
//�����Windows/Linuxʹ��GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
#include <GLFW/glfw3.h>
#else
//����Androidƽ̨ʹ��EGL

#endif

namespace Sapphire
{
//�����Windows/Linuxʹ��GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)

	void keyCallback(GLFWwindow* window, int keycode, int scancode,int action, int mods)
	{

	}

	void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
	{

	}

	void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
	{

	}

	void OnMouseButton(GLFWwindow* window, int button, int state, int mods)
	{

	}


	void Input::Initialize()
	{

		m_pGraphicDriver = (IGraphicDriver*)m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER);
		if (!m_pGraphicDriver || !m_pGraphicDriver->IsInitialized())
		{
			SAPPHIRE_LOGERROR("Input Initialize Error! graphicDriver has not initialized!");
			return;
		}
		//��ȡOpenGL���ڶ���
		void* window = m_pGraphicDriver->GetDisplayContext()->GetWindow();

		if (!window)
		{
			SAPPHIRE_LOGERROR("Input Init Error! window is null");
			return;
		}
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_BEGINFRAME);	//ע��֡��ʼ�¼�
		
		//ע��glfw�ص�����
		glfwSetKeyCallback((GLFWwindow*)window, keyCallback); //��������ص�
		glfwSetCursorPosCallback((GLFWwindow*)window, OnMouseMove); //����ƶ��ص�
		glfwSetScrollCallback((GLFWwindow*)window, OnMouseScroll); //�����ֻص�
		glfwSetMouseButtonCallback((GLFWwindow*)window, OnMouseButton); //��갴���ص�

		ResetState();
	}

	void Input::Release()
	{
		UnSubscribeEvent(ET_CORE_EVENT, EVENT_CORE_BEGINFRAME);
	}

	void Input::Update()
	{
		
		glfwPollEvents();
	}

	

#else
	//����Androidƽ̨ʹ��EGL

#endif

	void Input::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	{
		if (eEventType == ET_CORE_EVENT)
		{
			switch (eEvent)
			{
			case EVENT_CORE_BEGINFRAME:
				{

				}
				break;
			default:
				break;
			}
		}
		
	}
	

}