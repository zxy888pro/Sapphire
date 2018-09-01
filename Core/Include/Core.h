#ifndef __SAPPHIRE_CORE_H__
#define __SAPPHIRE_CORE_H__
#include "SapphireDef.h"
#include <singleton.h>

namespace Sapphire
{

	bool Is64bitSystem();

	class ResourceMgr;
	class MemoryManager;

	 SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count);
	 SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint   data, int count);

	 class SAPPHIRE_CLASS Core:public Singleton<Core>
	 {
	 public:
		 Core();
		 virtual ~Core();
		 void Init();
		 void Release();

		 ResourceMgr*  GetResourceManager();
		 Sapphire::MemoryManager* GetMemoryMgr();

	 private:

		 //资源管理器
		 ResourceMgr*  m_resMgr;
		 MemoryManager*  m_memMgr;


	 };

}

#endif // !__SAPPHIRE_CORE_H__
