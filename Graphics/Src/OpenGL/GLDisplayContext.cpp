#include "GLDisplayContext.h"
//如果是Windows/Linux使用GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
#include <GLFW/glfw3.h>
#else
//否则Android平台使用EGL

#endif

namespace Sapphire
{

	GLDisplayContext::GLDisplayContext() :m_mainWindow(NULL)
	{

	}

	GLDisplayContext::~GLDisplayContext()
	{

	}


	

	//如果是Windows/Linux使用GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
	
	void GLDisplayContext::Init()
	{

#include <GLFW/glfw3.h>
		glfwInit();
#ifndef GL_ES_VERSION_2_0
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif  

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	}

	void GLDisplayContext::SetContextAttribute(int attr, int val)
	{
		glfwWindowHint(attr, val);
	}


	void GLDisplayContext::CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, bool bVsync)
	{
		//如果窗口没创建就创建，已创建就不创建了
		if (m_mainWindow == NULL)
		{
			SAPPHIRE_LOG("GLDisplayContext::CreateNativeWindow");
#ifndef GL_ES_VERSION_2_0  //OpenGLES 2.0

			SetContextAttribute(GLFW_RED_BITS, 8);
			SetContextAttribute(GLFW_GREEN_BITS, 8);
			SetContextAttribute(GLFW_BLUE_BITS, 8);
			SetContextAttribute(GLFW_DEPTH_BITS, 24);   //24位深度
			SetContextAttribute(GLFW_STENCIL_BITS, 8);  //8位模板
#endif

			int monitorCount;  //先获取显示器数量
			GLFWmonitor** pMonitors = glfwGetMonitors(&monitorCount);
			SAPPHIRE_LOG(StringFormat("monitorCount = %d", monitorCount));
			GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor(); //获取主显示器

			m_mainWindow = glfwCreateWindow(width, height, wndName, pPrimaryMonitor, NULL);
			if (m_mainWindow == NULL)
			{
				SAPPHIRE_LOGERROR("GLDisplayContext CreateWindow Failed!");
				return;
			}
		}
		else
		{
			//已存在，只调整窗口
			glfwSetWindowSize((GLFWwindow*)m_mainWindow, width, height);
		}

		if (!bFullScreen)
			glfwSetWindowPos((GLFWwindow*)m_mainWindow, x, y);

		if (bVsync)
			glfwSwapInterval(1);  //垂直同步
		else
			glfwSwapInterval(0);

	}


	void GLDisplayContext::SwapBuffers()
	{
		if (m_mainWindow)
		{
			glfwSwapBuffers((GLFWwindow*)m_mainWindow);
			glfwPollEvents(); //处理所有事件
		}
			


	}



#else
	//否则Android平台使用EGL

#endif




	

	

	

}