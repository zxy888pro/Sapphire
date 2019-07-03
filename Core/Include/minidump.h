#pragma once

#include "Sapphire.h"

namespace Sapphire
{
#if defined(_MSC_VER) && defined(SAPPHIRE_MINIDUMPS)
	
	SAPPHIRE_API int WriteMiniDump(const char* applicationName, void* exceptionPointers);
#endif
}

