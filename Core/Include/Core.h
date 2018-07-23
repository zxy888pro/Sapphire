#ifndef __SAPPHIRE_CORE_H__
#define __SAPPHIRE_CORE_H__
#include "Sapphire.h"

namespace Sapphire
{

	bool Is64bitSystem();

	inline SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count);
	inline SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint   data, int count);

}

#endif // !__SAPPHIRE_CORE_H__
