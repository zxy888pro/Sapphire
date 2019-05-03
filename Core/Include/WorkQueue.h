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
	//������
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

	//�������
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
		
		void AddWorkerItem(SharedPtr<WorkItem> pitem);  //���������
		 
		bool RemoveWorkItem(SharedPtr<WorkItem> pitem);  //�Ƴ�������

		SharedPtr<WorkItem> PickWorkItem();   //��ȡ���Ƴ����ȼ���ߵ�������

		void Compeleted(SharedPtr<WorkItem> pitem);   //��ɹ�����

		void Pause();

		void Resume();

		void AddWorker(uint num);  //��ӹ���

		uint GetWorkerNum();

		bool IsCompleted();  //��������б��Ƿ����

		void Clear();

		virtual void Update() override;

	private:

		Mutex  m_mutexA;  //����������
		Mutex  m_mutexB;  //��ɶ�����
		//�����߳�
		std::map<uint, SharedPtr<Worker>> m_workers;
		COMPLETEDITEM_QUEUE m_completedItems;
		//�������
		WORKITEM_QUEUE m_workQueue;
			
		uint   m_maxWorkerNum;
		uint   m_minWorkerNum;

		bool   m_bPaused;
		ulong  m_ulIdOrder;

		ulong  CreateID();

		void   PurgeCompletedQueue();
		

	public:

		//�ñ���Map��Ϊ���ݲ���
		sigslot::signal1<VariantMap&> WorkSignal;

	};




}
