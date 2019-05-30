#pragma once
#include "Sapphire.h"
#include "AsynTaskPool.h"
#include "BaseResource.h"


namespace Sapphire
{
	//资源加载任务
	class SAPPHIRE_CLASS ResourceTask : public AsynTask
	{
	public:
		ResourceTask(SharedPtr<BaseResource> res,bool bAutoDelete = false);
		virtual ~ResourceTask();

		virtual void run() override;

	private:

		WeakPtr<BaseResource>  m_resource; //用弱引用

	};

	//资源加载任务
	class SAPPHIRE_CLASS ResourceLoader : public SubSystem
	{
		SAPPHIRE_OBJECT(ResourceLoader, SubSystem)

	public:

		ResourceLoader(Core* pCore);
		virtual ~ResourceLoader();

		void LoadResource(SharedPtr<BaseResource> resource);

		void Initialize();

		void Release();

	protected:


	 

	};

	

}