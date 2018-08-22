#include "BaseResource.h"
#include <queue>
void Sapphire::BaseResource::Clear()
{
	m_Priority = RES_MED_PRIORITY;
	m_nRefCount = 0;
	m_LastAccess = 0;
}

bool Sapphire::BaseResource::operator < (BaseResource& container)
{
	//�Ƚ���Դ���ȼ�
	if (GetPriority() < container.GetPriority())
		return true;
	else if (GetPriority() > container.GetPriority())
		return false;
	else
	{
		//�ٱȽ���Դ������
		if (m_LastAccess < container.GetLastAccess())
			return true;
		else if (m_LastAccess > container.GetLastAccess())
			return false;
		else
		{
			//���Ƚϴ�С
			if (GetSize() < container.GetSize())
				return true;
			else
				return false;
		}
	}
	return false;
}

void Sapphire::ResourceMgr::Clear()
{
	m_ResourceMap.clear();
	m_rhNextResHandle = INVALID_RHANDLE;
	m_nCurrentUsedMemory = 0;
	m_nMaximumMemory = 0;
	m_bResourceReserved = false;
	m_CurrentResource = m_ResourceMap.end();
}

bool Sapphire::ResourceMgr::Create(uint nMaxSize)
{
	Clear();
	SetMaximumMemory(nMaxSize);
	return true;
}

void Sapphire::ResourceMgr::Destroy()
{
	//����δ����������Դ, ������Դ���Լ��ͷ���
	for (ResMapItor itor = m_ResourceMap.begin(); itor != m_ResourceMap.end(); ++itor)
	{
		if (!((*itor).second)->IsLocked())
		{
			delete ((*itor).second);
		}
	}
	m_ResourceMap.clear();
	Clear();
}

bool Sapphire::ResourceMgr::SetMaximumMemory(size_t nMem)
{
	m_nMaximumMemory = nMem;
	return CheckForOverallocation();
}

bool Sapphire::ResourceMgr::ReserveMemory(size_t nMem)
{
	AddMemory(nMem);
	if (!CheckForOverallocation())
		return false;
	m_bResourceReserved = true;
	return true;
}

bool Sapphire::ResourceMgr::InsertResource(RHANDLE* rhUniqueID, BaseResource* pResource)
{
	// ȡ���@��Ŀ��µ���һ��Ψһ��ID
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

bool Sapphire::ResourceMgr::RemoveResource(BaseResource* pResource)
{
    //������Դ
	ResMapItor itor;
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

	return true;
}

bool Sapphire::ResourceMgr::DestroyResource(BaseResource* pResource)
{
	if (!RemoveResource(pResource))
		return false;
	delete pResource;
	return true;
}

Sapphire::BaseResource* Sapphire::ResourceMgr::GetResource(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);

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
		Lock(rhUniqueID);
		CheckForOverallocation();
		Unlock(rhUniqueID);
	}


	return itor->second;
}

Sapphire::BaseResource* Sapphire::ResourceMgr::Lock(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
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

int Sapphire::ResourceMgr::Unlock(RHANDLE rhUniqueID)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
	if (itor == m_ResourceMap.end())
		return -1;

	// �������ü���
	if (itor->second->GetReferenceCount() > 0)
		itor->second->SetReferenceCount(itor->second->GetReferenceCount() - 1);

	return itor->second->GetReferenceCount();
}

RHANDLE Sapphire::ResourceMgr::FindResourceHandle(BaseResource* pResource)
{
	//������Դ���
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

bool Sapphire::ResourceMgr::CheckForOverallocation()
{
	//��ǰʹ���ڴ��������ڴ�����
	if (m_nCurrentUsedMemory > m_nMaximumMemory)
	{

		int iMemToPurge = m_nCurrentUsedMemory - m_nMaximumMemory;

		// ���������޶����������йܵĶ���
		// �����Ƴ�һ�������ȼ��͵���Դ�ڳ��ռ�
		std::priority_queue<BaseResource*, std::vector<BaseResource*>, ptr_greater<BaseResource*> > PriQueue;

		//��map���û�������Ķ����Ƶ����ȶ�����
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

		//�����Դ����δ�ն�������Ȼ��̫���ڴ��ѷ��䣬�򷵻�ʧ�ܡ�
		//������̫����Դ���������߲������Դ̫�󵽴������������ڴ�
		if (PriQueue.empty() && (m_nCurrentUsedMemory > m_nMaximumMemory))
			return false;
	}
	return true;
}

int Sapphire::ResourceMgr::Unlock(BaseResource* pResource)
{
	RHANDLE rhResource = FindResourceHandle(pResource);
	if IS_INVALID_RHANDLE(rhResource)
		return -1;
	return Unlock(rhResource);
}

bool Sapphire::ResourceMgr::DestroyResource(RHANDLE rhUniqueID)
{
	BaseResource* pResource = GetResource(rhUniqueID);
	if (!RemoveResource(rhUniqueID))
		return false;
	delete pResource;
	return true;
}

bool Sapphire::ResourceMgr::RemoveResource(RHANDLE rhUniqueID)
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

bool Sapphire::ResourceMgr::InsertResource(RHANDLE rhUniqueID, BaseResource* pResource)
{
	ResMapItor itor = m_ResourceMap.find(rhUniqueID);
	if (itor != m_ResourceMap.end())
		//����˾���Ѿ�����
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
