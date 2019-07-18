#include "GLDisplayContext.h"
#include "Math/Vector2.h"
//如果是Windows/Linux使用GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
#include <GLFW/glfw3.h>
#else
//否则Android平台使用EGL

#endif

namespace Sapphire
{

	GLDisplayContext::GLDisplayContext() :m_mainWindow(NULL),
		m_isTerminated(true),
		m_windowName("defaultWindow"),
		m_width(1024),
		m_height(768),
		m_posX(0),
		m_posY(0),
		m_bVsync(false),
		m_bFullScreen(false),
		m_multiSample(1),
		m_externalWindow(NULL)
	{

	}


	GLDisplayContext::~GLDisplayContext()
	{

	}


	

	//如果是Windows/Linux使用GLFW
#if defined(SAPPHIRE_WIN)||defined(SAPPHIRE_LINUX)
	
	void GLDisplayContext::Initialize()
	{

//		glfwInit();
//#ifndef GL_ES_VERSION_2_0
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//#endif  
//
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//		m_isTerminated = false;
	}

	void GLDisplayContext::SetContextAttribute(int attr, int val)
	{
		glfwWindowHint(attr, val);
	}


	void GLDisplayContext::CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, int m_multiSample, bool bVsync)
	{
		m_posX = x;
		m_posY = y;
		m_windowName = wndName;
		m_width = width;
		m_height = height;
		m_bFullScreen = bFullScreen;
		m_multiSample = m_multiSample;
		m_bVsync = bVsync;

		//如果窗口没创建就创建，已创建就不创建了
		if (m_mainWindow == NULL)
		{
			SAPPHIRE_LOG("GLDisplayContext::CreateNativeWindow");
			glfwInit();
			SetContextAttribute(GLFW_DOUBLEBUFFER, 1);

#ifndef GL_ES_VERSION_2_0 
			SetContextAttribute(GLFW_CONTEXT_VERSION_MAJOR, 3);
			SetContextAttribute(GLFW_CONTEXT_VERSION_MINOR, 3);
			SetContextAttribute(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			SetContextAttribute(GLFW_RED_BITS, 8);
			SetContextAttribute(GLFW_GREEN_BITS, 8);
			SetContextAttribute(GLFW_BLUE_BITS, 8);
			SetContextAttribute(GLFW_DEPTH_BITS, 24);   //24位深度
			SetContextAttribute(GLFW_STENCIL_BITS, 8);  //8位模板

			if(m_externalWindow) //嵌入的外部主窗口
			{
				SetContextAttribute(GLFW_ALPHA_BITS, 8);
			}
			else
			{
				SetContextAttribute(GLFW_ALPHA_BITS, 0);
			}

#else
			//OpenGLES 2.0
			SetContextAttribute(GLFW_CONTEXT_VERSION_MAJOR, 2);
			SetContextAttribute(GLFW_CONTEXT_VERSION_MINOR, 0);
			SetContextAttribute(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#endif
			if (m_multiSample > 1)
			{
				SetContextAttribute(GLFW_SAMPLES, m_multiSample);
				 
			}
			else
			{
				SetContextAttribute(GLFW_SAMPLES, 0);
			}

			if (m_bFullScreen)
			{
				m_posX = 0;
				m_posY = 0;
			}

			int monitorCount;  //先获取显示器数量
			GLFWmonitor** pMonitors = glfwGetMonitors(&monitorCount);
			SAPPHIRE_LOG(StringFormat("monitorCount = %d", monitorCount));
			GLFWmonitor* pPrimaryMonitor = glfwGetPrimaryMonitor(); //获取主显示器

			m_mainWindow = glfwCreateWindow(width, height, wndName, pPrimaryMonitor, NULL);
			if (m_mainWindow == NULL)
			{
				SAPPHIRE_LOGERROR("GLDisplayContext CreateWindow Failed!");
				Terminate();
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

		glfwMakeContextCurrent((GLFWwindow*)m_mainWindow);
		//捕捉鼠标
		glfwSetInputMode((GLFWwindow*)m_mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{

			SAPPHIRE_LOGERROR(Sapphire::StringFormatA("glew init error! errorCode:! %d", glGetError()));
			Terminate();
		}
		glViewport(0, 0, width, height);
		m_isTerminated = false;
	}


	void GLDisplayContext::CloseWindow()
	{

	}

	void GLDisplayContext::SwapBuffers()
	{
		if (m_mainWindow && !m_isTerminated)
		{
			glfwSwapBuffers((GLFWwindow*)m_mainWindow);
			glfwPollEvents(); //处理所有事件
		}
	}



	void GLDisplayContext::Terminate()
	{
		glfwTerminate();
		
		m_isTerminated = true;
	}

	void* GLDisplayContext::GetWindow()
	{
		return NULL;
	}

	void GLDisplayContext::GetCurrentDisplayMode(DisplayMode& mode) const
	{
		GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		if (pMonitor)
		{
			int count = 0;
			const GLFWvidmode* videoModes = glfwGetVideoMode(pMonitor);
			if (videoModes)
			{
				mode.width = videoModes->width;
				mode.height = videoModes->height;
				mode.redBits = videoModes->redBits;
				mode.greenBits = videoModes->greenBits;
				mode.blueBits = videoModes->blueBits;
				mode.refreshRate = videoModes->refreshRate;
				return;
			}
		}
		SAPPHIRE_LOGERROR("GetDisplayMode Failed!");
	}

	void GLDisplayContext::GetDisplayMode(int index, DisplayMode& mode) const
	{
		 GLFWmonitor* pMonitor = glfwGetPrimaryMonitor();
		 if (pMonitor)
		 {
			 int count = 0;
			 const GLFWvidmode* videoModes = glfwGetVideoModes(pMonitor, &count);
			 if (index >= 0 && index < count)
			 {
				 mode.width = videoModes[index].width;
				 mode.height = videoModes[index].height; 
				 mode.redBits = videoModes[index].redBits;
				 mode.greenBits = videoModes[index].greenBits;
				 mode.blueBits = videoModes[index].blueBits; 
				 mode.refreshRate = videoModes[index].refreshRate;
				 return;
			 }
		 }
		 SAPPHIRE_LOGERROR("GetDisplayMode Failed!");
		 
	}

	Sapphire::IntVector2 GLDisplayContext::GetWindowSize() const
	{
		IntVector2 ret = IntVector2::ZERO;
		if (m_mainWindow && !m_isTerminated)
		{
			glfwGetWindowSize((GLFWwindow*)m_mainWindow, &ret.x_, &ret.y_);
		}
		return ret;
	}

	Sapphire::IntVector2 GLDisplayContext::GetWindowPos() const
	{
		IntVector2 ret = IntVector2::ZERO;
		if (m_mainWindow && !m_isTerminated)
		{
			glfwGetWindowPos((GLFWwindow*)m_mainWindow, &ret.x_, &ret.y_);
			
		}
		return ret;
	}

	void GLDisplayContext::Clear(uint flags, Color& color, float depth, uint stencil)
	{

	}

	void GLDisplayContext::SetExternalWindow(void* val)
	{
		if (!m_externalWindow)
			m_externalWindow = val;
		else
			SAPPHIRE_LOGERROR("Window already opened, can not set external window");
	}

#else
	//否则Android平台使用EGL

#endif



	

	

	

}