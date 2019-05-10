#pragma once

#include <SapphireDef.h>


#ifndef SAPPHIRE_WIN
#include <pthread.h>
typedef pthread_t ThreadID;
#else
typedef uint ThreadID;

#include <windows.h>
#endif




namespace Sapphire
{

#define MUTEX_MAX_SPIN_COUNT  16    //最大自旋次数

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


	class SAPPHIRE_CLASS MutexEx
	{
#ifdef SAPPHIRE_WIN
	private:
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
		MutexEx();
		~MutexEx();

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


	//实现自旋锁
	class SAPPHIRE_CLASS SpinMutex
	{
	public:
		SpinMutex()
		{
			
		}
		~SpinMutex()
		{
		}
		void Init()
		{
			locked = 0;
			spin_counter = MUTEX_MAX_SPIN_COUNT;
			recurse_counter = 0;
			thread_id = 0;
			reserve = 0;
		}
		void UnLock()
		{
			locked = 0;
		}
		volatile char padding1[SAPPHIRE_CACHE_LINE_SIZE];
		volatile uint32_t locked;
		volatile char padding2[SAPPHIRE_CACHE_LINE_SIZE - 1 * sizeof(uint32_t)];
		volatile uint32_t spin_counter;
		volatile uint32_t recurse_counter;
		volatile uint32_t thread_id;
		volatile uint32_t reserve;
		volatile char padding3[SAPPHIRE_CACHE_LINE_SIZE - 4 * sizeof(uint32_t)];
	};



	//资源自动守护
	template<class _Mutex = MutexEx>
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