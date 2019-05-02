#include "Sapphire.h"
#include "ResourceCache.h"
#include <queue>


namespace Sapphire
{

	 

	ResourceCache::ResourceCache(Core* pCore)
	{
		Clear();
		m_assert(pCore);
		m_pCore = pCore;
	}

	ResourceCache::~ResourceCache()
	{
		Destroy();
	}

	void ResourceCache::Clear()
	{
		m_ResourceMap.clear();
		m_nCurrentUsedMemory = 0;
		m_nMaximumMemory = 0;
		m_bResourceReserved = false;
		m_CurrentResource = m_ResourceMap.end();
	}

	bool ResourceCache::Create(uint nMaxSize)
	{
		Clear();
		SetMaximumMemory(nMaxSize);
		return true;
	}

	void ResourceCache::Destroy()
	{
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
		Clear();
	}

	bool ResourceCache::SetMaximumMemory(size_t nMem)
	{
		m_nMaximumMemory = nMem;
		return CheckForOverallocation();
	}

	bool ResourceCache::ReserveMemory(size_t nMem)
	{
		AddMemory(nMem);
		if (!CheckForOverallocation())
			return false;
		m_bResourceReserved = true;
		return true;
	}

	bool ResourceCache::InsertResource(const std::string& name, BaseResource* pResource)
	{
		return InsertResource(name.c_str(), pResource);
	}

	bool ResourceCache::InsertResource(const char* name, BaseResource* pResource)
	{
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
		
	}

	bool ResourceCache::RemoveResource(BaseResource* pResource)
	{
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
	}

	BaseResource* ResourceCache::RemoveResource(const std::string& name)
	{
		return RemoveResource(name.c_str());
	}

	BaseResource* ResourceCache::RemoveResource(const char* name)
	{
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
		return itor->second;
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

