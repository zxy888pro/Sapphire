#pragma once

#include <SapphireDef.h>
#include <EventContext.h>

namespace Sapphire
{

	struct IEventMgr
	{
		virtual void BroadcastEvent(ushort eEventType, ushort eEvent, void* eventData = NULL) = NULL;
		virtual bool SubscribeEvent(ushort eEventType, ushort eEvent, EventContext* subsriber) = NULL;
		virtual bool UnSubscribeEvent(ushort eEventType, ushort eEvent, EventContext* subsriber) = NULL;
	};

}