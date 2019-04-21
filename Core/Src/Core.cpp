#include <SapphireDef.h>
#include "Core.h"
#ifdef SAPPHIRE_WIN
#include <windows.h>
#include <sysinfoapi.h>
#else

#endif
#include <Sapphire.h>
#include <BaseResource.h>
#include <MemoryMgr.h>
#include "EventMgr.h"

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
		
	}

	Core::~Core()
	{
		
	}


 

	void Core::Init()
	{
		
		m_eventMgr = new EventMgr();
		m_memMgr = new MemoryManager();
		m_resMgr = new ResourceMgr();
		m_memMgr->initialize();
	}

	void Core::Release()
	{
		m_resMgr->Clear();
		safeDelete(m_resMgr);
		m_memMgr->release();
		safeDelete(m_memMgr);
		m_subSystems.clear();
	}

	Sapphire::SubSystem* Core::GetSubSystemWithName(std::string name)
	{
		SubSystem* pss = NULL;
		SUBSYTEM_ITEM it = m_subSystems.find(name);
		if (it == m_subSystems.end())
		{
			return pss;
		}
		pss = it->second;
		return pss;
	}

	Sapphire::SubSystem* Core::GetSubSystemWithType(ESubSystemType type)
	{
		std::string name;
		switch (type)
		{
		case Sapphire::ESST_MEMORYMGR:
			name = ENUM2STR(ESST_MEMORYMGR);
			break;
		case Sapphire::ESST_RESOURCEMGR:
			name = ENUM2STR(ESST_RESOURCEMGR);
			break;
		case Sapphire::ESST_WORKERQUEUE:
			name = ENUM2STR(ESST_WORKERQUEUE);
			break;
		case Sapphire::ESST_RENDERSYSTEM:
			name = ENUM2STR(ESST_RENDERSYSTEM);
			break;
		case Sapphire::ESST_GRAPHICDRIVER:
			name = ENUM2STR(ESST_GRAPHICDRIVER);
			break;
		case Sapphire::ESST_UNKNOWSYSTEM:
			name = ENUM2STR(ESST_UNKNOWSYSTEM);
			break;
		case Sapphire::ESST_MAXCOUNT:
			return NULL;
		default:
			return NULL;
		}
		return GetSubSystemWithName(name);
	}

	void Core::Update()
	{
		for (SUBSYTEM_MAP::iterator it = m_subSystems.begin(); it != m_subSystems.end(); ++it)
		{
			if (it->second != NULL)
			{
				it->second->Update();
			}
		}
	}

	Sapphire::ResourceMgr* Core::GetResourceManager()
	{
		return m_resMgr;
	}

	Sapphire::MemoryManager* Core::GetMemoryMgr()
	{
		return m_memMgr;
	}

}

 