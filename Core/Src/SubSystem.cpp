#include <SubSystem.h>

namespace Sapphire
{

	std::string SubSystem::GetName()
	{
		return ENUM2STR(m_type);
	}

}