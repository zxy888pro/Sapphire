#include "stdafx.h"
#include <windows.h>
#include <Sapphire.h>
#include <Str.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include "IGraphicDriver.h"
#include "IRenderSystem.h"
#include "Camera.h"
#include <MemoryMgr.h>
#include "BaseLight.h"
#include "BaseMesh.h"
#include "BaseLightMesh.h"
#include "BaseLightMapMesh.h"
#include "SkyBoxMesh.h"
#include "BaseEnviromentMapMesh.h"
#include "BaseUBOSimpleMesh.h"
#include "BaseScene.h"
#include <json/json.h>

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
		Sapphire::Core::GetSingletonPtr()->GetMemoryMgr()->dumpMemoryAllocations();
		Sapphire::Core::GetSingletonPtr()->GetMemoryMgr()->dumpLogReport();
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

	int x = 1022;
	uint mask = ~3;
	x &= ~3;
	Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
	Sapphire::LogUtil::LogMsgLn("初始化程序");
	new Sapphire::Core();
	

	MathHelper::SetRandomSeed(GetTickCount());
	createGraphicDriver();

	IGraphicDriver* pDriver = dynamic_cast<IGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
	Sapphire::IImageMgr* pImgMgr = pDriver->getImageMgr();
	new Sapphire::Camera();

	IRenderSystem* pRenderSys = (IRenderSystem*)Sapphire::Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_RENDERSYSTEM);
	pRenderSys->setDrawShadow(true);

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
	SharedPtr<BaseLight> pLight1 = SharedPtr<BaseLight>(new BaseLight());
	pScene->AddLight(pLight1);
	pLight1->setAmbient(glm::vec3(0.1, 0.2, 0.1));
	pLight1->setDiffuse(glm::vec3(1.0, 0.3, 0.3));
	pLight1->setSpecular(glm::vec3(1.0, 0.3, 0.3));
	pLight1->setLinear(0.5);
	pLight1->setQuadratic(0.055);
	pLight1->setPos(glm::vec3(0.0, 4.0, 0.0));

	SharedPtr<BaseLight> pLight2 = SharedPtr<BaseLight>(new BaseLight());
	pLight2->setType(Sapphire::LT_POINT);
	pScene->AddLight(pLight2);
	pLight2->setAmbient(glm::vec3(0.1, 0.2, 0.1));
	pLight2->setDiffuse(glm::vec3(1.0, 1.0, 0.3));
	pLight2->setSpecular(glm::vec3(-2.0, 3.0, 1.3));
	pLight2->setLinear(0.3);
	pLight2->setQuadratic(0.025);
	pLight2->setPos(glm::vec3(1.0, 0, 3.0));


	{
		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new BaseLightMesh());
		pMesh->setPos(glm::vec3(0.0, 0.0, 4.0));
		SharedPtr<BaseLightMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		pSMesh->setColor(glm::vec3(1.0, 0.0, 0.0));
		pScene->AddMesh("LightBox1", pMesh);
	}

	{
		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new BaseLightMapMesh());
		pMesh->setPos(glm::vec3(2.0, 0.0, 6.0));
		SharedPtr<BaseLightMapMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		pSMesh->setColor(glm::vec3(0.8, 0.32, 0.42));
		pSMesh->setShinniess(12);
		pScene->AddMesh("LightBox2", pMesh);
		pSMesh->SetDiffuseMap("container2.png");
		pSMesh->SetSepcularMap("container2_specular.png");
		
	}

	{
		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new BaseUBOSimpleMesh());
		pMesh->setPos(glm::vec3(7.0, 1.0, 6.0));
		SharedPtr<BaseUBOSimpleMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		pSMesh->setColor(glm::vec3(0.4, 0.1, 0.42));
		pScene->AddMesh("uboBox", pMesh);

	}

	{
		/*SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new SkyBoxMesh());
		SharedPtr<SkyBoxMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		pScene->AddMesh("SkyBox", pMesh);
		pSMesh->LoadCubeMap("images\\peaks.cub.json");*/
		pScene->SetSkyBox("images\\peaks.cub.json");

		SharedPtr<BaseMesh> pMesh = SharedPtr<BaseMesh>(new BaseEnviromentMapMesh());
		pMesh->setPos(glm::vec3(8.0, 2.0, 4.0));
		SharedPtr<BaseEnviromentMapMesh> pSMesh;
		pSMesh.DynamicCast(pMesh);
		pScene->AddMesh("enviromapBox", pMesh);
		pSMesh->SetCubeMap(pScene->GetSkyBoxMap());
		
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