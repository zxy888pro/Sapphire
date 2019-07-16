#include "Engine.h"
#include "ResourceCache.h"
#include "AsynTaskPool.h"
#include "ResourceLoader.h"
#include "GraphicDriver.h"
#include "IGraphicDriver.h"
#include "IRenderSystem.h"
#include "FileSystem.h"
#include "Input.h"

namespace Sapphire
{


	Engine::Engine(Core* pCore) :BaseObject(pCore),
		m_uMinFps(10),
#if defined(SAPPHIRE_ANDROID) 
		m_uMaxFps(60),
		m_uMaxInactiveFps(10),
		m_bPauseMinimized(false),
#else
		m_uMaxFps(200),
		m_uMaxInactiveFps(60),
		m_bPauseMinimized(false),
#endif
		m_uTimeStep(0),
		m_uTimeStepSmoothing(2),
		
		m_bInitialized(false),
		m_bExiting(false)
	{
		//初始化Log
		Sapphire::LogUtil::getInstancePtr()->Init("log.txt");
		Sapphire::LogUtil::LogMsgLn("初始化程序");
		ResourceLoader* resourceLoader = new ResourceLoader(pCore);
		AsynTaskPool* asynTaskPool = new AsynTaskPool(pCore, 8, 2, 100);
		ResourceCache* resourceCache = new ResourceCache(pCore);
		TimeSystem*    timeSystem = new TimeSystem(pCore);
		FileSystem*    fileSystem = new FileSystem(pCore);
		Input*         inputsys = new Input(pCore);
		
		pCore->RegisterSubSystem(resourceLoader, ESubSystemType::ESST_RESOURCELOADER);
		pCore->RegisterSubSystem(asynTaskPool, ESubSystemType::ESST_ASYNTASKPOOL);
		pCore->RegisterSubSystem(resourceCache, ESubSystemType::ESST_RESOURCECACHE);
		pCore->RegisterSubSystem(fileSystem, ESubSystemType::ESST_FILESYSTEM);
		pCore->RegisterSubSystem(timeSystem, ESubSystemType::ESST_TIMESYSTEM);
		pCore->RegisterSubSystem(inputsys, ESubSystemType::ESST_INPUTSYSTEM);
		pCore->RegisterSubSystem(this, ESubSystemType::ESST_ENGINE);

		resourceLoader->Initialize();
		asynTaskPool->Initialize();
		inputsys->Initialize();

		
	}

	Engine::~Engine()
	{

	}

	bool Engine::Initialize(const VariantMap& parameters)
	{
		if (m_bInitialized)
			return m_bInitialized;

		//初始化渲染引擎
		createGraphicDriver(m_pCore);
		createRenderSystem(m_pCore);

		

		TimeSystem* timeSys = dynamic_cast<TimeSystem*>(m_pCore->GetSubSystemWithType(ESST_TIMESYSTEM));
		m_assert(timeSys);
		timeSys->SetTimerPeriod(1);

		//文件系统初始化
		FileSystem* fileSys = dynamic_cast<FileSystem*>(m_pCore->GetSubSystemWithType(ESST_FILESYSTEM));
		m_assert(fileSys);
		{
			//设置程序目录，资源目录，配置目录等
			//添加资源包
			
		}

		IGraphicDriver* pGraphicDriver = dynamic_cast<IGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
		m_assert(pGraphicDriver);
		IRenderSystem* pRenderSys = dynamic_cast<IRenderSystem*>(m_pCore->GetSubSystemWithType(ESST_RENDERSYSTEM));
		m_assert(pRenderSys);
		{
			//设置图形参数


		}
		


		m_frameTimer.Reset();
		m_bInitialized = true;
		return m_bInitialized;
	}

	Sapphire::VariantMap Engine::ParseParameters(const std::vector<std::string>& arguments)
	{
		VariantMap ret;
		return ret;
	}

	bool Engine::HasParameter(const VariantMap& parameters, const String& parameter)
	{
		return parameters.find(parameter.str()) != parameters.end();
	}

	const Sapphire::Variant
		& Engine::GetParameter(const VariantMap& parameters, const String& parameter, const Variant& defaultValue /*= Variant::EMPTY*/)
	{

		VariantMap::const_iterator it = parameters.find(parameter.str());
		if (it != parameters.end())
		{
			return it->second;
		}
		return defaultValue;
		
	}

	void Engine::RunFrame()
	{
		m_assert(m_bInitialized);
		TimeSystem* timeSys = dynamic_cast<TimeSystem*>(m_pCore->GetSubSystemWithType(ESST_TIMESYSTEM));
		m_assert(timeSys);
		IGraphicDriver* pGraphicDriver = dynamic_cast<IGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
		m_assert(pGraphicDriver);
		Input* pInputSys = dynamic_cast<Input*>(m_pCore->GetSubSystemWithType(ESST_INPUTSYSTEM));
		m_assert(pInputSys);

		timeSys->BeginFrame(m_uTimeStep);
		if (m_bPauseMinimized && pInputSys->IsWindowMinimized())
		{

		}
		else
		{
			Update();
		}
		Render();
		ApplyFrameLimit();  //帧数限制
		timeSys->EndFrame();
	}

	void Engine::Update()
	{

		VariantMap eventData;
		eventData["TimeStep"] = m_uTimeStep;
		FireEvent(ET_CORE_EVENT, EVENT_CORE_UPDATE, &eventData);

		// 逻辑更新事件
		FireEvent(ET_CORE_EVENT, EVENT_CORE_POSTUPDATE, &eventData);

		// 渲染更新事件
		FireEvent(ET_CORE_EVENT, EVENT_CORE_RENDERUPDATE, &eventData);

		// 后渲染更新事件
		FireEvent(ET_CORE_EVENT, EVENT_CORE_POSTRENDERUPDATE, &eventData);
	}

	void Engine::Render()
	{
		IGraphicDriver* pGraphicDriver =  dynamic_cast<IGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
		IRenderSystem*	pRenderSystem  =  dynamic_cast<IRenderSystem*>(m_pCore->GetSubSystemWithType(ESST_RENDERSYSTEM));
		if (pGraphicDriver && pGraphicDriver->BeginFrame())
		{
			pRenderSystem->Render();
			//绘制UI

		}


		pGraphicDriver->EndFrame();
	}

	void Engine::Exit()
	{

	}

	void Engine::ApplyFrameLimit()
	{

	}

	void Engine::SetNextTimeStep(float seconds)
	{

	}

	void Engine::DoExit()
	{
		IGraphicDriver* pGraphicDriver = dynamic_cast<IGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
		if (pGraphicDriver)
		{
			
		}
		m_bExiting = true;
	}

}