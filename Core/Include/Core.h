#ifndef __SAPPHIRE_CORE_H__
#define __SAPPHIRE_CORE_H__
#include "SapphireDef.h"
#include <singleton.h>
#include <Ptr.h>
#include <SubSystem.h>
#include <IEventMgr.h>

namespace Sapphire
{

	bool Is64bitSystem();

	class ResourceContainer;
	class MemoryManager;

	 SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count);
	 SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint   data, int count);

	 class SAPPHIRE_CLASS Core : public Singleton<Core>
	 {
		 typedef std::unordered_map<std::string, SubSystem*> SUBSYTEM_MAP;
		 typedef std::unordered_map<std::string, SubSystem*>::iterator   SUBSYTEM_ITEM;
		 typedef std::unordered_map<std::string, SubSystem*>::const_iterator   SUBSYTEM_CONST_ITEM;

	 public:

		 Core();
		 virtual ~Core();
		 void Init();
		 void Release();
		 IEventMgr*  GetEventManager(){ return m_eventMgr; }

		 SubSystem*  GetSubSystemWithName(std::string name) const;
		 SubSystem*  GetSubSystemWithType(ESubSystemType type) const;
		 template<class T>  T* GetSubSystem() const;
		 template<class T> void RegisterSubSystem(T* ptr, ESubSystemType type);
		 
		 //更新所有子系统
		 void Update();

		 //ResourceMgr*  GetResourceManager();
		 //Sapphire::MemoryManager* GetMemoryMgr();

	 private:

		 //以后子系统通通放到SUBSYTEM_MAP中, 用一个通用模板函数取到
		 //资源管理器
		 /* ResourceMgr*  m_resMgr;
		  MemoryManager*  m_memMgr;*/

		 SUBSYTEM_MAP m_subSystems;
		 
		 //事件系统作为核心系统，不在子系统中
		 IEventMgr*      m_eventMgr;
		 


	 };


	 template <class T> T* Core::GetSubSystem() const { return GetSubSystemWithName(T::GetTypeNameStatic());}
	 
	 template<class T>
	 void Core::RegisterSubSystem(T* ptr, ESubSystemType type)
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
			 return;
		 default:
			 return;
		 }
		 if (m_subSystems.find(name) != m_subSystems.end())
			 return;
		 m_subSystems[name] = dynamic_cast<SubSystem*>(ptr);
	 }

}

#endif // !__SAPPHIRE_CORE_H__
