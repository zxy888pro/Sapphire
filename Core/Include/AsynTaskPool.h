#pragma once
#include "Sapphire.h"
#include "Thread.h"

namespace Sapphire
{
	class SAPPHIRE_CLASS AsynTask : public IRunnable
	{
		

	public:
		AsynTask();
		AsynTask(const char* name)
		{
			taskName = name;
		}
		~AsynTask();

		virtual void run() {};
		const std::string& getName() const { return taskName; }
		uint uPriority;
		volatile bool bCompleted;
		static bool SAPPHIRE_STDCALL ComparePtr(const AsynTask* lhs, const AsynTask* rhs)
		{
			return lhs->uPriority < rhs->uPriority;
		}
	protected:
		std::string taskName;


	};

	class AsynTaskThread;

	class SAPPHIRE_CLASS AsynTaskPool : public Thread, public SubSystem
	{

		SAPPHIRE_OBJECT(AsynTaskPool,SubSystem)

	public:

		typedef   std::vector<AsynTask*>  TASKITEM_QUEUE;    //工作项
		typedef std::list<AsynTask*> COMPLETEDITEM_QUEUE;    //完成列表
		typedef std::vector<AsynTaskThread*> WORKER_QUEUE;

		AsynTaskPool(Core* pCore);
		AsynTaskPool(Core* pCore,uint maxWorkerCount, uint minWorkerCount, ulong sleepTime);

		virtual ~AsynTaskPool();

		void Initialize();

		void AddTask(AsynTask*  pTask);

		bool RemoveTask(AsynTask*  pTask);

		void SetWorkerNum(int num);
		void AddWorker(bool autoExit);

		bool IsCompleted();

		AsynTask* Pop();

		uint GetThreadCount() const;

		uint GetTaskCount() const;

		void Clear();

		void Update();

		void Close();



		virtual void ThreadFunc() override;

	private:

		MutexEx  m_taskMutex;
		MutexEx  m_threadMutex;

		WORKER_QUEUE m_workers;

		TASKITEM_QUEUE   m_tasks;

		ulong  CreateID();


		ulong   m_ulIdOrder;
		ulong   m_sleepTime;
		bool   m_bIsRunning;
	private:
		uint   m_maxWorkerCount;
		uint   m_minWorkerCount;
		uint   m_curWorkerCount;
		uint   m_curTaskCount;

	public:
		uint getMaxWorkerCount() const;

		void setMaxWorkerCount(uint maxWorkerCount);

		uint getMinWorkerCount() const;

		void setMinWorkerCount(uint minWorkerCount);

		ulong getSleepTime() const;

		void setSleepTime(ulong sleepTime);



	};
}

