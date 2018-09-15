#include "stdafx.h"
#include <Sapphire.h>
#include <Str.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include "Camera.h"
#include <MemoryMgr.h>
#include "BaseMesh.h"
#include "BaseLightMesh.h"
#include "BaseLightMapMesh.h"
#include "BaseEmissionMesh.h"
#include <Math/Vector2.h>
#include <Color.h>
#include <Variant.h>
#include "BaseScene.h"


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
GLFWwindow* window = NULL;
Sapphire::BaseLightMesh* pMesh;
Sapphire::BaseLightMapMesh* pMesh2;
Sapphire::BaseEmissionMesh* pMesh3;
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
		Sapphire::Core::GetSingletonPtr()->GetMemoryMgr()->dumpLogReport();
		release();
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

	

	Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
	Sapphire::LogUtil::LogMsgLn("初始化程序");
	new Sapphire::Core();
	new Sapphire::GraphicDriver();


	Sapphire::GraphicDriver::GetSingletonPtr()->Init();

	Sapphire::IImageMgr* pImgMgr = Sapphire::GraphicDriver::GetSingletonPtr()->getImageMgr();
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
	//捕捉鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{

		Sapphire::LogUtil::LogMsgLn(Sapphire::StringFormatA("glew init error! errorCode:! %d", glGetError()));
		glfwTerminate();
	}

	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	pScene = new Sapphire::Scene();
}

void release()
{
	Sapphire::Camera* pcam = Sapphire::Camera::GetSingletonPtr();
	safeDelete(pcam);
	Sapphire::GraphicDriver* pGd = Sapphire::GraphicDriver::GetSingletonPtr();
	pGd->Release();
	safeDelete(pGd);
	Sapphire::Core* pCore = Sapphire::Core::GetSingletonPtr();
	pCore->Release();
	safeDelete(pCore);
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

void TestMemoryTrack()
{
	using namespace Sapphire;

	/*int* p2 = _New int[5];
	_Delete[] p2;*/

	int* p1 = (int*)_Malloc(sizeof(int));
	*p1 = 5;
	_Free(p1);

	/*char* pszTmp = _New char[256];
	memset(pszTmp, 0, 256);
	_Delete[] pszTmp;
	*/
}

int main()
{
	using namespace Sapphire;

	init();
	TestMemoryTrack();
	//pMesh3 = _New BaseEmissionMesh();
	////pMesh3->LoadBaseShader("shaders\\BaseEmissionLightVs.glsl", "shaders\\BaseEmissionLightFs.glsl");
	//pMesh3->Init();
	//pMesh3->setColor(glm::vec3(1.0, 0.3, 0.3));
	//pMesh = new BaseLightMesh();
	////pMesh->LoadBaseShader("shaders\\BaseLightVs.glsl", "shaders\\BaseLightFs.glsl");
	//pMesh->Init();
	//pMesh->setLightPos(glm::vec3(3, 3, 2));
	//pMesh2 = new BaseLightMapMesh();
	////pMesh2->LoadBaseShader("shaders\\BaseLightMapVs.glsl", "shaders\\BaseLightMapFs.glsl");
	//pMesh2->Init();
	//pMesh2->SetDiffuseMap("container2.png");
	//pMesh2->SetSepcularMap("container2_specular.png");
	//pMesh2->setLightPos(glm::vec3(2, 1, 2));
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		ProcessInput(window);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		pScene->Upate();
		pScene->Render();
		/*pMesh->Render();
		pMesh2->Render();
		pMesh3->Render();*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	release();



	return 0;
}