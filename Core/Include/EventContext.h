#pragma once

#include <SapphireDef.h>
#include <map>
#include <sigslot/sigslot.h>
#include "Str.h"

namespace Sapphire
{
	struct IEventMgr;
	class  BaseObject;

	class SAPPHIRE_CLASS EventContext : public sigslot::has_slots<>
	{
	public:
		friend class Core;
		EventContext();
		virtual ~EventContext();
	
		virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL);
		void FireEvent(ushort eEventType, ushort eEvent, void* eventData = NULL);
		void SubscribeEvent(ushort eEventType, ushort eEvent);
		void UnSubscribeEvent(ushort eEventType, ushort eEvent);

	protected:

	 

	private:
		typedef std::map<ushort, std::map<ushort, bool>> SUBSCRIBE_INFO;

		SUBSCRIBE_INFO m_subsInfo;
		IEventMgr*  m_eventMgr;
		
	};

	//事件处理器
	class SAPPHIRE_CLASS EventHandler
	{
	public:

		EventHandler(BaseObject* recevier):
			m_recevier(recevier),
			m_sender(0),
			m_eventType(0),
			m_event(0)
		{

		};

		EventHandler(BaseObject* recevier, void* usrData) :m_recevier(recevier),
			m_sender(0),
			m_eventType(0),
			m_event(0),
			m_usrData(usrData)
		{

		}

		void	SetEvent(ushort eEventType, ushort eEvent)
		{
			
			m_eventType = eEventType;
			m_event = eEvent;
		}

		virtual  void Invoke(void* eventData) = 0;

		BaseObject* GetReceiver() const { return m_recevier; }

		BaseObject* GetSender() const { return m_sender; }

		ushort		GetEventType() const { return m_eventType; }

		ushort		GetEvent() const { return m_event; }

		void*    GetUserData() const { return m_usrData; }

	protected:

		BaseObject*		 m_recevier;
		BaseObject*		 m_sender;
		ushort			 m_eventType;
		ushort			 m_event;
		void*			 m_usrData;

	};

	template <class T>
	class SAPPHIRE_CLASS EventHandlerImpl : public EventHandler
	{
	public:
		//定义一个指向成员函数的指针
		typedef void (T::*HandlerFunctionPtr)(const char*, void*);

		EventHandlerImpl(T* receiver, HandlerFunctionPtr function) :
			EventHandler(receiver),
			myFunction(function)
		{
			assert(myFunction);
		}

		EventHandlerImpl(T* receiver, HandlerFunctionPtr function, void* usrData) :
			EventHandler(receiver, eventData),
			myFunction(function)
		{
			assert(myFunction);
		}

		virtual void Invoke(void* eventData)
		{
			T* receiver = static_cast<T*>(m_recevier);
			(receiver->*myFunction)(m_eventType, eventData);
		}

	private:

		//事件函数指针
		HandlerFunctionPtr myFunction;
	};

#define SAPPHIRE_HANDLER(className, function) (new Urho3D::EventHandlerImpl<className>(this, &className::function))
#define SAPPHIRE_HANDLER_USERDATA(className, function, userData) (new Urho3D::EventHandlerImpl<className>(this, &className::function, userData))
	
}