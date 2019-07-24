#ifndef __SAPPHIRE_CORE_H__
#define __SAPPHIRE_CORE_H__

#include "SapphireDef.h"
#include <singleton.h>
#include <Ptr.h>
#include <SubSystem.h>
#include <IEventMgr.h>
#include "Str.h"

namespace Sapphire
{

	bool Is64bitSystem();

	class ResourceContainer;
	class MemoryManager;
	class UIDCreator;

	 SAPPHIRE_API void Mem_Set_WORD(void *dest, ushort data, int count);
	 SAPPHIRE_API void Mem_Set_QUAD(void *dest, uint   data, int count);

	 class SAPPHIRE_CLASS Core : public RefCounter, public Singleton<Core>
	 {
		 typedef std::unordered_map<std::string, SubSystem*> SUBSYTEM_MAP;
		 typedef std::unordered_map<std::string, SubSystem*>::iterator   SUBSYTEM_ITEM;
		 typedef std::unordered_map<std::string, SubSystem*>::const_iterator   SUBSYTEM_CONST_ITEM;
		 typedef std::unordered_map<StringHash, SharedPtr<ObjectFactory>, StringHashFunc, StringHashCMP>   OBJECTFACTORY_MAP;
		 typedef std::unordered_map<StringHash, SharedPtr<ObjectFactory>, StringHashFunc, StringHashCMP>::iterator   OBJECTFACTORY_MAP_ITERATOR;
		 typedef std::unordered_map<StringHash, SharedPtr<ObjectFactory>, StringHashFunc, StringHashCMP>::const_iterator   OBJECTFACTORY_MAP_CITERATOR;

	 public:

		 Core();
		 virtual ~Core();
		 void Init();
		 void Release();
		 UINT64 GenUID();

		 /// 注册一个对象工厂
		 void RegisterFactory(ObjectFactory* factory);
		 /// 注册一个对象工厂 (带目录)
		 void RegisterFactory(ObjectFactory* factory, const char* category);

		 const std::string GetTypeName(StringHash objectType) const;

		 IEventMgr*  GetEventManager(){ return m_eventMgr; }

		 SubSystem*  GetSubSystemWithName(std::string name) const;
		 SubSystem*  GetSubSystemWithType(ESubSystemType type) const;
		 //template<class T>  SubSystem* GetSubSystem() const;
		 template<class T>  T* GetSubSystem() const;
		 template<class T> void RegisterSubSystem(T* ptr, ESubSystemType type);
		 
		 //更新所有子系统
		 void Update();

		 //ResourceMgr*  GetResourceManager();
		 //Sapphire::MemoryManager* GetMemoryMgr();

 
		 template <class T> void RegisterFactory();
	 
		 template <class T> void RegisterFactory(const char* category);

	 private:

		 //以后子系统通通放到SUBSYTEM_MAP中, 用一个通用模板函数取到
		 //资源管理器
		 /* ResourceMgr*  m_resMgr;
		  MemoryManager*  m_memMgr;*/

		 SUBSYTEM_MAP m_subSystems;
		 std::unordered_map<std::string, std::string> m_subSystemNameMap;
		 
		 //事件系统作为核心系统，不在子系统中
		 IEventMgr*      m_eventMgr;
		 UIDCreator*     m_uidCreator;

		 //对象目录
		 std::unordered_map<std::string, std::vector<StringHash>> m_objCategories;
		 //对象工厂
		 OBJECTFACTORY_MAP  m_objFactories;
		 


	 };


	 template <class T> void Core::RegisterFactory() { RegisterFactory(new ObjectFactoryImpl<T>(this)); }

	 template <class T> void Core::RegisterFactory(const char* category)
	 {
		 RegisterFactory(new ObjectFactoryImpl<T>(this), category);
	 }


	 
	 template <class T> T* Core::GetSubSystem() const { return dynamic_cast<T*>(GetSubSystemWithName(T::GetClassTypeNameStatic())); }
	 //template <class T> SubSystem* Core::GetSubSystem() const { return GetSubSystemWithName(T::GetClassTypeNameStatic()); }
	 
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
			 return;
		 default:
			 return;
		 }
		 if (m_subSystems.find(name) != m_subSystems.end())
			 return;
		 m_subSystems[name] = dynamic_cast<SubSystem*>(ptr);
		 std::string className = T::GetClassTypeNameStatic();
		 m_subSystemNameMap[className] = name;
	 }

}

#endif // !__SAPPHIRE_CORE_H__
