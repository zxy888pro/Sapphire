#pragma once

#include <SapphireDef.h>
#include <map>
#include <sigslot/sigslot.h>

namespace Sapphire
{
	class IEventMgr;

	class EventContext : public sigslot::has_slots<>
	{
	public:
		friend class Core;
		EventContext();
		virtual ~EventContext();
	
		virtual void Invoke(ushort eEventType, ushort eEvent, void* eventData = NULL);
		void FireEvent(ushort eEventType, ushort eEvent, void* eventData = NULL);
		void SubscribeEvent(ushort eEventType, ushort eEvent);
		void UnSubscribeEvent(ushort eEventType, ushort eEvent);

	private:
		typedef std::map<ushort, std::map<ushort, bool>> SUBSCRIBE_INFO;

		SUBSCRIBE_INFO m_subsInfo;
		IEventMgr*  m_eventMgr;
		Core*    m_pCore;
	};


}