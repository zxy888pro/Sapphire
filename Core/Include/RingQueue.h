#pragma once
#include "Sapphire.h"
#include "Atom.h"
#include "Thread.h"

namespace Sapphire
{

	//记录RingQueue首尾的序号
	struct RingQueueHead
	{
		////把head/tail在分割在不同Cache Line中，使得head/tail同时被不同的线程高速读取
		volatile UINT32 head;
		byte padding1[SAPPHIRE_CACHE_LINE_SIZE - sizeof(UINT32)]; 
		

		volatile UINT32 tail;
		byte padding2[SAPPHIRE_CACHE_LINE_SIZE - sizeof(UINT32)];
	};



	//RingQueue核心
	template <typename T, UINT32 Capacity>
	class RingQueueCore
	{
	public:
		typedef T *         item_type;

		RingQueueCore()  {}
		~RingQueueCore() {}

	public:
		static const bool bIsAllocOnHeap = true;

	public:
		RingQueueHead       info;

		item_type*           queue;
	};



	template <typename T, UINT32 Capacity = 16U,
		typename CoreType = RingQueueCore<T, Capacity> >
	class RingQueueBase
	{
	public:
		typedef UINT32                    size_type;
		typedef UINT32                    index_type;
		typedef T *                         value_type;
		typedef typename CoreType::item_type  item_type;
		typedef CoreType                      core_type;
		typedef T *                         pointer;
		typedef const T *                   const_pointer;
		typedef T &                         reference;
		typedef const T &                   const_reference;

	public:
		static const size_type  uCapacity = (size_type)MAX(NextPowerOfTwo(Capacity), 2);   //容量，用2的n次幂的话，可以通过拿节点的序号和掩码的&（求模）取得在ringBuffer的数组的实际索引位置
		static const index_type uMask = (index_type)(uCapacity - 1);   //掩码,用于模计算实际索引

	public:
		RingQueueBase(bool bInitHead = false);
		~RingQueueBase();

	public:
		void dump_info(){};
		void dump_detail(){};

		index_type mask() const      { return uMask; };
		size_type capacity() const   { return uCapacity; };
		size_type length() const     { return sizes(); };
		size_type sizes() const;

		void init(bool bInitHead = false);

		int push(T * item);
		T * pop();

	protected:
		core_type       core;
		SpinLock    spinlock;
		MutexEx queue_mutex;        //一读一写是无锁，超过加互斥锁
	};

	template <typename T, UINT32 Capacity /*= 16U*/,
		typename CoreType /*= RingQueueCore<T, Capacity> */>
		Sapphire::RingQueueBase<T, Capacity, CoreType>::RingQueueBase(bool bInitHead /*= false*/)
	{
		init(bInitHead);
	}

	template <typename T, UINT32 Capacity /*= 16U*/,
		typename CoreType /*= RingQueueCore<T, Capacity> */>
		T * Sapphire::RingQueueBase<T, Capacity, CoreType>::pop()
	{
		index_type head, tail, next;
		value_type item;
		spinlock.Lock(); //自旋锁
		head = core.info.head;
		tail = core.info.tail;
		if ((tail == head) || (tail > head && (head - tail) > uMask)) {
			COMPILER_BARRIER;
			spinlock.UnLock();
			return (value_type)NULL;
		}
		next = tail + 1;
		core.info.tail = next;

		item = core.queue[tail & uMask];
		COMPILER_BARRIER;
		spinlock.UnLock();
		return item;

	}

	template <typename T, UINT32 Capacity /*= 16U*/,
		typename CoreType /*= RingQueueCore<T, Capacity> */>
		int Sapphire::RingQueueBase<T, Capacity, CoreType>::push(T * item)
	{
		index_type head, tail, next;
		spinlock.Lock(); //自旋锁
		head = core.info.head;
		tail = core.info.tail;
		if ((head - tail) > uMask) {
			COMPILER_BARRIER;
			// 队列已满, 释放锁
			spinlock.UnLock();
			return -1;
		}
		next = head + 1;
		core.info.head = next;

		core.queue[head & uMask] = item;    // 把数据写入队列

		COMPILER_BARRIER;        // 编译器读写屏障

		spinlock.UnLock();
	}

	template <typename T, UINT32 Capacity, typename CoreType>
	RingQueueBase<T, Capacity, CoreType>::~RingQueueBase()
	{
		COMPILER_WRITE_BARRIER;

		spinlock.UnLock();

	}

	template <typename T, UINT32 Capacity, typename CoreType>
	inline
		void RingQueueBase<T, Capacity, CoreType>::init(bool bInitHead /* = false */)
	{
		//RingQueue初始化

		if (!bInitHead) {
			core.info.head = 0;
			core.info.tail = 0;
		}
		else {
			memset((void *)&core.info, 0, sizeof(core.info));
		}

		COMPILER_WRITE_BARRIER;
		 
	}

	template <typename T, UINT32 Capacity, typename CoreType>
	inline
		typename RingQueueBase<T, Capacity, CoreType>::size_type
		RingQueueBase<T, Capacity, CoreType>::sizes() const
	{
		index_type head, tail;

		COMPILER_BARRIER;

		head = core.info.head;

		tail = core.info.tail;
		//如果首尾序号差小于掩码，直接返回序号差，否则超出了
		return (size_type)((head - tail) <= uMask) ? (head - tail) : (size_type)-1;
	}

	
	template <typename T, UINT32 Capacity = 1024>
	class RingQueue : public RingQueueBase < T, Capacity, RingQueueCore<T, Capacity> >
	{
	public:
		typedef UINT32                    size_type;
		typedef UINT32                    index_type;
		typedef T *                         value_type;
		typedef T *                         pointer;
		typedef const T *                   const_pointer;
		typedef T &                         reference;
		typedef const T &                   const_reference;

		typedef RingQueueCore<T, Capacity>   core_type;
		
		RingQueue(bool bFillQueue = true, bool bInitHead = true);
		~RingQueue();


	protected:


	};

	template <typename T, UINT32 Capacity /*= 1024*/>
	Sapphire::RingQueue<T, Capacity>::~RingQueue()
	{
		if (RingQueueCore<T, Capacity>::bIsAllocOnHeap) {
			if (this->core.queue != NULL) {
				delete[] this->core.queue;
				this->core.queue = NULL;
			}
		}
	}

	template <typename T, UINT32 Capacity /*= 1024*/>
	Sapphire::RingQueue<T, Capacity>::RingQueue(bool bFillQueue /*= true*/, bool bInitHead /*= true*/)
	{
		value_type *newData = new T *[uCapacity];
		if (newData != NULL) {
			if (bFillQueue) {
				memset((void *)newData, 0, sizeof(value_type) * uCapacity);
			}
			this->core.queue = newData;
		}
	};

}
