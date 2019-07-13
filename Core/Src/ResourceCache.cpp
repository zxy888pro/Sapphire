#include "Sapphire.h"
#include "ResourceCache.h"
#include <queue>


namespace Sapphire
{

	 

	ResourceCache::ResourceCache(Core* pCore):
		SubSystem(pCore)
	{
		Clear();
	}

	ResourceCache::~ResourceCache()
	{
		Destroy();
	}

	void ResourceCache::Clear()
	{
		//m_resMutex.Lock();
		//����δ����������Դ, ������Դ���Լ��ͷ���
		for (ResCMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
		{
			if (!((*itor).second)->IsLocked())
			{
				//delete ((*itor).second);
				safeDelete(((*itor).second));
			}
		}
		m_ResourceMap.clear();
		m_nCurrentUsedMemory = 0;
		m_nMaximumMemory = 0;
		m_bResourceReserved = false;
		m_CurrentResource = m_ResourceMap.end();
		//m_resMutex.Unlock();
	}

	bool ResourceCache::Create(uint nMaxSize)
	{
		Clear();
		SetMaximumMemory(nMaxSize);
		return true;
	}

	void ResourceCache::Destroy()
	{
		Clear();
	}
		

	bool ResourceCache::SetMaximumMemory(size_t nMem)
	{
		m_nMaximumMemory = nMem;
		//m_resMutex.Lock();
		bool ret = CheckForOverallocation();
		//m_resMutex.Unlock();
		return ret;
	}

	bool ResourceCache::ReserveMemory(size_t nMem)
	{
		AddMemory(nMem);
		//m_resMutex.Lock();
		if (!CheckForOverallocation())
			return false;
		m_bResourceReserved = true;
		//m_resMutex.Unlock();
		return true;
	}

	bool ResourceCache::InsertResource(const std::string& name, BaseResource* pResource)
	{
		return InsertResource(name.c_str(), pResource);
	}

	bool ResourceCache::InsertResource(const char* name, BaseResource* pResource)
	{	
		//m_resMutex.Lock();
		if (m_ResourceMap.find(name) == m_ResourceMap.end())
		{
			m_ResourceMap.insert(std::make_pair(name, pResource));
			if (!m_bResourceReserved)
			{
				AddMemory(pResource->GetSize()); //�����ڴ�ռ��
				if (!CheckForOverallocation())
					return false;
			}
			return true;
		}
		else
		{
			SAPPHIRE_LOG(StringFormat("InsertResource Failed! Resource %s has existed!", name));
			return false;
		}
		//m_resMutex.Unlock();
		
	}

	bool ResourceCache::RemoveResource(BaseResource* pResource)
	{
		//m_resMutex.Lock();
		//������Դ
		ResCMapItor itor;
		for (itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
		{
			if (itor->second == pResource)
				break;
		}
		if (itor == m_ResourceMap.end())
			return false;
		if (itor->second->IsLocked())
			//�����Ƴ�������Դ
			return false;
		//�����ڴ�����ⲿ��
		RemoveMemory(pResource->GetSize());
		m_ResourceMap.erase(itor);
		//m_resMutex.Unlock();
	}

	BaseResource* ResourceCache::RemoveResource(const std::string& name)
	{
		return RemoveResource(name.c_str());
	}

	BaseResource* ResourceCache::RemoveResource(const char* name)
	{
		//m_resMutex.Lock();
		//������Դ
		ResCMapItor itor = m_ResourceMap.find(name);
		if (itor == m_ResourceMap.end())
			return NULL;
		if (itor->second->IsLocked())
			//�����Ƴ�������Դ
			return NULL;
		//�����ڴ�����ⲿ��
		BaseResource* pResource = itor->second;
		if (pResource)
		{
			RemoveMemory(itor->second->GetSize());
			m_ResourceMap.erase(itor);
		}
		//m_resMutex.Unlock();
		return pResource;

	}

	bool ResourceCache::DestroyResource(BaseResource* pResource)
	{
		if (!RemoveResource(pResource))
			return false;
		//delete pResource;
		safeDelete(pResource);
		return true;
	}

	bool ResourceCache::DestroyResource(const char* name)
	{
		BaseResource* pResource = RemoveResource(name);
		if (!pResource)
			return false;
		//delete pResource;
		safeDelete(pResource);
		return true;
	}

	bool ResourceCache::DestroyResource(const std::string& name)
	{
		return DestroyResource(name.c_str());
	}

	Sapphire::BaseResource* ResourceCache::GetResource(const std::string& name)
	{
		return GetResource(name.c_str()); 
	}

	Sapphire::BaseResource* ResourceCache::GetResource(const char* name)
	{
		//m_resMutex.Lock();
		ResCMapItor itor = m_ResourceMap.find(name);

		if (itor == m_ResourceMap.end())
			return NULL;

		//���õ�ǰ����ʱ��
		itor->second->SetLastAccess(time(0));

		// ������������´���
		if (itor->second->IsDisposed())
		{
			itor->second->Recreate();
			AddMemory(itor->second->GetSize());

			//ȷ�����ʱ���ᱻ�Ƴ��ڴ�
			Lock(name);
			CheckForOverallocation();
			Unlock(name);
		}
		//m_resMutex.Unlock();
		return itor->second;
	}

	void ResourceCache::StoreResourceDependency(BaseResource* resource, const std::string& dependency)
	{
		//m_depMutex.Lock();
		if (!resource)
			return;
	
		std::string name = resource->GetName();
		std::unordered_set<std::string>& dependents = m_ResourceDependences[dependency];
		dependents.insert(name);
		//m_depMutex.Unlock();
	}

	void ResourceCache::ResetDependencies(BaseResource* resource)
	{
		//m_depMutex.Lock();
		if (!resource)
			return;

		std::string name  = resource->GetName().c_str();

		for (std::unordered_map<std::string, std::unordered_set<std::string> >::iterator it = m_ResourceDependences.begin(); it != m_ResourceDependences.end();)
		{
			std::unordered_set<std::string>& dependents = it->second;
			dependents.erase(name);
			if (dependents.empty())
			{
				std::unordered_map<std::string, std::unordered_set<std::string> >::iterator delIt = it;
				++it;
				m_ResourceDependences.erase(delIt);
			}
			else
			{
				++it;
			}
		}
		//m_depMutex.Unlock();
	}

	Sapphire::BaseResource* ResourceCache::Lock(const char* name)
	{
		ResCMapItor itor = m_ResourceMap.find(name);
		if (itor == m_ResourceMap.end())
			return NULL;

		//���Ӷ���������
		itor->second->SetReferenceCount(itor->second->GetReferenceCount() + 1);

		//����������·���
		if (itor->second->IsDisposed())
		{
			itor->second->Recreate();
			AddMemory(itor->second->GetSize());
			CheckForOverallocation();
		}

		return itor->second;
	}

	int ResourceCache::Unlock(const char* name)
	{
		ResCMapItor itor = m_ResourceMap.find(name);
		if (itor == m_ResourceMap.end())
			return -1;

		// �������ü���
		if (itor->second->GetReferenceCount() > 0)
			itor->second->SetReferenceCount(itor->second->GetReferenceCount() - 1);

		return itor->second->GetReferenceCount();
	}

	 

	bool ResourceCache::CheckForOverallocation()
	{
		//��ǰʹ���ڴ��������ڴ�����
		if (m_nCurrentUsedMemory > m_nMaximumMemory)
		{
			//Ҫ�����ڴ���
			int iMemToPurge = m_nCurrentUsedMemory - m_nMaximumMemory;

			// ���������޶����������йܵĶ���
			// �����Ƴ�һ�������ȼ��͵���Դ�ڳ��ռ�
			std::priority_queue<BaseResource*, std::vector<BaseResource*>, ptr_greater<BaseResource*> > PriQueue;

			//��map���û�������Ķ����Ƶ����ȶ�����
			for (ResCMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
			{
				if (!itor->second->IsDisposed() && !itor->second->IsLocked())
					PriQueue.push(itor->second);
			}

			while ((!PriQueue.empty()) && (m_nCurrentUsedMemory > m_nMaximumMemory))
			{
				uint nDisposalSize = PriQueue.top()->GetSize();
				PriQueue.top()->Dispose();
				if (PriQueue.top()->IsDisposed())
					RemoveMemory(nDisposalSize);
				PriQueue.pop();
			}

			//�����Դ����δ�ն�������Ȼ��̫���ڴ��ѷ��䣬�򷵻�ʧ�ܡ�
			//������̫����Դ���������߲������Դ̫�󵽴������������ڴ�
			if (PriQueue.empty() && (m_nCurrentUsedMemory > m_nMaximumMemory))
				return false;
		}
		return true;
	}

}

