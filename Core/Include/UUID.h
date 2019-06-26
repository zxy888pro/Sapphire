#pragma once

#include "Sapphire.h"
#ifdef SAPPHIRE_WIN
#include <objbase.h>
#elif defined(SAPPHIRE_ANDROID)
#include <uuid/uuid.h>
#elif defined(SAPPHIRE_LINUX)
#include <uuid/uuid.h>
#endif
 

namespace Sapphire
{ 

	class SAPPHIRE_CLASS UUID
	{
	public:
		UUID();
		~UUID();

		void CreateUUID();

		std::string ToString();
	private:
		GUID m_guid;
		
	};

	
	



}