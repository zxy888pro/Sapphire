#include <Thread.h>

#ifdef SAPPHIRE_WIN
#include <windows.h>
#include <atomic>
#elif defined(SAPPHIRE_ANDROID)
#include <pthread.h>
#elif defined(SAPPHIRE_LINUX)
#include <pthread.h>
#elif defined(IOS)
#include <pthread.h>
#endif

namespace Sapphire
{

//线程函数入口
#ifdef SAPPHIRE_WIN
	DWORD	WINAPI  ThreadFuncEntry(void* pdata)
	{
		Thread* pThread = static_cast<Thread*>(pdata);
		//执行线程函数
		pThread->ThreadFunc();
		return 0;
	}
#else

	void* ThreadFuncEntry(void* pdata)
	{
		Thread* pThread = static_cast<Thread*>(pdata);
		pThread->ThreadFunc();
		pthread_exit((void*)0);
		return 0;
	}

#endif

	Thread::Thread() :m_handle(0), m_shouldRun(false)
	{

	}

	Thread::~Thread()
	{
		Stop();
	}

	void Thread::ThreadFunc()
	{

	}

	bool Thread::Run()
	{
		//已经分配句柄，不能运行
		if (m_handle)
			return false;

		m_shouldRun = true;
#ifdef SAPPHIRE_WIN
		//用一个静态函数做统一入口， 在把自己做参数传进去
		m_handle = CreateThread(0, 0, ThreadFuncEntry, this, 0, 0);

#else
		m_handle = new pthread_t;
		pthread_attr_t type;
		pthread_attr_init(&type);
		pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
		pthread_create((pthread_t*)m_handle, &type, ThreadFuncEntry, this);
		
#endif
		return m_handle == 0;
	}

	void Thread::Stop()
	{
		if (m_handle == NULL)
		{
			return;
		}
#ifdef SAPPHIRE_WIN
		WaitForSingleObject((HANDLE)m_handle, INFINITE);
		CloseHandle((HANDLE)m_handle);
#else
		pthread_t* pThread = (pthread_t*)m_handle;
		if (pThread)
			pthread_join(*pThread, 0);
		delete thread;
#endif
		m_handle = NULL;
	}



	bool Thread::IsStarted()
	{

		return m_handle == NULL;
	}

	void Thread::SetPriority(int priority)
	{
#ifdef SAPPHIRE_WIN
		if (m_handle)
			SetThreadPriority((HANDLE)m_handle, priority);
#elif defined(SAPPHIRE_LINUX) && !defined(SAPPHIRE_ANDROID)
		pthread_t* pThread = (pthread_t*)m_handle;
		if (pThread)
			pthread_setschedprio(*pThread, priority);
#endif
	}

	void Thread::SetMainThread()
	{
		mainThreadID = GetCurThreadID();
	}

	ThreadID Thread::GetCurThreadID()
	{
#ifdef SAPPHIRE_WIN
		return GetCurrentThreadId();
#else
		return pthread_self();
#endif
	}

	bool Thread::IsMainThread()
	{
		return GetCurrentThreadId() == mainThreadID;
	}

	void* Thread::GetHandle() const
	{
		return m_handle;
	}

	ThreadID Thread::mainThreadID;


	Mutex::Mutex()
	{
#ifdef SAPPHIRE_WIN
		InitializeCriticalSection(&m_cs);
#else
		pthread_mutexattr_init(&m_attr);
		pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE);
		pthread_mutex_init(&m_cs, &m_attr);
#endif
#ifdef SAPPHIRE_DEBUG
		file = 0, line = 0;
#endif
	}


	Mutex::~Mutex()
	{
#ifdef SAPPHIRE_WIN
		DeleteCriticalSection(&m_cs);
#else
		pthread_mutexattr_destroy(&m_attr);
		pthread_mutex_destroy(&m_cs);
#endif
	}

#ifdef SAPPHIRE_DEBUG
	void Mutex::Lock(const char* f, int l)
	{
		sapphire_mutex_lock(&m_cs);
		file = f; line = l;
	}
	bool Mutex::TryLock()
	{
		return sapphire_mutex_trylock(&m_cs);
	}

	void Mutex::Unlock()
	{
		sapphire_mutex_unlock(&m_cs);
		file = (const char*)0xaabbccdd; line = 0x12345678;
	}
#else
	void Mutex::Lock()
	{
		sapphire_mutex_lock(&m_cs);
	}

	bool Mutex::TryLock()
	{
		return sapphire_mutex_trylock(&m_cs);
	}

	void Mutex::Unlock()
	{
		sapphire_mutex_unlock(&m_cs);
	}
#endif // !SAPPHIRE_DEBUG

	ThreadEx::ThreadEx() :Thread()
	{

	}

	void ThreadEx::ThreadFunc()
	{
		if (m_pTask)
		{
			m_pTask->run();
		}
	}

	void ThreadEx::Run(IRunnable* task)
	{
		m_pTask = task;
		Thread::Run();
		
	}

}

