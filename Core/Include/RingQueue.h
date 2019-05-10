#pragma once
#include "Sapphire.h"
#include "Atom.h"


namespace Sapphire
{

	struct RingQueueHead
	{
		volatile UINT32 head;
		byte padding1[SAPPHIRE_CACHE_LINE_SIZE - sizeof(UINT32)];

		volatile UINT32 tail;
		byte padding2[SAPPHIRE_CACHE_LINE_SIZE - sizeof(UINT32)];
	};



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
		static const size_type  uCapacity = (size_type)MAX(NextPowerOfTwo(Capacity), 2);
		static const index_type uMask = (index_type)(uCapacity - 1);

	public:
		RingQueueBase(bool bInitHead = false);
		~RingQueueBase();

	public:
		void dump_info();
		void dump_detail();

		index_type mask() const      { return uMask; };
		size_type capacity() const   { return uCapacity; };
		size_type length() const     { return sizes(); };
		size_type sizes() const;

		void init(bool bInitHead = false);

		int push(T * item);
		T * pop();

		int push2(T * item);
		T * pop2();

		int spin_push(T * item);
		T * spin_pop();

		int spin1_push(T * item);
		T * spin1_pop();

		int spin2_push(T * item);
		T * spin2_pop();

		int spin2_push_(T * item);

		int spin3_push(T * item);
		T * spin3_pop();

		int spin8_push(T * item);
		T * spin8_pop();

		int spin9_push(T * item);
		T * spin9_pop();

		int mutex_push(T * item);
		T * mutex_pop();

	protected:
		core_type       core;
		SpinMutex    spin_mutex;
		MutexEx queue_mutex;        //一读一写是无锁，超过加互斥锁
	};


	template <typename T, uint32_t Capacity, typename CoreType>
	RingQueueBase<T, Capacity, CoreType>::RingQueueBase(bool bInitHead /* = false */)
	{
		//printf("RingQueueBase::RingQueueBase();\n\n");

		init(bInitHead);
	};

	template <typename T, uint32_t Capacity, typename CoreType>
	RingQueueBase<T, Capacity, CoreType>::~RingQueueBase()
	{
		COMPILER_WRITE_BARRIER;

		spin_mutex.locked = 0;

	}

	template <typename T, uint32_t Capacity, typename CoreType>
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

		spin_mutex.Init();
		 
	}

}
