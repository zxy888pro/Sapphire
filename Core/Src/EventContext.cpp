#include "EventContext.h"
#include <IEventMgr.h>
#include <Core.h>

namespace Sapphire
{

	EventContext::EventContext()
	{
		if (Core::GetSingletonPtr())
		{
			m_eventMgr = Core::GetSingletonPtr()->GetEventManager();
		}
		else
		{
			m_eventMgr = NULL;
		}

		
	}


	EventContext::~EventContext()
	{
		SUBSCRIBE_INFO::iterator evtTypeIt = m_subsInfo.begin();
		while (evtTypeIt != m_subsInfo.end())
		{
			std::map<ushort, bool> map = evtTypeIt->second;
			for (std::map<ushort, bool>::iterator evtIt = map.begin(); evtIt != map.end(); ++evtIt)
			{
				if (evtIt->second == true)
				{
					//m_eventMgr->UnSubscribeEvent(evtTypeIt->first, evtIt->second, this);
					UnSubscribeEvent(evtTypeIt->first, evtIt->second);
				}
			}
			++evtTypeIt;
		}
	}

	void EventContext::FireEvent(ushort eEventType, ushort eEvent, void* eventData)
	{
		m_eventMgr->BroadcastEvent(eEventType, eEvent, eventData);
	}

	void EventContext::SubscribeEvent(ushort eEventType, ushort eEvent)
	{
		m_eventMgr->SubscribeEvent(eEventType, eEvent, this);
	}

	void EventContext::UnSubscribeEvent(ushort eEventType, ushort eEvent)
	{
		m_eventMgr->UnSubscribeEvent(eEventType, eEvent, this);
	}

	void EventContext::Invoke(ushort eEventType, ushort eEvent, void* eventData /*= NULL*/)
	{
		//事件处理
	}

}