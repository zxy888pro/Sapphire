#include "AsynTaskPool.h"
#include <algorithm>
#include <map>

namespace Sapphire
{

	class AsynTaskThread : public  Thread
	{
	public:
		 

		AsynTaskThread(AsynTaskPool* pool, ulong id, bool autoExit = true)
		{
			m_pTaskPool = pool;
			m_id = id;
			m_bExitFlag = false;
			workState = 0;
			m_bAutoExit = autoExit;
			m_curTask = 0;
		}
		~AsynTaskThread()
		{
			
		}

		virtual void ThreadFunc() {

			while (!m_bExitFlag)
			{
				LockState();
				workState = 0;
				UnlockState();
				AsynTask* pTask = m_pTaskPool->Pop();
				if (pTask != 0)
				{
					m_curTask = pTask;
					pTask->run();
					pTask->bCompleted = true; 
					if (pTask->isAutoDelete()) //是否是需要自动删除的任务
					{
						safeDelete(pTask);
					}
					if (m_bAutoExit)
					{
						m_bExitFlag = true;
						continue;
					}
					m_sleepTime = m_pTaskPool->getSleepTime();
				}
				else
				{
					m_sleepTime = 200;
				}
				LockState();
				workState = 1;
				UnlockState();
#ifdef SAPPHIRE_WIN
				Sleep(m_sleepTime);
#else
				usleep(m_sleepTime);
#endif

			}
			LockState();
			workState = 2;
			UnlockState();
		}

	public:
		ulong getId() const {
			return m_id;
		}

		void setId(ulong m_id) {
			AsynTaskThread::m_id = m_id;
		}

		void setExitFlag(bool flag)
		{
			m_bExitFlag = flag;
		}
		bool getExitFlag(){ return m_bExitFlag; }
		void LockState()
		{
			m_mutex.Lock();
		}

		void UnlockState()
		{
			m_mutex.Unlock();
		}

		volatile int workState;

	private:
		AsynTaskPool* m_pTaskPool;
		ulong m_id;
		volatile  bool m_bExitFlag;
		bool m_bAutoExit;
		ulong m_sleepTime;
		AsynTask*  m_curTask;
		MutexEx m_mutex;

	};

	
	AsynTaskPool::AsynTaskPool(Core* pCore) :
		SubSystem(pCore),
		m_maxWorkerCount(8),
		m_minWorkerCount(0),
		m_ulIdOrder(0),
		m_sleepTime(50),
		m_curTaskCount(0),
		m_bIsRunning(true)
	{
		m_type = ESST_ASYNTASKPOOL;
	}

	AsynTaskPool::AsynTaskPool(Core* pCore,uint maxWorkerCount, uint minWorkerCount, ulong sleepTime):
		SubSystem(pCore)
	{
		m_maxWorkerCount = maxWorkerCount;
		m_minWorkerCount = minWorkerCount;
		m_sleepTime = sleepTime;
		m_ulIdOrder = 0;
		m_curTaskCount = 0;
		m_bIsRunning = true;
		m_type = ESST_ASYNTASKPOOL;
	}

	AsynTaskPool::~AsynTaskPool()
	{
		Close();
	}

	void AsynTaskPool::Initialize()
	{
		Clear();
		SetWorkerNum(m_minWorkerCount);
		Run();

	}

	void AsynTaskPool::AddTask(AsynTask* pTask)
	{
		if (pTask != 0)
		{
			pTask->bCompleted = false;
			m_taskMutex.Lock();
			if (m_tasks.empty())
				m_tasks.push_back(pTask);
			else
			{
				if (std::find(m_tasks.begin(), m_tasks.end(), pTask) == m_tasks.end())
					m_tasks.push_back(pTask);
				std::sort(m_tasks.begin(), m_tasks.end(), AsynTask::ComparePtr);
			}
			m_curTaskCount = m_tasks.size();
			if (m_curTaskCount > 1)
				AddWorker(true);
			m_taskMutex.Unlock();
			return;
		}
	}

	bool AsynTaskPool::RemoveTask(AsynTask* pTask)
	{
		if (pTask == NULL)
			return true;
		m_taskMutex.Lock();
		TASKITEM_QUEUE::iterator it = std::find(m_tasks.begin(), m_tasks.end(), pTask);
		if (it != m_tasks.end())
		{
			m_tasks.erase(it);
			//safeDelete(pTask);
			std::sort(m_tasks.begin(), m_tasks.end(), AsynTask::ComparePtr);
		}
		m_curTaskCount = m_tasks.size();
		m_taskMutex.Unlock();
		return false;
	}

	void AsynTaskPool::SetWorkerNum(int num)
	{
		if (num > m_maxWorkerCount)
			num = m_maxWorkerCount;
		if (num < m_minWorkerCount)
			num = m_minWorkerCount;
		m_threadMutex.Lock();
		m_curWorkerCount = num;
		if (num > m_workers.size())
		{
			num = num - m_workers.size();
			for (int i = 0; i < num; ++i)
			{
				AsynTaskThread* pWorker = new AsynTaskThread(this, CreateID(), false);
				pWorker->Run();
				m_workers.push_back(pWorker);
			}

		}
		else if (num < m_workers.size())
		{
			num = m_workers.size() - num;
			for (WORKER_QUEUE::iterator it = m_workers.begin(); it != m_workers.end(); ++it)
			{
				if (num <= 0)
					break;
				if (!(*it)->getExitFlag())
				{
					(*it)->setExitFlag(true);
				}
				--num;
			}
		}
		m_threadMutex.Unlock();
	}

	void AsynTaskPool::AddWorker(bool autoExit)
	{
		if (m_curWorkerCount < m_maxWorkerCount)
		{
			m_threadMutex.Lock();
			AsynTaskThread* pWorker = new AsynTaskThread(this, CreateID(), autoExit);
			pWorker->Run();
			m_workers.push_back(pWorker);
			++m_curWorkerCount;
			m_threadMutex.Unlock();
		}
	}

	bool AsynTaskPool::IsCompleted()
	{
		bool bRet = true;
		if (GetTaskCount() > 0)
		{
			bRet = false;
		}
		m_threadMutex.Lock();
		for (WORKER_QUEUE::iterator it = m_workers.begin(); it != m_workers.end(); ++it)
		{
			if ((*it) != 0 && (*it)->workState == 0)
			{
				bRet = false;
				break;
			}
		}
		m_threadMutex.Unlock();
		return bRet;
	}

	Sapphire::AsynTask* AsynTaskPool::Pop()
	{
		AsynTask*  pTask = 0;
		m_taskMutex.Lock();
		if (m_tasks.size() > 0)
		{
			pTask = m_tasks.back();
			m_tasks.pop_back();
			std::sort(m_tasks.begin(), m_tasks.end(), AsynTask::ComparePtr);
		}
		m_curTaskCount = m_tasks.size();
		m_taskMutex.Unlock();
		return pTask;
	}

	uint AsynTaskPool::GetThreadCount() const
	{
		return m_curWorkerCount;
	}

	uint AsynTaskPool::GetTaskCount() const
	{
		return m_curTaskCount;
	}

	void AsynTaskPool::Clear()
	{
		m_taskMutex.Lock();
		for (TASKITEM_QUEUE::iterator it = m_tasks.begin(); it != m_tasks.end();)
		{
			m_tasks.erase(it);
		}
		m_taskMutex.Unlock();
	}

	void AsynTaskPool::Update()
	{
		m_threadMutex.Lock();
		for (WORKER_QUEUE::iterator it = m_workers.begin(); it != m_workers.end();)
		{
			if ((*it) != 0 && (*it)->workState == 2)
			{
				safeDelete(*it);
				it = m_workers.erase(it);
				--m_curWorkerCount;
			}
			else
			{
				++it;
			}
		}
		m_threadMutex.Unlock();
		if (m_tasks.size() > 1)
		{
			AddWorker(true);
		}
	}

	void AsynTaskPool::Close()
	{
		m_bIsRunning = false;
#ifdef SAPPHIRE_WIN
		Stop();
		//int ret = WaitForSingleObject(GetHandle(), INFINITE);
#else
		Stop();
		//int ret = pthread_join(mThreadHandle, NULL);
#endif
		Clear();
	}

	void AsynTaskPool::ThreadFunc()
	{
		while (m_bIsRunning)
		{
			Update();
#ifdef SAPPHIRE_WIN
			Sleep(50);
#else
			usleep(50);
#endif
		}
		m_threadMutex.Lock();
		int ret = 0;
		for (WORKER_QUEUE::iterator it = m_workers.begin(); it != m_workers.end();)
		{
			AsynTaskThread* pThread = *it;
			pThread->setExitFlag(true);
#ifdef SAPPHIRE_WIN
			//ret = WaitForSingleObject(pThread->GetHandle(), INFINITE);
			pThread->Stop();
#else
			//ret = pthread_join(pThread->mThreadHandle, NULL);
			pThread->Stop();
#endif
			
			safeDelete(*it);
			it = m_workers.erase(it);
		}
		m_threadMutex.Unlock();
	}

	ulong AsynTaskPool::CreateID()
	{
		++m_ulIdOrder;
		return m_ulIdOrder;
	}

	uint AsynTaskPool::getMaxWorkerCount() const
	{
		return m_maxWorkerCount;
	}

	void AsynTaskPool::setMaxWorkerCount(uint maxWorkerCount)
	{
		AsynTaskPool::m_maxWorkerCount = maxWorkerCount;
	}

	uint AsynTaskPool::getMinWorkerCount() const
	{
		return m_minWorkerCount;
	}

	void AsynTaskPool::setMinWorkerCount(uint minWorkerCount)
	{
		AsynTaskPool::m_minWorkerCount = minWorkerCount;
	}

	ulong AsynTaskPool::getSleepTime() const
	{
		return m_sleepTime;
	}

	void AsynTaskPool::setSleepTime(ulong sleepTime)
	{
		AsynTaskPool::m_sleepTime = sleepTime;
	}




	AsynTask::AsynTask()
	{
		taskName = "";
	}

	AsynTask::~AsynTask()
	{

	}

}