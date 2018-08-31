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

	//�Զ���
	class SAPPHIRE_CLASS MutexAutoLock
	{
	public:

		MutexAutoLock(Mutex& mutex);
		~MutexAutoLock();

	private:

		Mutex& mutexRef;
	};
}
