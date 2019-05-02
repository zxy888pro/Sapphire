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
		//销毁未锁定所有资源, 锁定资源由自己释放了
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
				AddMemory(pResource->GetSize()); //更新内存占用
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
		//查找资源
		ResCMapItor itor;
		for (itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
		{
			if (itor->second == pResource)
				break;
		}
		if (itor == m_ResourceMap.end())
			return false;
		if (itor->second->IsLocked())
			//不能移除锁定资源
			return false;
		//从总内存减掉这部分
		RemoveMemory(pResource->GetSize());
		m_ResourceMap.erase(itor);
	}

	BaseResource* ResourceCache::RemoveResource(const std::string& name)
	{
		return RemoveResource(name.c_str());
	}

	BaseResource* ResourceCache::RemoveResource(const char* name)
	{
		//查找资源
		ResCMapItor itor = m_ResourceMap.find(name);
		if (itor == m_ResourceMap.end())
			return NULL;
		if (itor->second->IsLocked())
			//不能移除锁定资源
			return NULL;
		//从总内存减掉这部分
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

		//设置当前访问时间
		itor->second->SetLastAccess(time(0));

		// 如果丢弃掉重新创建
		if (itor->second->IsDisposed())
		{
			itor->second->Recreate();
			AddMemory(itor->second->GetSize());

			//确保检查时不会被移除内存
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

		//增加对象引用数
		itor->second->SetReferenceCount(itor->second->GetReferenceCount() + 1);

		//如果丢弃重新分配
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

		// 减少引用计数
		if (itor->second->GetReferenceCount() > 0)
			itor->second->SetReferenceCount(itor->second->GetReferenceCount() - 1);

		return itor->second->GetReferenceCount();
	}

	 

	bool ResourceCache::CheckForOverallocation()
	{
		//当前使用内存大于最大内存上限
		if (m_nCurrentUsedMemory > m_nMaximumMemory)
		{
			//要清理内存了
			int iMemToPurge = m_nCurrentUsedMemory - m_nMaximumMemory;

			// 创建个有限队列来保存托管的对象
			// 尝试移除一部分优先级低的资源腾出空间
			std::priority_queue<BaseResource*, std::vector<BaseResource*>, ptr_greater<BaseResource*> > PriQueue;

			//将map里的没有锁定的对象复制到优先队列中
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

			//如果资源队列未空而我们仍然有太多内存已分配，则返回失败。
			//可能有太多资源被锁定或者插入的资源太大到大于请求的最大内存
			if (PriQueue.empty() && (m_nCurrentUsedMemory > m_nMaximumMemory))
				return false;
		}
		return true;
	}

}

