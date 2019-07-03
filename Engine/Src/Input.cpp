#include "Input.h"
#include "IGraphicDriver.h"
#include "IDisplayContext.h"
//如果是Windows/Linux使用GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
#include <GLFW/glfw3.h>
#else
//否则Android平台使用EGL

#endif

namespace Sapphire
{
//如果是Windows/Linux使用GLFW
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
		//获取OpenGL窗口对象
		void* window = m_pGraphicDriver->GetDisplayContext()->GetWindow();

		if (!window)
		{
			SAPPHIRE_LOGERROR("Input Init Error! window is null");
			return;
		}
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_BEGINFRAME);	//注册帧开始事件
		
		//注册glfw回调函数
		glfwSetKeyCallback((GLFWwindow*)window, keyCallback); //按键输入回调
		glfwSetCursorPosCallback((GLFWwindow*)window, OnMouseMove); //鼠标移动回调
		glfwSetScrollCallback((GLFWwindow*)window, OnMouseScroll); //鼠标滚轮回调
		glfwSetMouseButtonCallback((GLFWwindow*)window, OnMouseButton); //鼠标按键回调

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
	//否则Android平台使用EGL

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