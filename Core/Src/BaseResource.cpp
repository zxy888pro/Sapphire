#include "BaseResource.h"
#include <queue>
#include "ResourceCache.h"

void Sapphire::BaseResource::Clear()
{
	m_Priority = RES_MED_PRIORITY;
	m_nRefCount = 0;
	m_LastAccess = 0;
}

void Sapphire::BaseResource::OnLoadStart()
{
	m_eState = ResourceState_Loading;
	ResourceCache* cache = m_pCore->GetSubSystem<ResourceCache>();
	if (cache)
	{
		cache->InsertResource(m_resName.c_str(), this);
	}
}

void Sapphire::BaseResource::OnLoadEnd()
{
	m_eState = ResourceState_Loaded;
}

void Sapphire::BaseResource::OnLoadError()
{
	m_eState = ResourceState_Unload;
}

bool Sapphire::BaseResource::operator < (BaseResource& container)
{
	//比较资源优先级
	if (GetPriority() < container.GetPriority())
		return true;
	else if (GetPriority() > container.GetPriority())
		return false;
	else
	{
		//再比较资源访问数
		if (m_LastAccess < container.GetLastAccess())
			return true;
		else if (m_LastAccess > container.GetLastAccess())
			return false;
		else
		{
			//最后比较大小
			if (GetSize() < container.GetSize())
				return true;
			else
				return false;
		}
	}
	return false;
}

Sapphire::ResourceContainer::ResourceContainer(Core* pCore):
BaseObject(pCore)
{
	 
}

void Sapphire::ResourceContainer::Clear()
{
	m_ResourceMap.clear();
	m_rhNextResHandle = INVALID_RHANDLE;
	m_nCurrentUsedMemory = 0;
	m_nMaximumMemory = 0;
	m_bResourceReserved = false;
	m_CurrentResource = m_ResourceMap.end();
}

bool Sapphire::ResourceContainer::Create(uint nMaxSize)
{
	Clear();
	SetMaximumMemory(nMaxSize);
	return true;
}

void Sapphire::ResourceContainer::Destroy()
{
	//销毁未锁定所有资源, 锁定资源由自己释放了
	for (ResMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
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

bool Sapphire::ResourceContainer::SetMaximumMemory(size_t nMem)
{
	m_nMaximumMemory = nMem;
	return CheckForOverallocation();
}

bool Sapphire::ResourceContainer::ReserveMemory(size_t nMem)
{
	AddMemory(nMem);
	if (!CheckForOverallocation())
		return false;
	m_bResourceReserved = true;
	return true;
}

bool Sapphire::ResourceContainer::InsertResource(RHANDLE* rhUniqueID, BaseResource* pResource)
{
	// 取得這個目錄下的下一個唯一的ID
	*rhUniqueID = GetNextResHandle();
	m_ResourceMap.insert(ResMapPair(*rhUniqueID, pResource));
     
	if (!m_bResourceReserved)
	{
		AddMemory(pResource->GetSize());
		if (!CheckForOverallocation())
			return false;
	}
	else
		m_bResourceReserved = false;

	return true;
}

bool Sapphire::ResourceContainer::RemoveResource(BaseResource* pResource)
{
    //查找资源
	ResMapItor itor;
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

	return true;
}

bool Sapphire::ResourceContainer::DestroyResource(BaseResource* pResource)
{
	if (!RemoveResource(pResource))
		return false;
	//delete pResource;
	safeDelete(pResource);
	return true;
}

Sapphire::BaseResource* Sapphire::ResourceContainer::GetResource(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);

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
		Lock(rhUniqueID);
		CheckForOverallocation();
		Unlock(rhUniqueID);
	}


	return itor->second;
}

Sapphire::BaseResource* Sapphire::ResourceContainer::Lock(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
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

int Sapphire::ResourceContainer::Unlock(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
	if (itor == m_ResourceMap.end())
		return -1;

	// 减少引用计数
	if (itor->second->GetReferenceCount() > 0)
		itor->second->SetReferenceCount(itor->second->GetReferenceCount() - 1);

	return itor->second->GetReferenceCount();
}

RHANDLE Sapphire::ResourceContainer::FindResourceHandle(BaseResource* pResource)
{
	//查找资源句柄
	ResMapItor itor;
	for (itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
	{
		if (itor->second == pResource)
			break;
	}
	if (itor == m_ResourceMap.end())
		return INVALID_RHANDLE;
	return itor->first;
}

bool Sapphire::ResourceContainer::CheckForOverallocation()
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
		for (ResMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
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

int Sapphire::ResourceContainer::Unlock(BaseResource* pResource)
{
	RHANDLE rhResource = FindResourceHandle(pResource);
	if IS_INVALID_RHANDLE(rhResource)
		return -1;
	return Unlock(rhResource);
}

bool Sapphire::ResourceContainer::DestroyResource(RHANDLE rhUniqueID)
{
	BaseResource* pResource = GetResource(rhUniqueID);
	if (!RemoveResource(rhUniqueID))
		return false;
	delete pResource;
	return true;
}

bool Sapphire::ResourceContainer::RemoveResource(RHANDLE rhUniqueID)
{
	
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
	if (itor == m_ResourceMap.end())
	
		return false;
	if (((*itor).second)->IsLocked())
		return false;

	RemoveMemory(((*itor).second)->GetSize());
	m_ResourceMap.erase(itor);

	return true;
}

bool Sapphire::ResourceContainer::InsertResource(RHANDLE rhUniqueID, BaseResource* pResource)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
	if (itor != m_ResourceMap.end())
		//如果此句柄已经存在
		return false;
	m_ResourceMap.insert(ResMapPair(rhUniqueID, pResource));
	
	if (!m_bResourceReserved)
	{
		AddMemory(pResource->GetSize());

		if (!CheckForOverallocation())
			return false;
	}
	else
		m_bResourceReserved = false;

	return true;
}
