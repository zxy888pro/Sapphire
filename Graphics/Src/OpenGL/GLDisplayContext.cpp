#include "GLDisplayContext.h"
//�����Windows/Linuxʹ��GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
#include <GLFW/glfw3.h>
#else
//����Androidƽ̨ʹ��EGL

#endif

namespace Sapphire
{

	GLDisplayContext::GLDisplayContext() :m_mainWindow(NULL)
	{

	}

	GLDisplayContext::~GLDisplayContext()
	{

	}


	

	//�����Windows/Linuxʹ��GLFW
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
		//�������û�����ʹ������Ѵ����Ͳ�������
		if (m_mainWindow == NULL)
		{
			SAPPHIRE_LOG("GLDisplayContext::CreateNativeWindow");
#ifndef GL_ES_VERSION_2_0  //OpenGLES 2.0

			SetContextAttribute(GLFW_RED_BITS, 8);
			SetContextAttribute(GLFW_GREEN_BITS, 8);
			SetContextAttribute(GLFW_BLUE_BITS, 8);
			SetContextAttribute(GLFW_DEPTH_BITS, 24);   //24λ���
			SetContextAttribute(GLFW_STENCIL_BITS, 8);  //8λģ��
#endif

			int monitorCount;  //�Ȼ�ȡ��ʾ������
			GLFWmonitor** pMonitors = glfwGetMonitors(&monitorCount);
			SAPPHIRE_LOG(StringFormat("monitorCount = %d", monitorCount));
			GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor(); //��ȡ����ʾ��

			m_mainWindow = glfwCreateWindow(width, height, wndName, pPrimaryMonitor, NULL);
			if (m_mainWindow == NULL)
			{
				SAPPHIRE_LOGERROR("GLDisplayContext CreateWindow Failed!");
				return;
			}
		}
		else
		{
			//�Ѵ��ڣ�ֻ��������
			glfwSetWindowSize((GLFWwindow*)m_mainWindow, width, height);
		}

		if (!bFullScreen)
			glfwSetWindowPos((GLFWwindow*)m_mainWindow, x, y);

		if (bVsync)
			glfwSwapInterval(1);  //��ֱͬ��
		else
			glfwSwapInterval(0);

	}


	void GLDisplayContext::SwapBuffers()
	{
		if (m_mainWindow)
		{
			glfwSwapBuffers((GLFWwindow*)m_mainWindow);
			glfwPollEvents(); //���������¼�
		}
			


	}



#else
	//����Androidƽ̨ʹ��EGL

#endif




	

	

	

}