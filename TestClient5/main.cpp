#include "stdafx.h"
#include <windows.h>
#include <Sapphire.h>
#include <Str.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include "IGraphicDriver.h"
#include "Camera.h"
#include <MemoryMgr.h>
#include <json/json.h>
#include "BaseScene.h"
#include "BaseUIMesh.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
GLFWwindow* window = NULL;
Sapphire::Scene* pScene;


void OnMouseMove(GLFWwindow* window, double xpos, double ypos);
void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset);
void OnScreenSizeChanged(GLFWwindow* window, int width, int height);
void release();

void ProcessInput(GLFWwindow* pWnd)
{
	if (glfwGetKey(pWnd, GLFW_KEY_ESCAPE) == GLFW_TRUE)
	{
		glfwTerminate();
		/*Sapphire::Core::GetSingletonPtr()->GetMemoryMgr()->dumpMemoryAllocations();
		Sapphire::Core::GetSingletonPtr()->GetMemoryMgr()->dumpLogReport();*/
		return;
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

void init()
{
	using namespace Sapphire;


	Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
	Sapphire::LogUtil::LogMsgLn("��ʼ������");
	new Sapphire::Core();
	createGraphicDriver();

	MathHelper::SetRandomSeed(GetTickCount());
	IGraphicDriver* pDriver = dynamic_cast<IGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
	Sapphire::IImageMgr* pImgMgr = pDriver->getImageMgr();
	new Sapphire::Camera();

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "test", NULL, NULL);

	if (window == NULL)
	{
		Sapphire::LogUtil::LogMsgLn("glfw CreateWindow Failed!");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, OnScreenSizeChanged);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetScrollCallback(window, OnMouseScroll);
	//��׽���
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{

		Sapphire::LogUtil::LogMsgLn(Sapphire::StringFormatA("glew init error! errorCode:! %d", glGetError()));
		glfwTerminate();
	}
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	pScene = new Scene();
	pScene->Initialize();
	pScene->setEnablePost(false);
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


void Prepare()
{
	using namespace Sapphire;
	{
		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new BaseUIMesh());
		pMesh->setPos(glm::vec3(0.0, 0.0, 4.0));
		SharedPtr<BaseUIMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		//pSMesh->setColor(glm::vec3(1.0, 0.0, 0.0));
		pScene->AddMesh("test GS", pMesh);
	}
}

void Update()
{
	pScene->Upate();
}

void Render()
{
	pScene->Render();
	 
}

int main()
{
	using namespace Sapphire;

	init();

	Prepare();

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ProcessInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		Update();
		Render();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}