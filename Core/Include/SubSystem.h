#pragma once
#include <SapphireDef.h>
#include <EventContext.h>
#include <BaseObject.h>

namespace Sapphire
{
	enum ESubSystemType
	{
		ESST_MEMORYMGR,
		ESST_RESOURCECACHE,
		ESST_RESOURCEMGR,
		ESST_WORKERQUEUE,
		ESST_RENDERSYSTEM,
		ESST_GRAPHICDRIVER,
		ESST_ASYNTASKPOOL,
		ESST_RESOURCELOADER,
		ESST_INPUTSYSTEM,
		ESST_TIMESYSTEM,
		ESST_FILESYSTEM,
		ESST_LOGGER,
		ESST_UNKNOWSYSTEM,
		ESST_MAXCOUNT
	};


	class SAPPHIRE_CLASS SubSystem :public BaseObject
	{
		SAPPHIRE_OBJECT(SubSystem, BaseObject)

	public:

		SubSystem(Core* pCore):
			BaseObject(pCore)
		{
			m_type = ESST_UNKNOWSYSTEM;

			 
		}
		 
		ESubSystemType GetType() const { return m_type; }
		std::string GetName();

		virtual void Update();

	protected:

		ESubSystemType m_type;
	};

	 

}