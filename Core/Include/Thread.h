#pragma once

#include <SapphireDef.h>


#ifndef SAPPHIRE_WIN
#include <pthread.h>
typedef pthread_t ThreadID;
#else
typedef uint ThreadID;
#endif




namespace Sapphire
{

#ifdef SAPPHIRE_WIN
#  define sapphire_mutex_t           CRITICAL_SECTION
#  define sapphire_thread_t          HANDLE
#  define sapphire_mutex_init(m)     InitializeCriticalSection(m)
#  define sapphire_mutex_lock(m)     EnterCriticalSection(m)
#  define sapphire_mutex_trylock(m)  TryEnterCriticalSection(m)
#  define sapphire_mutex_unlock(m)   LeaveCriticalSection(m)
#  define sapphire_mutex_destroy(m)  DeleteCriticalSection(m)
#else
#  define sapphire_mutex_t           pthread_mutex_t
#  define sapphire_thread_t          pthread_t
#  define sapphire_mutex_init(m)     pthread_mutex_init(m, NULL)
#  define sapphire_mutex_lock(m)     pthread_mutex_lock(m)
#  define sapphire_mutex_trylock(m)  (pthread_mutex_trylock(m)==0)
#  define sapphire_mutex_unlock(m)   pthread_mutex_unlock(m)
#  define sapphire_mutex_destroy(m)  pthread_mutex_destroy(m)

#endif


	class SAPPHIRE_CLASS Mutex
	{
#ifdef SAPPHIRE_WIN
		CRITICAL_SECTION m_cs;
#else
		pthread_mutex_t m_cs;
		pthread_mutexattr_t m_attr;
#endif

#ifdef SAPPHIRE_DEBUG
		const char* file;
		int			line;
#endif

	public:
		Mutex();
		~Mutex();

#ifdef SAPPHIRE_DEBUG
		void Lock(const char* f = 0, int l = 0);
		bool TryLock();
		void Unlock();
#else
		void Lock();
		bool TryLock();
		void Unlock();
#endif

	};

	//资源自动守护
	template<class _Mutex = Mutex>
	class ResGuard
	{
		_Mutex& mMutex;
	public:
#ifdef SAPPHIRE_DEBUG
		ResGuard(_Mutex& mtx, const char* f = 0, int l = 0) : mMutex(mtx)	{ mMutex.Lock(f, l); }
		~ResGuard()														{ mMutex.Unlock(); }
#else
		ResGuard(_Mutex& mtx) : mMutex(mtx)								{ mMutex.Lock(); }
		~ResGuard()														{ mMutex.Unlock(); }
#endif
	};

	struct IRunnable
	{
		virtual void run() = 0;
	};


	class SAPPHIRE_CLASS Thread
	{
	public:
		Thread();
		virtual ~Thread();

		virtual void ThreadFunc() = 0;

		virtual bool Run();

		void Stop();

		bool IsStarted();

		void SetPriority(int priority);

		static void SetMainThread();

		static ThreadID GetCurThreadID();

		static bool IsMainThread();

		void* GetHandle() const;

	protected:

		void* m_handle;

		volatile bool m_shouldRun;

		static ThreadID mainThreadID;
	};


	



	class SAPPHIRE_CLASS ThreadEx : public Thread
	{
	public:

		ThreadEx();
		virtual ~ThreadEx();

		virtual void ThreadFunc();

		virtual void Run(IRunnable* task);
		
	private:

		IRunnable* m_pTask;
 
	};
}