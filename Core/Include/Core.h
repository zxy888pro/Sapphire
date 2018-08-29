#ifndef __SAPPHIRE_CORE_H__
#define __SAPPHIRE_CORE_H__
#include "SapphireDef.h"
#include <singleton.h>
#include <BaseResource.h>

namespace Sapphire
{

	bool Is64bitSystem();

	 SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count);
	 SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint   data, int count);

	 class SAPPHIRE_API Core:public Singleton<Core>
	 {
	 public:
		 Core();
		 virtual ~Core();

		 ResourceMgr*  GetResourceManager() { return m_resMgr; }

	 private:

		 //资源管理器
		 ResourceMgr*  m_resMgr;


	 };

}

#endif // !__SAPPHIRE_CORE_H__
