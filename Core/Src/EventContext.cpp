#include "EventContext.h"
#include <IEventMgr.h>
#include <Core.h>
#include "Variant.h"

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
		m_eventMgr->BroadcastEvent(eEventType, eEvent, this, eventData);
	}

	void EventContext::SubscribeEvent(ushort eEventType, ushort eEvent, EventHandler* handler)
	{
		auto i = m_eventHandlerMap.find(eEventType);
		if (i != m_eventHandlerMap.end())
		{
			auto& evtTypeMap = i->second;
			auto k = evtTypeMap.find(eEvent);
			if (k != evtTypeMap.end())
			{
				if (k->second != handler)
					evtTypeMap[eEvent] = handler;
			}
			else
			{
				evtTypeMap[eEvent] = handler;
			}
		}
		else
		{
			m_eventHandlerMap.insert(std::make_pair(eEventType, std::map<ushort, SharedPtr<EventHandler>>()));
			m_eventHandlerMap[eEventType][eEvent] = handler;
		}
		m_eventMgr->SubscribeEvent(eEventType, eEvent, this);
	}

	void EventContext::UnSubscribeEvent(ushort eEventType, ushort eEvent)
	{
		auto i = m_eventHandlerMap.find(eEventType);
		if (i != m_eventHandlerMap.end())
		{
			auto k = i->second.find(eEvent);
			if (k != i->second.end())
			{
				i->second.erase(k);
			}
		}
		m_eventMgr->UnSubscribeEvent(eEventType, eEvent, this);
	}

	void EventContext::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	{
		//事件处理
		auto i = m_eventHandlerMap.find(eEventType);
		if (i != m_eventHandlerMap.end())
		{
			auto k = i->second.find(eEvent);
			if (k != i->second.end())
			{
				k->second->Invoke(eventData);
			}
		}
	}


}