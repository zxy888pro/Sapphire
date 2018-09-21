#include <WorkerQueue.h>
#include <Thread.h>
#include <Variant.h>

namespace Sapphire
{
	class Worker : public Thread, public RefCounter
	{
	public:

		Worker(WorkerQueue* owner, uint index)
		{
			m_owner = owner;
			m_index = index;
		}

		//Ïß³Ìº¯Êý
		virtual void ThreadFunc() override
		{
			 
		}

		uint GetIndex() const { return m_index; }

	private:
		WorkerQueue* m_owner;
		uint m_index;
	};

}