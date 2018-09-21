#pragma once
#include <SapphireDef.h>
#include <RefCounter.h>
#include <Ptr.h>
#include <SubSystem.h>
#include <Mutex.h>
namespace Sapphire
{
	class VariantMap;
	class Worker;
	//任务项
	class WorkerItem: public RefCounter
	{
		friend class WorkerQueue;

	public:
		WorkerItem();
		~WorkerItem();

		virtual void Execute(){};

		uint uPriority;
		bool bSendEvent;
		volatile bool bCompleted;    

	private:
		bool bPooled;
	};

	//任务队列
	class SAPPHIRE_CLASS WorkerQueue : SubSystem
	{
		SAPPHIRE_SUBSYSTEM(ESST_WORKERQUEUE)
				
	public:

		WorkerQueue();
		virtual ~WorkerQueue();

		void Initialize();
		
		void AddWorkerItem(SharedPtr<WorkerItem> pitem);

		bool RemoveWorkItem(SharedPtr<WorkerItem> pitem);

		void Pause();

		void Resume();

		void AddWorker(uint num);

		uint GetWorkerNum();

		bool IsCompleted();

	private:

		Mutex  m_mutex;
		//工作线程
		std::map<uint, Worker> m_workers;
		std::list<WorkerItem> m_pool;
			
		uint   m_maxWorkerNum;
		uint   m_minWorkerNum;
	public:

		//用变体Map作为数据参数
		sigslot::signal1<VariantMap&> WorkSignal;

	};




}
