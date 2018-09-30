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

	ThreadID Thread::mainThreadID;

}