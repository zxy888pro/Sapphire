#pragma once

#include <SapphireDef.h>

namespace Sapphire
{
	class SAPPHIRE_CLASS Mutex
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
	class SAPPHIRE_CLASS MutexAutoLock
	{
	public:

		MutexAutoLock(Mutex& mutex);
		~MutexAutoLock();

	private:

		Mutex& mutexRef;
	};
}
