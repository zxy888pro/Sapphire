#pragma once

#include <SapphireDef.h>
#include "Atom.h"

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
#define sapphire_sleep(t)	Sleep(t)       //睡眠
#define sapphire_yield()	(int)SwitchToThread()	//出让一下线程
#define sapphire_pause()    _mm_pause()   //PAUSE指令提升了自旋等待循环（spin-wait loop）的性能
#else
#  define sapphire_mutex_t           pthread_mutex_t
#  define sapphire_thread_t          pthread_t
#  define sapphire_mutex_init(m)     pthread_mutex_init(m, NULL)
#  define sapphire_mutex_lock(m)     pthread_mutex_lock(m)
#  define sapphire_mutex_trylock(m)  (pthread_mutex_trylock(m)==0)
#  define sapphire_mutex_unlock(m)   pthread_mutex_unlock(m)
#  define sapphire_mutex_destroy(m)  pthread_mutex_destroy(m)
#define sapphire_sleep(t)	usleep(t);
#define sapphire_yield()	(int)pthread_yield();	
##define sapphire_pause()    sapphire_yield()
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
	class SAPPHIRE_CLASS SpinLock
	{
	public:
		 

		SpinLock() :locked(0), yield_threshold(3)
		{
			
		}
		SpinLock(UINT32 _yieldThreshold):
			locked(0), 
			yield_threshold(_yieldThreshold)
		{

		}
		~SpinLock()
		{
		}
		
		void Lock();
		
		void UnLock();
		
	private:

		volatile UINT32 locked;   //0未锁 1锁
		volatile UINT32 yield_threshold;

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