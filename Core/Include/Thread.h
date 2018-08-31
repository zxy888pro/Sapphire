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




	class SAPPHIRE_CLASS Thread
	{
	public:
		Thread();
		virtual ~Thread();

		virtual void ThreadFunc() = 0;

		bool Run();

		void Stop();

		bool IsStarted();

		void SetPriority(int priority);

		static void SetMainThread();

		static ThreadID GetCurThreadID();

		static bool IsMainThread();

	protected:

		void* m_handle;

		volatile bool m_shouldRun;

		static ThreadID mainThreadID;
	};
}