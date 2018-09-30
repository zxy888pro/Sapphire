#pragma once

#include <SapphireDef.h>
#include <Ptr.h>
#include <IEventMgr.h>
#include <EventContext.h>

namespace Sapphire
{
	class EventMgr : public IEventMgr
	{
	public:

		EventMgr();
		virtual ~EventMgr();

		virtual void BroadcastEvent(ushort eEventType, ushort eEvent, void* eventData = NULL);
		virtual bool SubscribeEvent(ushort eEventType, ushort eEvent, EventContext* subsriber);
		virtual bool UnSubscribeEvent(ushort eEventType, ushort eEvent, EventContext* subsriber);

	private:

		typedef std::unordered_map<ushort, std::unordered_map<ushort, std::vector<EventContext*>> > EventDataMap;
		typedef std::unordered_map<ushort, std::vector<EventContext*>>  EventSubcriberArrayMap;
		typedef std::vector<EventContext*> EventSubcribers;

		EventDataMap  m_listenerMap;

	};

}