#pragma once
#include <SapphireDef.h>
#include <RefCounter.h>
namespace Sapphire
{
	struct WorkerItem: public RefCounter
	{
		friend class WorkerQueue;
	public:
		WorkerItem();

		uint uPriority;
		bool bSendEvent;
		volatile bool bCompleted;

	private:
		bool bPooled;
	};

	class SAPPHIRE_CLASS WorkerQueue
	{
	public:

		WorkerQueue();
		virtual ~WorkerQueue();

	private:

		

	};

}
