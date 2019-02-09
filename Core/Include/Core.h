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

	class ResourceMgr;
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

		 SubSystem*  GetSubSystemWithName(std::string name);
		 SubSystem*  GetSubSystemWithType(ESubSystemType type);
		 template<class T>  T* GetSubSystem();
		 template<class T> void RegisterSubSystem(T* ptr, ESubSystemType type);
		 
		 //����������ϵͳ
		 void Update();

		 ResourceMgr*  GetResourceManager();
		 Sapphire::MemoryManager* GetMemoryMgr();

	 private:

		 //�Ժ���ϵͳͨͨ�ŵ�SUBSYTEM_MAP��, ��һ��ͨ��ģ�庯��ȡ��
		 //��Դ������
		 ResourceMgr*  m_resMgr;
		 MemoryManager*  m_memMgr;

		 SUBSYTEM_MAP m_subSystems;
		 
		 //�¼�ϵͳ��Ϊ����ϵͳ��������ϵͳ��
		 IEventMgr*      m_eventMgr;
		 


	 };


	 template <class T> T* Core::GetSubSystem(){ return GetSubSystemWithName(T::GetTypeNameStatic());}
	 
	 template<class T>
	 void Core::RegisterSubSystem(T* ptr, ESubSystemType type)
	 {
		 std::string name = ENUM2STR(type);
		 if (m_subSystems.find(name) != m_subSystems.end())
			 return;
		 m_subSystems[name] = dynamic_cast<SubSystem*>(ptr);
	 }

}

#endif // !__SAPPHIRE_CORE_H__
