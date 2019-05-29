#include "Sapphire.h"
#include "EventMgr.h"
#include <IEventMgr.h>

namespace Sapphire
{

	EventMgr::EventMgr()
	{

	}

	EventMgr::~EventMgr()
	{

	}

	void EventMgr::BroadcastEvent(ushort eEventType, ushort eEvent, EventContext* eEventSrc, void* eventData)
	{
		
		//找到对应类型的SubscriberMap
		EventDataMap::iterator evtTypeMap = m_listenerMap.find(eEventType);
		if (evtTypeMap == m_listenerMap.end())
		{
			return;
		}
		EventSubcriberArrayMap::iterator evtMap = m_listenerMap[eEventType].find(eEvent);
		if (evtMap == m_listenerMap[eEventType].end())
		{
			return;
		}
		EventSubcribers& subcribers = m_listenerMap[eEventType][eEvent];
		for (EventSubcribers::iterator it = subcribers.begin(); it != subcribers.end(); ++it)
		{
			if (*it != NULL)
			{
				(*it)->Invoke(eEventType, eEvent, eEventSrc, eventData);
			}
		}
		
	}

	bool EventMgr::SubscribeEvent(ushort eEventType, ushort eEvent, EventContext* pSubcriber)
	{
		if (pSubcriber == NULL)
			return false;
		EventDataMap::iterator evtTypeMap = m_listenerMap.find(eEventType);
		if (evtTypeMap == m_listenerMap.end())
		{
			m_listenerMap.insert(std::make_pair(eEventType, EventSubcriberArrayMap()));
		}
		EventSubcriberArrayMap::iterator evtMap = m_listenerMap[eEventType].find(eEvent);
		if (evtMap == m_listenerMap[eEventType].end())
		{
			m_listenerMap[eEventType].insert(std::make_pair(eEvent, EventSubcribers()));
		}
		EventSubcribers& subcribs = m_listenerMap[eEventType][eEvent];
		if (std::find(subcribs.begin(), subcribs.end(), pSubcriber) == subcribs.end())
		{
			subcribs.push_back(pSubcriber);
			return true;
		}
		else
		{
			SAPPHIRE_LOG("SubscribeEvent Failed, Subscriber has existed!");
		}
		return false;
	}

	bool EventMgr::UnSubscribeEvent(ushort eEventType, ushort eEvent, EventContext* pSubscriber)
	{
		if (pSubscriber == NULL)
			return false;
		EventDataMap::iterator evtTypeMap = m_listenerMap.find(eEventType);
		if (evtTypeMap == m_listenerMap.end())
		{
			return false;
		}
		EventSubcriberArrayMap::iterator evtMap = m_listenerMap[eEventType].find(eEvent);
		if (evtMap == m_listenerMap[eEventType].end())
		{
			return false;
		}
		EventSubcribers& subcribers = m_listenerMap[eEventType][eEvent];
		EventSubcribers::iterator subcriber = std::find(subcribers.begin(), subcribers.end(), pSubscriber);
		if (subcriber != subcribers.end())
		{
			subcribers.erase(subcriber);
			return true;
		}
		return false;
	}

}