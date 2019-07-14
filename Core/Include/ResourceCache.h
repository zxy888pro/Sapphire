#pragma once


#include "SapphireDef.h"
#include "Thread.h"
#include <SubSystem.h>
#include "BaseResource.h"



namespace Sapphire
{

	//资源比较
	template <class T>
	class resPtr_greater
	{
	public:
		inline bool operator ()(T left, T right)
		{
			return !((*left) < (*right));
		}
	};

	typedef std::unordered_map<std::string, BaseResource*>  ResCMap;
	typedef ResCMap::iterator ResCMapItor;
	typedef ResCMap::value_type ResCMapPair;
	
	//资源缓存管理
	class SAPPHIRE_CLASS ResourceCache : public SubSystem
	{
		SAPPHIRE_OBJECT(ResourceCache, SubSystem)

	public:

		 
		ResourceCache(Core* pCore);
		virtual ~ResourceCache();

		void Clear();

		bool Create(uint nMaxSize);
		void Destroy();

		bool SetMaximumMemory(size_t nMem);  //设置资源管理器的最大占用内存上限
		size_t GetMaximumMemory()		{ return m_nMaximumMemory; }

		// 资源字典迭代

		//直接跨dll访问ResCMap会导致栈指针错误，必须通过包装间接访问才能保证安全
		//定位到第一个资源
		inline void GotoBegin()
		{
			m_CurrentResource = m_ResourceMap.begin();
		}
		//取得当前资源
		inline BaseResource* GetCurrentResource()
		{
			return (*m_CurrentResource).second;
		}
		//下一个资源
		inline bool GotoNext()
		{
			m_CurrentResource++;  return IsValid();
		}
		//当前资源释放有效
		inline bool IsValid()
		{
			return (m_CurrentResource != m_ResourceMap.end()) ? true : false;
		}

		// -----------------------------------------------------------------------
		// 一般资源访问

		// 预分配内存,以便插入资源后直接可用
		bool ReserveMemory(size_t nMem);
		//插入資源
		bool InsertResource(const std::string& name, BaseResource* pResource);
		bool InsertResource(const char* name, BaseResource* pResource);

		bool RemoveResource(BaseResource* pResource);
		BaseResource* RemoveResource(const std::string& name); //从Cache中移除资源，并返回资源指针
		BaseResource* RemoveResource(const char* name);

		// 销毁资源并释放内存
		bool DestroyResource(BaseResource* pResource);
		bool DestroyResource(const char* name);
		bool DestroyResource(const std::string& name);

		// 获取指定的资源， 如果资源已经被销毁，那么会重建一个
		BaseResource* GetResource(const std::string& name);
		BaseResource* GetResource(const char* name);

		void StoreResourceDependency(BaseResource* resource, const std::string& dependency);  //保存资源的依赖

		void ResetDependencies(BaseResource* resource);  //重置资源依赖

		// 锁定资源，保证不会被自动清理
		BaseResource* Lock(const char* name);

		//解锁资源
		int Unlock(const char* name);


	protected:
		
		volatile size_t			m_nCurrentUsedMemory;	//当前内存占用
		volatile size_t			m_nMaximumMemory;		//最大内存上限
		bool			m_bResourceReserved;   //是否预分配内存
		ResCMapItor		m_CurrentResource;     //当前资源迭代器
		ResCMap        m_ResourceMap;

		MutexEx        m_resMutex;
		MutexEx        m_depMutex;
		
		std::unordered_map<std::string, std::unordered_set<std::string>>   m_ResourceDependences;


	private:

		// 内部函数
		inline void AddMemory(size_t nMem)		{ m_nCurrentUsedMemory += nMem; }
		inline void RemoveMemory(size_t nMem)	{ m_nCurrentUsedMemory -= nMem; }
		//检查已释放资源
		bool CheckForOverallocation();

	};



}