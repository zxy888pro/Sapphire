#pragma once


#include "SapphireDef.h"
#include "Thread.h"
#include <SubSystem.h>
#include "BaseResource.h"



namespace Sapphire
{

	//��Դ�Ƚ�
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
	
	//��Դ�������
	class SAPPHIRE_CLASS ResourceCache : public SubSystem
	{
		SAPPHIRE_OBJECT(ResourceCache, SubSystem)

	public:

		 
		ResourceCache(Core* pCore);
		virtual ~ResourceCache();

		void Clear();

		bool Create(uint nMaxSize);
		void Destroy();

		bool SetMaximumMemory(size_t nMem);  //������Դ�����������ռ���ڴ�����
		size_t GetMaximumMemory()		{ return m_nMaximumMemory; }

		// ��Դ�ֵ����

		//ֱ�ӿ�dll����ResCMap�ᵼ��ջָ����󣬱���ͨ����װ��ӷ��ʲ��ܱ�֤��ȫ
		//��λ����һ����Դ
		inline void GotoBegin()
		{
			m_CurrentResource = m_ResourceMap.begin();
		}
		//ȡ�õ�ǰ��Դ
		inline BaseResource* GetCurrentResource()
		{
			return (*m_CurrentResource).second;
		}
		//��һ����Դ
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
		//�����YԴ
		bool InsertResource(const std::string& name, BaseResource* pResource);
		bool InsertResource(const char* name, BaseResource* pResource);

		bool RemoveResource(BaseResource* pResource);
		BaseResource* RemoveResource(const std::string& name); //��Cache���Ƴ���Դ����������Դָ��
		BaseResource* RemoveResource(const char* name);

		// ������Դ���ͷ��ڴ�
		bool DestroyResource(BaseResource* pResource);
		bool DestroyResource(const char* name);
		bool DestroyResource(const std::string& name);

		// ��ȡָ������Դ�� �����Դ�Ѿ������٣���ô���ؽ�һ��
		BaseResource* GetResource(const std::string& name);
		BaseResource* GetResource(const char* name);

		void StoreResourceDependency(BaseResource* resource, const std::string& dependency);  //������Դ������

		void ResetDependencies(BaseResource* resource);  //������Դ����

		// ������Դ����֤���ᱻ�Զ�����
		BaseResource* Lock(const char* name);

		//������Դ
		int Unlock(const char* name);


	protected:
		
		volatile size_t			m_nCurrentUsedMemory;	//��ǰ�ڴ�ռ��
		volatile size_t			m_nMaximumMemory;		//����ڴ�����
		bool			m_bResourceReserved;   //�Ƿ�Ԥ�����ڴ�
		ResCMapItor		m_CurrentResource;     //��ǰ��Դ������
		ResCMap        m_ResourceMap;

		MutexEx        m_resMutex;
		MutexEx        m_depMutex;
		
		std::unordered_map<std::string, std::unordered_set<std::string>>   m_ResourceDependences;


	private:

		// �ڲ�����
		inline void AddMemory(size_t nMem)		{ m_nCurrentUsedMemory += nMem; }
		inline void RemoveMemory(size_t nMem)	{ m_nCurrentUsedMemory -= nMem; }
		//������ͷ���Դ
		bool CheckForOverallocation();

	};



}