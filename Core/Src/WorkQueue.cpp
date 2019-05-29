#include <WorkQueue.h>
#include <logUtil.h>
#include <Thread.h>
#include <Variant.h>
#include <utility>

namespace Sapphire
{
	class Worker : public Thread, public RefCounter
	{
	public:

		Worker(WorkQueue* owner,ulong id)
		{
			m_owner = owner;
			m_id = id;
		}

		//循环从任务队列中提取任务执行
		virtual void ThreadFunc() override
		{
			while (true)
			{
				SharedPtr<WorkItem> pItem = m_owner->PickWorkItem();
				if (pItem.NotNull())
				{
					pItem->Execute();
					pItem->bCompleted = true;
					
				}
				Sleep(100);
			}
		}

		ulong GetID(){ return m_id; }
	private:
		WorkQueue* m_owner;
		uint m_id;
	};

	WorkQueue::WorkQueue(Core* pCore):
		SubSystem(pCore)
	{

	}

	WorkQueue::~WorkQueue()
	{

	}

	void WorkQueue::Initialize()
	{
		Clear();
	}

	void WorkQueue::AddWorkerItem(SharedPtr<WorkItem> pitem)
	{
		if (pitem == NULL)
		{
			SAPPHIRE_LOGERROR("WorkerItem is Null");
			return;
		}
		
		pitem->bCompleted = false;
		
		//在有工作线程竞争的时候，需加锁
		if (m_workers.size() > 0 && !m_bPaused)
			m_mutexA.Acquire();

		m_assert(std::find(m_workQueue.begin(), m_workQueue.end(), pitem) != m_workQueue.end());
		m_workQueue.push_back(pitem);
		std::sort(m_workQueue.begin(), m_workQueue.end(), WorkItem::ComparePtr);

		if (m_workers.size() > 0 && !m_bPaused)
		{
			m_mutexA.Release();
		}
		

	}

	bool WorkQueue::RemoveWorkItem(SharedPtr<WorkItem> pitem)
	{
		if (pitem == NULL)
			return true;
		m_mutexA.Acquire();
		WORKITEM_QUEUE::iterator it = std::find(m_workQueue.begin(), m_workQueue.end(), pitem);
		if (it != m_workQueue.end())
		{
			m_workQueue.erase(it);
			std::sort(m_workQueue.begin(), m_workQueue.end(), WorkItem::ComparePtr);
		}
		m_mutexA.Release();
		return false;
	}

	void WorkQueue::Pause()
	{
		if (!m_bPaused)
		{
			m_mutexA.Acquire();
			m_bPaused = true;
		}
	}

	void WorkQueue::Resume()
	{
		if (m_bPaused)
		{
			m_mutexA.Release();
			m_bPaused = false;
		}
	}

	void WorkQueue::AddWorker(uint num)
	{
		Pause();

		for (int i = 0; i < num; ++i)
		{
			SharedPtr<Worker> worker = SharedPtr<Worker>(new Worker(this, CreateID()));
			worker->Run();
			m_workers.insert(std::make_pair(worker->GetID(), worker));
			
		}

	}

	uint WorkQueue::GetWorkerNum()
	{
		return m_workers.size();
	}

	bool WorkQueue::IsCompleted()
	{
		return false;
	}

	void WorkQueue::Clear()
	{
		m_ulIdOrder = 0;
	}

	void WorkQueue::Update()
	{
		//清理完成的任务
		PurgeCompletedQueue();
	}

	ulong WorkQueue::CreateID()
	{
		++m_ulIdOrder;
		return m_ulIdOrder;
	}

	void WorkQueue::PurgeCompletedQueue()
	{
		COMPLETEDITEM_QUEUE tmpQueue;
		m_mutexB.Acquire();
		while (m_completedItems.size() > 0)
		{
			tmpQueue.push_back(m_completedItems.back());
			m_completedItems.pop_back();
		}
		m_mutexB.Release();
		for (COMPLETEDITEM_QUEUE::iterator it = tmpQueue.begin(); it != tmpQueue.end(); ++it)
		{
			if ((*it)->bSendEvent == true)
			{
				FireEvent(ET_SUBSYTEM_EVENT, EVENT_SUBSYS_WORKCOMLPETED, (*it)->GetData());
			}
		}

	}

	Sapphire::SharedPtr<Sapphire::WorkItem> WorkQueue::PickWorkItem()
	{
		SharedPtr<Sapphire::WorkItem> workItem;
		if (m_workQueue.size() > 0 && !m_bPaused)
		{
			m_mutexA.Acquire();
			workItem = m_workQueue.back();
			m_workQueue.pop_back();
			std::sort(m_workQueue.begin(), m_workQueue.end(), WorkItem::ComparePtr);
			m_mutexA.Release();
		}
		return workItem;
	}

	void WorkQueue::Compeleted(SharedPtr<WorkItem> pitem)
	{
		if (pitem.NotNull())
		{
			m_mutexB.Acquire();
			m_completedItems.push_back(pitem);
			m_mutexB.Release();
		}
	}

	 

}