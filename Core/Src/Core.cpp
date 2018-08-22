#include "Sapphire.h"
#include "Core.h"
#ifdef SAPPHIRE_WIN
#include <windows.h>
#include <sysinfoapi.h>
#else

#endif

namespace Sapphire
{
	bool Is64bitSystem()
	{
#ifdef SAPPHIRE_WIN
		SYSTEM_INFO si;
		GetNativeSystemInfo(&si);
		if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||
			si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64)
			return TRUE;
		else
			return FALSE;
#else
		return false;
#endif

	}

	SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count)
	{
		//该函数一次填充16位字节内存
#ifdef _MSC_VER
		_asm
		{
			mov edi, dest; //EDI指针指向目标内存地址 
			mov ecx, count; //将移动次数放入ECX 
			mov ax, data; //将16位数据放入AX
			rep stosw; //rep指令是重复上述操作,直到ECX为0，每执行一次ECX-1；STOS是将EAX值拷贝到ES:edi指向的地址; 如果设置了direction flag, 那么edi会在该指令执行后减小, 如果没有设置direction flag, 那么edi的值会增加.
		}
#else
		memset(dest, data, size);
#endif
	}

	///////////////////////////////////////////////////////////

	SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint data, int count)
	{
#ifdef _MSC_VER
		// 该函数一次填充32位字节内存
		_asm
		{
			mov edi, dest;
			mov ecx, count;
			mov eax, data;
			rep stosd;
		}
#else
		memset(dest, data, size);
#endif
	}

	Core::Core()
	{
		m_resMgr = new ResourceMgr();
	}

	Core::~Core()
	{
		m_resMgr->Clear();

		safeDelete(m_resMgr);
	}


}

 