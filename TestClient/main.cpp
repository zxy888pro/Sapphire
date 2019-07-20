#include "stdafx.h"
#include "Sapphire.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "Include/GLTest01.h"
#include "GLTest02.h"


void ProcessInput(GLFWwindow* pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_TRUE)
	{
		glfwTerminate();
	}
}


int main()
{

	Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
	Sapphire::LogUtil::LogMsgLn("³õÊ¼»¯³ÌÐò");


	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	

	GLFWwindow* window = glfwCreateWindow(800, 600, "test", NULL, NULL);

	if (window == NULL)
	{
		Sapphire::LogUtil::LogMsgLn("glfw CreateWindow Failed!");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		
		Sapphire::LogUtil::LogMsgLn(Sapphire::StringFormatA("glew init error! errorCode:! %d", glGetError()));
		glfwTerminate();
	}
	GLenum ret = glewInit();
	GLTest02* test = new GLTest02();
	
	glViewport(0, 0, 800, 600);
	test->Init("texVs.glsl","texFs.glsl");
	test->CreateRenderInfo();
	while (!glfwWindowShouldClose(window))
	{
		ProcessInput(window);
		test->Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete test;
	glfwTerminate();

	return 0;
}