#include "Input.h"
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

	void Input::Update()
	{
		void* window = NULL;//���ھ��,��GraphicDriver��ȡ


		
		glfwSetKeyCallback((GLFWwindow*)window, keyCallback); //��������ص�

		glfwPollEvents();
	}

#else
	//����Androidƽ̨ʹ��EGL

#endif
	

}