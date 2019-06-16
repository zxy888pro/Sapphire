#include "ResourceLoader.h"
#include "AsynTaskPool.h"


namespace Sapphire
{

	 

	ResourceLoader::ResourceLoader(Core* pCore) :SubSystem(pCore)
	{
		
	}

	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::LoadResource(BaseResource* resource)
	{
		if (resource)
		{
			m_assert(m_pCore);
			AsynTaskPool* pTaskPool = dynamic_cast<AsynTaskPool*>(m_pCore->GetSubSystemWithType(ESST_ASYNTASKPOOL));
			m_assert(pTaskPool);
			ResourceTask* pResTask = new ResourceTask(resource, true);
			pTaskPool->AddTask(pResTask);

		}
	}

	void ResourceLoader::Initialize()
	{
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCEREQUEST_EVENT);	//��Դ��������
		SubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT);  //��Դ�������
	}



	void ResourceLoader::Release()
	{
		UnSubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCEREQUEST_EVENT);
		UnSubscribeEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT);
	}

	void ResourceLoader::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	{
		switch (eEvent)
		{
		case EVENT_CORE_RESOURCEREQUEST_EVENT:
		{
			if (eventData)
			{
				BaseResource* res = (BaseResource*)eventData;
				SAPPHIRE_LOG(StringFormatA("���� %s !", res->GetName().c_str()));
				LoadResource(res);
			}
		}
		case EVENT_CORE_RESOURCELOADEND_EVENT:
		{
			if (eventData)
			{
				BaseResource* res = (BaseResource*)eventData;
				SAPPHIRE_LOG(StringFormatA("%s �������!", res->GetName().c_str()));
			}
		}
			break;
		default:
			break;
		}
	}

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
			if(m_resource->Load()) //������Դ
			{
				m_resource->OnLoadEnd(); //������Դ������ɺ���
				m_resource->FireEvent(ET_CORE_EVENT, EVENT_CORE_RESOURCELOADEND_EVENT, m_resource); //������Դ��������¼�
			}else
			{
				m_resource->OnLoadError();
			}
			
		}
	}

}