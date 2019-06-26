#include "UUID.h"
#ifdef SAPPHIRE_WIN
#include <objbase.h>
#elif defined(SAPPHIRE_ANDROID)
#include <uuid/uuid.h>
#elif defined(SAPPHIRE_LINUX)
#include <uuid/uuid.h>
#endif

namespace Sapphire
{

	 

	UUID::UUID()
	{

	}

	UUID::~UUID()
	{

	}

	void UUID::CreateUUID()
	{
#ifdef SAPPHIRE_WIN
		
		::CoCreateGuid(&m_guid);
#elif defined(SAPPHIRE_ANDROID)
		uuid_generate(reinterpret_cast<unsigned char *>(&m_guid));
#elif defined(SAPPHIRE_LINUX)
		uuid_generate(reinterpret_cast<unsigned char *>(&m_guid));
#endif
	}

	std::string UUID::ToString()
	{
		char buf[64] = { 0 };
#ifdef __GNUC__
		snprintf(
#else // MSVC
		_snprintf_s(
#endif
			buf,
			sizeof(buf),
			"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
			m_guid.Data1, m_guid.Data2, m_guid.Data3,
			m_guid.Data4[0], m_guid.Data4[1],
			m_guid.Data4[2], m_guid.Data4[3],
			m_guid.Data4[4], m_guid.Data4[5],
			m_guid.Data4[6], m_guid.Data4[7]);
		return std::string(buf);

	}

}