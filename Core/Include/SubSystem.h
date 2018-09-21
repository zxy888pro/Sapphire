#pragma once
#include <SapphireDef.h>
#include <sigslot/sigslot.h>

namespace Sapphire
{
	enum ESubSystemType
	{
		ESST_MEMORYMGR,
		ESST_RESOURCEMGR,
		ESST_WORKERQUEUE,
		ESST_UNKNOWSYSTEM,
		ESST_MAXCOUNT
	};

#define SAPPHIRE_SUBSYSTEM(Type) \
	public: static std::string GetTypeNameStatic() { static std::string typeName(#Type); return typeName; };

	class SAPPHIRE_CLASS SubSystem : sigslot::has_slots<>
	{
	public:

		SubSystem()
		{
			m_type = ESST_UNKNOWSYSTEM;
		}
		 
		ESubSystemType GetType() const { return m_type; }
		std::string GetName();


	protected:

		ESubSystemType m_type;
	};

	 

}