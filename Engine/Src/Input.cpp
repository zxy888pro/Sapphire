#include "Input.h"
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

	void Input::Update()
	{
		void* window = NULL;//窗口句柄,从GraphicDriver获取


		
		glfwSetKeyCallback((GLFWwindow*)window, keyCallback); //按键输入回调

		glfwPollEvents();
	}

#else
	//否则Android平台使用EGL

#endif
	

}