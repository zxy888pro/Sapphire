#ifndef _BASE_RESOURCE_H_
#define _BASE_RESOURCE_H_

#include "Sapphire.h"
#include "handle.h"


namespace Sapphire
{
	typedef Handle <BaseResource> HRESOURCE;

	//��Դ����
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

        //��Դ���ȼ�
		inline void SetPriority(PriorityType priority)		{ m_Priority = priority; }
		inline PriorityType GetPriority()					{ return m_Priority; }

		//������
		inline void SetReferenceCount(uint nCount)			{ m_nRefCount = nCount; }
		inline uint GetReferenceCount()						{ return m_nRefCount; }
		
		inline bool IsLocked()								{ return (m_nRefCount > 0) ? true : false; }

		inline void SetLastAccess(time_t LastAccess)		{ m_LastAccess = LastAccess; }
		inline time_t GetLastAccess()						{ return m_LastAccess; }

		//����������Դ˳��
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

	typedef std::map<HRESOURCE, BaseResource*> ResMap;
	typedef ResMap::iterator ResMapItor;
	typedef ResMap::value_type ResMapPair;


	class ResManager
	{

	public:

		ResManager()			{ Clear(); }
		virtual ~ResManager()	{ Destroy(); }

		void Clear();

		bool Create(uint nMaxSize);
		void Destroy();

	   
		bool SetMaximumMemory(size_t nMem);  //������Դ�����������ռ���ڴ�����
		size_t GetMaximumMemory()		{ return m_nMaximumMemory; }  


		// --------------------------------------------------------------------------
		// ��Դ�ֵ����

		//ֱ�ӿ�dll����ResMap�ᵼ��ջָ����󣬱���ͨ����װ��ӷ��ʲ��ܱ�֤��ȫ
		inline void GotoBegin()
		{
			m_CurrentResource = m_ResourceMap.begin();
		}
		//ȡ�õ�ǰ��Դ
		inline BaseResource* GetCurrentResource()
		{
			return (*m_CurrentResource).second;
		}
		//��һ��
		inline bool GotoNext()
		{
			m_CurrentResource++;  return IsValid();
		}
		//��ǰ��Դ�ͷ���Ч
		inline bool IsValid()
		{
			return (m_CurrentResource != m_ResourceMap.end()) ? true : false;
		}


		// -----------------------------------------------------------------------
		// һ����Դ����

		// Ԥ�����ڴ�,�Ա������Դ��ֱ�ӿ���
		bool ReserveMemory(size_t nMem);

		bool InsertResource(HRESOURCE* rhUniqueID, BaseResource* pResource);
		bool InsertResource(HRESOURCE rhUniqueID, BaseResource* pResource);

		bool RemoveResource(BaseResource* pResource);
		bool RemoveResource(HRESOURCE rhUniqueID);

		// ������Դ���ͷ��ڴ�
		bool DestroyResource(BaseResource* pResource);
		bool DestroyResource(HRESOURCE rhUniqueID);

		// ��ȡָ������Դ�� �����Դ�Ѿ������٣���ô���ؽ�һ��
		BaseResource* GetResource(HRESOURCE rhUniqueID);

		// ������Դ����֤���ᱻ�Զ�����
		BaseResource* Lock(HRESOURCE rhUniqueID);

		//������Դ
		int Unlock(HRESOURCE rhUniqueID);
		int Unlock(BaseResource* pResource);

		// ͨ����Դָ���ȡ�洢���
		HRESOURCE FindResourceHandle(BaseResource* pResource);

	protected:

		// �ڲ�����
		inline void AddMemory(size_t nMem)		{ m_nCurrentUsedMemory += nMem; }
		inline void RemoveMemory(size_t nMem)	{ m_nCurrentUsedMemory -= nMem; }
		//uint GetNextResHandle()					{ return --m_rhNextResHandle; }

		//������ͷ���Դ
		bool CheckForOverallocation();

	protected:
		HRESOURCE			m_rhNextResHandle;
		size_t			m_nCurrentUsedMemory;
		size_t			m_nMaximumMemory;
		bool			m_bResourceReserved;
		ResMapItor		m_CurrentResource;
		ResMap			m_ResourceMap;
	};

}


#endif