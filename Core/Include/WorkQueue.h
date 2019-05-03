#pragma once
#include <SapphireDef.h>
#include <RefCounter.h>
#include <Ptr.h>
#include <SubSystem.h>
#include <Mutex.h>
#include <queue>
#include <Variant.h>

namespace Sapphire
{
	class Worker;
	//任务项
	class WorkItem: public RefCounter
	{
		friend class WorkQueue;

	public:
		WorkItem();
		~WorkItem();

		virtual void Execute(){};

		static bool ComparePtr(const WorkItem* lhs, const WorkItem* rhs)
		{
			return lhs->uPriority < rhs->uPriority;
		}

		static bool Compare(const WorkItem& lhs, const WorkItem& rhs)
		{
			return lhs.uPriority < rhs.uPriority;
		}

		uint uPriority;
		bool bSendEvent;
		volatile bool bCompleted; 
		virtual void* GetData() { return NULL; }

	private:
		bool bPooled;
	};

	//任务队列
	class SAPPHIRE_CLASS WorkQueue : SubSystem
	{
		SAPPHIRE_OBJECT(WorkQueue, SubSystem)
				
	public:

		//typedef std::priority_queue<SharedPtr<WorkerItem>, std::vector<WorkerItem>, WorkerItem::ComparePtr> WORKITEM_QUEUE;
		typedef std::vector<SharedPtr<WorkItem>> WORKITEM_QUEUE;
		typedef std::list<SharedPtr<WorkItem>> COMPLETEDITEM_QUEUE;

		WorkQueue(Core* pCore);
		virtual ~WorkQueue();

		void Initialize();
		
		void AddWorkerItem(SharedPtr<WorkItem> pitem);  //添加任务项
		 
		bool RemoveWorkItem(SharedPtr<WorkItem> pitem);  //移除任务项

		SharedPtr<WorkItem> PickWorkItem();   //提取并移除优先级最高的任务项

		void Compeleted(SharedPtr<WorkItem> pitem);   //完成工作项

		void Pause();

		void Resume();

		void AddWorker(uint num);  //添加工人

		uint GetWorkerNum();

		bool IsCompleted();  //检查任务列表是否都完成

		void Clear();

		virtual void Update() override;

	private:

		Mutex  m_mutexA;  //工作队列锁
		Mutex  m_mutexB;  //完成队列锁
		//工作线程
		std::map<uint, SharedPtr<Worker>> m_workers;
		COMPLETEDITEM_QUEUE m_completedItems;
		//任务队列
		WORKITEM_QUEUE m_workQueue;
			
		uint   m_maxWorkerNum;
		uint   m_minWorkerNum;

		bool   m_bPaused;
		ulong  m_ulIdOrder;

		ulong  CreateID();

		void   PurgeCompletedQueue();
		

	public:

		//用变体Map作为数据参数
		sigslot::signal1<VariantMap&> WorkSignal;

	};




}
