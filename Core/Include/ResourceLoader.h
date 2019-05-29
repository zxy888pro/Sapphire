#pragma once
#include "Sapphire.h"
#include "Thread.h"
#include "BaseResource.h"


namespace Sapphire
{
	class SAPPHIRE_CLASS ResourceLoader :public BaseObject, public Thread
	{
		SAPPHIRE_OBJECT(ResourceLoader, BaseObject)

	public: 
		ResourceLoader(Core* pCore);
		virtual ~ResourceLoader();

		void Initialized();
		void LoadResource(const char* resourcePath, ResoureType type, ResourceEventHandler* phandler);  //º”‘ÿ◊ ‘¥
		void Release();

	private:

		void Dispatch();   

	public:
		virtual void ThreadFunc() override;

		virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL) override;

	};

}