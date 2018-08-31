#include "Mutex.h"

#ifdef SAPPHIRE_WIN
#include <windows.h>
#elif defined(SAPPHIRE_ANDROID)
#include <pthread.h>
#elif defined(SAPPHIRE_LINUX)
#include <pthread.h>
#elif defined(IOS)
#include <pthread.h>
#endif


namespace Sapphire
{

#ifdef SAPPHIRE_WIN
	Mutex::Mutex() :m_handle(new CRITICAL_SECTION)
	{
		InitializeCriticalSection((CRITICAL_SECTION*)m_handle);
	}

	Mutex::~Mutex()
	{
		CRITICAL_SECTION* cs = (CRITICAL_SECTION*)m_handle;
		DeleteCriticalSection(cs);
		delete cs;
		m_handle = 0;
	}

	void Mutex::Acquire()
	{
		EnterCriticalSection((CRITICAL_SECTION*)m_handle);
	}

	void Mutex::Release()
	{
		LeaveCriticalSection((CRITICAL_SECTION*)m_handle);
	}
#else
	//Android/Linux/IOS都用pthread
	Mutex::Mutex() :
		m_handle(new pthread_mutex_t)
	{
		pthread_mutex_t* mutex = (pthread_mutex_t*)m_handle;
		pthread_mutexattr_t attr;
		pthread_mutexattr_init(&attr);
		pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE); //可重入鎖
		pthread_mutex_init(mutex, &attr);
	}

	Mutex::~Mutex()
	{
		pthread_mutex_t* mutex = (pthread_mutex_t*)m_handle;
		pthread_mutex_destroy(mutex);
		delete mutex;
		m_handle = 0;
	}

	void Mutex::Acquire()
	{
		pthread_mutex_lock((pthread_mutex_t*)m_handle);
	}

	void Mutex::Release()
	{
		pthread_mutex_unlock((pthread_mutex_t*)m_handle);
	}

#endif

	MutexAutoLock::MutexAutoLock(Mutex& mutex) :mutexRef(mutex)
	{
		mutexRef.Acquire();
	}

	MutexAutoLock::~MutexAutoLock()
	{
		mutexRef.Release();
	}

}