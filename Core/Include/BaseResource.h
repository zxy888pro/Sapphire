#ifndef _BASE_RESOURCE_H_
#define _BASE_RESOURCE_H_

#include "Sapphire.h"
#include "handle.h"


namespace Sapphire
{
#define RHANDLE uint
#define INVALID_RHANDLE 0xFFFFFFFF
#define IS_INVALID_RHANDLE(_rh)	((_rh == INVALID_RHANDLE) ? true : false)
#define IS_VALID_RHANDLE(_rh)	((_rh == INVALID_RHANDLE) ? false : true)

	enum ResoureType
	{
		ResoureType_Texture,
		ResoureType_Model,
		ResoureType_MaxCount
	};

	//资源基类
	class  BaseResource
	{
	public:

		enum PriorityType
		{
			RES_LOW_PRIORITY = 0,
			RES_MED_PRIORITY,
			RES_HIGH_PRIORITY
		};

		BaseResource()			{ Clear(); }
		virtual ~BaseResource()	{ Destroy(); }

		virtual void Clear();

		virtual bool Create()	{ return false; }
		virtual void Destroy()	{}

		virtual bool Recreate() = 0;
		virtual void Dispose() = 0;

		virtual size_t GetSize() = 0;
		virtual bool IsDisposed() = 0;

        //资源优先级
		inline void SetPriority(PriorityType priority)		{ m_Priority = priority; }
		inline PriorityType GetPriority()					{ return m_Priority; }

		//引用数
		inline void SetReferenceCount(uint nCount)			{ m_nRefCount = nCount; }
		inline uint GetReferenceCount()						{ return m_nRefCount; }
		
		inline bool IsLocked()								{ return (m_nRefCount > 0) ? true : false; }

		inline void SetLastAccess(time_t LastAccess)		{ m_LastAccess = LastAccess; }
		inline time_t GetLastAccess()						{ return m_LastAccess; }

		//决定丢弃资源顺序
		virtual bool operator < (BaseResource& container);

	protected:

		PriorityType	m_Priority;
		uint			m_nRefCount;
		time_t			m_LastAccess;

	};

	template <class T>
	class ptr_less
	{
	public:
		inline bool operator ()(T left, T right)
		{
			return ((*left) < (*right));
		}
	};


	template <class T>
	class ptr_greater
	{
	public:
		inline bool operator ()(T left, T right)
		{
			return !((*left) < (*right));
		}
	};

	typedef std::map<RHANDLE, BaseResource*> ResMap;
	typedef ResMap::iterator ResMapItor;
	typedef ResMap::value_type ResMapPair;


	class ResourceMgr
	{

	public:

		ResourceMgr()			{ Clear(); }
		virtual ~ResourceMgr()	{ Destroy(); }

		void Clear();

		bool Create(uint nMaxSize);
		void Destroy();

	   
		bool SetMaximumMemory(size_t nMem);  //设置资源管理器的最大占用内存上限
		size_t GetMaximumMemory()		{ return m_nMaximumMemory; }  


		// --------------------------------------------------------------------------
		// 资源字典迭代

		//直接跨dll访问ResMap会导致栈指针错误，必须通过包装间接访问才能保证安全
		inline void GotoBegin()
		{
			m_CurrentResource = m_ResourceMap.begin();
		}
		//取得当前资源
		inline BaseResource* GetCurrentResource()
		{
			return (*m_CurrentResource).second;
		}
		//下一个
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
		bool InsertResource(RHANDLE* rhUniqueID, BaseResource* pResource);
		bool InsertResource(RHANDLE rhUniqueID, BaseResource* pResource);

		bool RemoveResource(BaseResource* pResource);
		bool RemoveResource(RHANDLE rhUniqueID);

		// 销毁资源并释放内存
		bool DestroyResource(BaseResource* pResource);
		bool DestroyResource(RHANDLE rhUniqueID);

		// 获取指定的资源， 如果资源已经被销毁，那么会重建一个
		BaseResource* GetResource(RHANDLE rhUniqueID);

		// 锁定资源，保证不会被自动清理
		BaseResource* Lock(RHANDLE rhUniqueID);

		//解锁资源
		int Unlock(RHANDLE rhUniqueID);
		int Unlock(BaseResource* pResource);

		// 通过资源指针获取存储句柄
		RHANDLE FindResourceHandle(BaseResource* pResource);

	protected:

		// 内部函数
		inline void AddMemory(size_t nMem)		{ m_nCurrentUsedMemory += nMem; }
		inline void RemoveMemory(size_t nMem)	{ m_nCurrentUsedMemory -= nMem; }
		//獲取下一個可用的資源句柄,句柄暫時由容器統一管理和递增生產
		uint GetNextResHandle()					{ return --m_rhNextResHandle; }

		//检查已释放资源
		bool CheckForOverallocation();

	protected:
		RHANDLE			m_rhNextResHandle;
		size_t			m_nCurrentUsedMemory;
		size_t			m_nMaximumMemory;
		bool			m_bResourceReserved;
		ResMapItor		m_CurrentResource;
		ResMap			m_ResourceMap;
	};

}


#endif