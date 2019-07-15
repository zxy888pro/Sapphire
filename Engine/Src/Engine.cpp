#include "Engine.h"
#include "ResourceCache.h"
#include "AsynTaskPool.h"
#include "ResourceLoader.h"
#include "GraphicDriver.h"
#include "IGraphicDriver.h"
#include "IRenderSystem.h"

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

		ResourceLoader* resourceLoader = new ResourceLoader(pCore);
		AsynTaskPool* asynTaskPool = new AsynTaskPool(pCore, 8, 2, 100);
		ResourceCache* resourceCache = new ResourceCache(pCore);
		TimeSystem*    timeSystem = new TimeSystem(pCore);
		
		pCore->RegisterSubSystem(resourceLoader, ESubSystemType::ESST_RESOURCELOADER);
		pCore->RegisterSubSystem(asynTaskPool, ESubSystemType::ESST_ASYNTASKPOOL);
		pCore->RegisterSubSystem(resourceCache, ESubSystemType::ESST_RESOURCECACHE);
		pCore->RegisterSubSystem(timeSystem, ESubSystemType::ESST_TIMESYSTEM);
		pCore->RegisterSubSystem(this, ESubSystemType::ESST_ENGINE);

		resourceLoader->Initialize();
		asynTaskPool->Initialize();

		
	}

	Engine::~Engine()
	{

	}

	bool Engine::Initialize(const VariantMap& parameters)
	{
		if (m_bInitialized)
			return;

		createGraphicDriver(m_pCore);
		createRenderSystem(m_pCore);

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

	}

	void Engine::Update()
	{

		VariantMap eventData;
		eventData["TimeStep"] = m_uTimeStep;
		FireEvent(ET_CORE_EVENT, EVENT_CORE_UPDATE, &eventData);

		// Logic post-update event
		FireEvent(ET_CORE_EVENT, EVENT_CORE_POSTUPDATE, &eventData);

		// Rendering update event
		FireEvent(ET_CORE_EVENT, EVENT_CORE_RENDERUPDATE, &eventData);

		// Post-render update event
		FireEvent(ET_CORE_EVENT, EVENT_CORE_POSTRENDERUPDATE, &eventData);
	}

	void Engine::Render()
	{
		IGraphicDriver* pGraphicDriver =  dynamic_cast<IGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
		IRenderSystem*	pRenderSystem  =  dynamic_cast<IRenderSystem*>(m_pCore->GetSubSystemWithType(ESST_RENDERSYSTEM));
		if (pGraphicDriver && pGraphicDriver->BeginFrame())
		{
			pRenderSystem->Render();
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

	}

}