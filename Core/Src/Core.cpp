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
#include "UIDCreator.h"

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
		m_uidCreator = new UIDCreator();
		m_eventMgr = new EventMgr();
	}

	Core::~Core()
	{
		safeDelete(m_uidCreator);
		safeDelete(m_eventMgr);
	}


 

	void Core::Init()
	{
		/*m_memMgr = new MemoryManager();
		m_resMgr = new ResourceMgr();
		m_memMgr->initialize();*/
	}

	void Core::Release()
	{
		
		for (SUBSYTEM_ITEM it = m_subSystems.begin(); it != m_subSystems.end(); it++)
		{
			SubSystem* pSubSystem = it->second;
			if (pSubSystem)
			{
				safeDelete(pSubSystem);
			}
		}
		m_subSystems.clear();
		/*m_resMgr->Clear();
		safeDelete(m_resMgr);
		m_memMgr->release();
		safeDelete(m_memMgr);*/
	}

	UINT64 Core::GenUID()
	{
		return m_uidCreator->GetNewUID();
	}

	void Core::RegisterFactory(ObjectFactory* factory)
	{
		if (!factory)
			return;

		m_objFactories[factory->GetType()] = factory;
	}

	void Core::RegisterFactory(ObjectFactory* factory, const char* category)
	{
		if (!factory)
			return;

		RegisterFactory(factory);
		if (strlen(category))  //如果目录名有效
			m_objCategories[category].push_back(factory->GetType());
	}

	const std::string Core::GetTypeName(StringHash objectType) const
	{
		//从对象工厂中找对象获取其类型名
		OBJECTFACTORY_MAP_CITERATOR it = m_objFactories.find(objectType);
		return it != m_objFactories.end() ? it->second->GetTypeName() : "";
	}

	Sapphire::SubSystem* Core::GetSubSystemWithClassName(std::string name) const
	{
		SubSystem* pss = NULL;
		std::unordered_map<std::string, std::string>::const_iterator i = m_subSystemNameMap.find(name);
		if (i != m_subSystemNameMap.end())
		{
			SUBSYTEM_CONST_ITEM it = m_subSystems.find(i->second);
			if (it == m_subSystems.end())
			{
				return pss;
			}
			pss = it->second;
		}
		return pss;
	}

	Sapphire::SubSystem* Core::GetSubSystemWithName(std::string name) const
	{
		SUBSYTEM_CONST_ITEM it = m_subSystems.find(name);
		if (it == m_subSystems.end())
		{
			return NULL;
		}
		return it->second;
	}

	Sapphire::SubSystem* Core::GetSubSystemWithType(ESubSystemType type) const
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
		case Sapphire::ESST_ASYNTASKPOOL:
			name = ENUM2STR(ESST_ASYNTASKPOOL);
			break;
		case Sapphire::ESST_RESOURCELOADER:
			name = ENUM2STR(ESST_RESOURCELOADER);
			break;
		case Sapphire::ESST_RESOURCECACHE:
			name = ENUM2STR(ESST_RESOURCECACHE);
			break;
		case Sapphire::ESST_TIMESYSTEM:
			name = ENUM2STR(ESST_TIMESYSTEM);
			break;
		case Sapphire::ESST_FILESYSTEM:
			name = ENUM2STR(ESST_FILESYSTEM);
			break;
		case Sapphire::ESST_INPUTSYSTEM:
			name = ENUM2STR(ESST_INPUTSYSTEM);
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

	/*Sapphire::ResourceMgr* Core::GetResourceManager()
	{
	return m_resMgr;
	}

	Sapphire::MemoryManager* Core::GetMemoryMgr()
	{
	return m_memMgr;
	}
	*/
}

 