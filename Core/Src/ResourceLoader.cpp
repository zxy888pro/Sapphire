#include "ResourceLoader.h"
#include "AsynTaskPool.h"


namespace Sapphire
{

	 

	ResourceLoader::ResourceLoader(Core* pCore) :SubSystem(pCore)
	{

	}

	void ResourceLoader::LoadResource(SharedPtr<BaseResource> resource)
	{
		if (resource.NotNull())
		{
			m_assert(m_pCore);
			AsynTaskPool* pTaskPool = dynamic_cast<AsynTaskPool*>(m_pCore->GetSubSystem<AsynTaskPool>());
			ResourceTask* pResTask = new ResourceTask(resource, true);
			pTaskPool->AddTask(pResTask);

		}
	}

	void ResourceLoader::Initialize()
	{
		SubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCEREQUEST_EVENT);	//资源加载请求
		SubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCELOADEND_EVENT);  //资源加载完成
	}



	void ResourceLoader::Release()
	{
		UnSubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCEREQUEST_EVENT);
		UnSubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCELOADEND_EVENT);
	}

	ResourceTask::ResourceTask(SharedPtr<BaseResource> res, bool bAutoDelete /*= false*/)
	{
		m_resource = res;
		m_bAutoDelete = bAutoDelete;
	}

	void ResourceTask::run()
	{
		if (m_resource.NotNull())
		{
			m_resource->OnLoadStart();
			if(m_resource->Load(m_resource->GetName().c_str())) //加载资源
			{
				m_resource->OnLoadEnd(); //进行资源加载完成后处理
				m_resource->FireEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCELOADEND_EVENT, m_resource); //发送资源加载完成事件
			}else
			{
				m_resource->OnLoadError();
			}
			
		}
	}

}