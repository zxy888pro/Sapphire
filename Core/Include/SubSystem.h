#pragma once
#include <SapphireDef.h>
#include <EventContext.h>

namespace Sapphire
{
	enum ESubSystemType
	{
		ESST_MEMORYMGR,
		ESST_RESOURCEMGR,
		ESST_WORKERQUEUE,
		ESST_RENDERSYSTEM,
		ESST_GRAPHICDRIVER,
		ESST_UNKNOWSYSTEM,
		ESST_MAXCOUNT
	};

#define SAPPHIRE_SUBSYSTEM(Type) \
	public: static std::string GetTypeNameStatic() { static std::string typeName(#Type); return typeName; };

	class SAPPHIRE_CLASS SubSystem :public EventContext
	{
	public:

		SubSystem()
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