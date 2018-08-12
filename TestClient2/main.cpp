#include "stdafx.h"
#include "Sapphire.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "Image.h"
#include "Shader.h"
#include "GLTestGeometry.h"
#include "Camera.h"
#include "GLUITest.h"
#include "SDFTest.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
Sapphire::GLTestGeometry* glTestGeo = NULL;
Sapphire::GLUITest* glUiTest = NULL;
Sapphire::SDFTest*  sdfTest = NULL;
Sapphire::Shader* shader = NULL;
Sapphire::Shader* textShader = NULL;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


void ProcessInput(GLFWwindow* pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_TRUE)
	{
		glfwTerminate();
	}
	if (glfwGetKey(pWnd, GLFW_KEY_W) == GLFW_PRESS)
		Sapphire::Camera::GetSingletonPtr()->ProcessKeyboard(Sapphire::FORWARD, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_S) == GLFW_PRESS)
		Sapphire::Camera::GetSingletonPtr()->ProcessKeyboard(Sapphire::BACKWARD, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_A) == GLFW_PRESS)
		Sapphire::Camera::GetSingletonPtr()->ProcessKeyboard(Sapphire::LEFT, deltaTime);
	if (glfwGetKey(pWnd, GLFW_KEY_D) == GLFW_PRESS)
		Sapphire::Camera::GetSingletonPtr()->ProcessKeyboard(Sapphire::RIGHT, deltaTime);
}

void Init()
{
	Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
	Sapphire::LogUtil::LogMsgLn("初始化程序");

	new Sapphire::ImageMgr();

	Sapphire::ImageMgr* pImgMgr = Sapphire::ImageMgr::GetSingletonPtr();
	new Sapphire::Camera();
	
}

void Release()
{
	delete Sapphire::ImageMgr::GetSingletonPtr();

	if (glTestGeo != NULL)
	{
		delete glTestGeo;
	}
	if (glUiTest != NULL)
	{
		delete glUiTest;
	}
	if (sdfTest != NULL)
	{
		delete sdfTest;
	}
	if (shader != NULL)
	{
		delete shader;
	}
	if (textShader != NULL)
	{
		delete textShader;
	}
	
	delete Sapphire::Camera::GetSingletonPtr();
}

void OnScreenSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	Sapphire::Camera::GetSingletonPtr()->ProcessMouseMovement(xoffset, yoffset);
}

void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	Sapphire::Camera::GetSingletonPtr()->ProcessMouseScroll(yoffset);
}

int main()
{
	
	Init();

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
	glfwSetFramebufferSizeCallback(window, OnScreenSizeChanged);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetScrollCallback(window, OnMouseScroll);
	//捕捉鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{

		Sapphire::LogUtil::LogMsgLn(Sapphire::StringFormatA("glew init error! errorCode:! %d", glGetError()));
		glfwTerminate();
	}
	
	shader = new Sapphire::Shader("test2Vs.glsl", "test2Fs.glsl", "");
	if (shader != NULL)
	{
		glTestGeo = new Sapphire::GLTestGeometry(shader);
	}
	textShader = new Sapphire::Shader("FontVs.glsl", "FontFs.glsl", "");
	if (shader != NULL)
	{
		glUiTest = new Sapphire::GLUITest(textShader);
	}
	
	sdfTest = new Sapphire::SDFTest();
	sdfTest->init("STXIHEI", 32, 32);
	
	glViewport(0, 0, 800, 600);
	glTestGeo->Init();
	glUiTest->Init(Sapphire::FontRenderMode::FONT_RENDER_MODE_OUTLINE, "fonts/arial.ttf", 32, 32);
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ProcessInput(window);
		glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glTestGeo->Render();
		glUiTest->RenderText("This is sample text", 125.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
		glUiTest->RenderText("(C) LearnOpenGL.com", 220.0f, 210.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	Release();

	return 0;
}