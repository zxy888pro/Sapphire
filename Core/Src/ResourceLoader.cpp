#include "ResourceLoader.h"
#include "AsynTaskPool.h"


namespace Sapphire
{



	ResourceLoader::ResourceLoader(Core* pCore) :SubSystem(pCore)
	{

	}

	ResourceLoader::~ResourceLoader()
	{
		Release();
	}

	void ResourceLoader::LoadResource(BaseResource* resource, bool bAsysn)
	{
		if (resource)
		{
			m_assert(m_pCore);
			if (bAsysn)
			{
				//異步加載
				AsynTaskPool* pTaskPool = m_pCore->GetSubSystem<AsynTaskPool>();
				m_assert(pTaskPool);
				ResourceTask* pResTask = new ResourceTask(resource, true);
				pTaskPool->AddTask(pResTask);
			}
			else
			{
				//同步加載
				resource->OnLoadStart();
				if (resource->Load())
					resource->OnLoadEnd();
				else
					resource->OnLoadError();
			}


		}
	}

	void ResourceLoader::Initialize()
	{
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCEREQUEST_EVENT, SAPPHIRE_HANDLER(ResourceLoader, OnResourceRequestEvent));	//资源加载请求
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT, SAPPHIRE_HANDLER(ResourceLoader, OnResourceLoadedEvent));  //资源加载完成
	}



	void ResourceLoader::Release()
	{
		UnSubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCEREQUEST_EVENT);
		UnSubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT);
	}



	void ResourceLoader::OnResourceRequestEvent(ushort eEventType, ushort eEvent, void* eventData)
	{
		if (eventData)
		{
			BaseResource* res = (BaseResource*)eventData;
			SAPPHIRE_LOG(StringFormatA("加载 %s !", res->GetName().c_str()));
			LoadResource(res);
		}

	}

	void ResourceLoader::OnResourceLoadedEvent(ushort eEventType, ushort eEvent, void* eventData)
	{
		if (eventData)
		{
			BaseResource* res = (BaseResource*)eventData;
			SAPPHIRE_LOG(StringFormatA("%s 加载完成!", res->GetName().c_str()));
		}
	}

	//void ResourceLoader::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	//{
	//	switch (eEvent)
	//	{
	//	case EVENT_CORE_RESOURCEREQUEST_EVENT:
	//	{
	//		if (eventData)
	//		{
	//			BaseResource* res = (BaseResource*)eventData;
	//			SAPPHIRE_LOG(StringFormatA("加载 %s !", res->GetName().c_str()));
	//			LoadResource(res);
	//		}
	//	}
	//	case EVENT_CORE_RESOURCELOADEND_EVENT:
	//	{
	//		if (eventData)
	//		{
	//			BaseResource* res = (BaseResource*)eventData;
	//			SAPPHIRE_LOG(StringFormatA("%s 加载完成!", res->GetName().c_str()));
	//		}
	//	}
	//		break;
	//	default:
	//		break;
	//	}
	//}

	ResourceTask::ResourceTask(BaseResource* res, bool bAutoDelete /*= false*/)
	{
		m_resource = res;
		m_bAutoDelete = bAutoDelete;
	}

	ResourceTask::~ResourceTask()
	{

	}

	void ResourceTask::run()
	{
		if (m_resource.NotNull())
		{
			m_resource->OnLoadStart();
			if (m_resource->Load()) //加载资源
			{
				m_resource->OnLoadEnd(); //进行资源加载完成后处理
				m_resource->FireEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT, m_resource); //发送资源加载完成事件
			}
			else
			{
				m_resource->OnLoadError();
			}

		}
	}

}