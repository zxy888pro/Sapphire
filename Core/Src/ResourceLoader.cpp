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
		SubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCEREQUEST_EVENT);	//��Դ��������
		SubscribeEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCELOADEND_EVENT);  //��Դ�������
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
			if(m_resource->Load(m_resource->GetName().c_str())) //������Դ
			{
				m_resource->OnLoadEnd(); //������Դ������ɺ���
				m_resource->FireEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_RESOURCELOADEND_EVENT, m_resource); //������Դ��������¼�
			}else
			{
				m_resource->OnLoadError();
			}
			
		}
	}

}