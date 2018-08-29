#pragma once

#include <SapphireDef.h>

namespace Sapphire
{
	class SAPPHIRE_API Mutex
	{
	public:
		Mutex();
		~Mutex();

		void Acquire();
		void Release();

	private:

		void*  m_handle;
	};

	//×Ô¶¯Ëø
	class SAPPHIRE_API MutexLock
	{
	public:

		MutexLock(Mutex& mutex);
		~MutexLock();

	private:

		Mutex& mutexRef;
	};
}
