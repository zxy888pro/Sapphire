#include "ResourceLoader.h"



namespace Sapphire
{

	ResourceLoader::ResourceLoader(Core* pCore) :BaseObject(pCore)
	{

	}

	ResourceLoader::~ResourceLoader()
	{

	}

	void ResourceLoader::Initialized()
	{
		SubscribeEvent(ET_OTHER_EVENT, EVENT_SUBSYS_RESOURCEREQUEST_EVENT);
	}

	void ResourceLoader::LoadResource(const char* resourcePath, ResoureType type, ResourceEventHandler* phandler)
	{

	}

	void ResourceLoader::Release()
	{
		UnSubscribeEvent(ET_OTHER_EVENT, EVENT_SUBSYS_RESOURCEREQUEST_EVENT);
	}

	void ResourceLoader::Dispatch()
	{

	}

	void ResourceLoader::ThreadFunc()
	{
		Dispatch();
	}

	void ResourceLoader::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	{
		
	}

}