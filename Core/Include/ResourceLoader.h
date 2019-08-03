#pragma once
#include "Sapphire.h"
#include "AsynTaskPool.h"
#include "BaseResource.h"


namespace Sapphire
{
	//��Դ��������
	class SAPPHIRE_CLASS ResourceTask : public AsynTask
	{
	public:
		ResourceTask(BaseResource* res, bool bAutoDelete = false);
		virtual ~ResourceTask();

		virtual void run() override;

	private:

		WeakPtr<BaseResource>  m_resource; //��������

	};

	//��Դ��������
	class SAPPHIRE_CLASS ResourceLoader : public SubSystem
	{
		SAPPHIRE_OBJECT(ResourceLoader, SubSystem)

	public:

		ResourceLoader(Core* pCore);
		virtual ~ResourceLoader();

		void LoadResource(BaseResource* resource, bool bAsysn = false);

		void Initialize();

		void Release();


		virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL) override;

	protected:

	protected:

		
	 

	};

	

}