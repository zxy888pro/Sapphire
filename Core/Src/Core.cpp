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
		//�ú���һ�����16λ�ֽ��ڴ�
#ifdef _MSC_VER
		_asm
		{
			mov edi, dest; //EDIָ��ָ��Ŀ���ڴ��ַ 
			mov ecx, count; //���ƶ���������ECX 
			mov ax, data; //��16λ���ݷ���AX
			rep stosw; //repָ�����ظ���������,ֱ��ECXΪ0��ÿִ��һ��ECX-1��STOS�ǽ�EAXֵ������ES:ediָ��ĵ�ַ; ���������direction flag, ��ôedi���ڸ�ָ��ִ�к��С, ���û������direction flag, ��ôedi��ֵ������.
		}
#else
		memset(dest, data, size);
#endif
	}

	///////////////////////////////////////////////////////////

	SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint data, int count)
	{
#ifdef _MSC_VER
		// �ú���һ�����32λ�ֽ��ڴ�
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

 