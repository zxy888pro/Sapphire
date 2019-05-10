#pragma once
#include "Sapphire.h"



namespace Sapphire
{
//ƒ⁄¥Ê∆¡’œ
#ifdef SAPPHIRE_WIN
#define MEMORY_BARRIER  MemoryBarrier()
#define MEMORY_READ_BARRIER MemoryBarrier()
#define MEMORY_WRITE_BARRIER  MemoryBarrier()
//±‡“Î∆˜∆¡’œ
#define MEMORY_READWRITE_BARRIER  _ReadWriteBarrier()
#define COMPILER_READ_BARRIER        _ReadBarrier()
#define COMPILER_WRITE_BARRIER        _WriteBarrier()
#define COMPILER_BARRIER        _ReadWriteBarrier()


//‘≠◊”≤Ÿ◊˜

#define val_compare_and_swap32(destPtr, oldValue, newValue)        \
    (int32_t)(InterlockedCompareExchange((volatile LONG *)(destPtr),    \
                            (LONG)(newValue), (LONG)(oldValue)))

#define val_compare_and_swap32u(destPtr, oldValue, newValue)       \
    (uint32_t)(InterlockedCompareExchange((volatile LONG *)(destPtr),   \
                            (LONG)(newValue), (LONG)(oldValue)))

#define val_compare_and_swap64(destPtr, oldValue, newValue)           \
    (int64_t)(InterlockedCompareExchange64((volatile LONG64 *)(destPtr),   \
                            (LONG64)(newValue), (LONG64)(oldValue)))

#define val_compare_and_swap64u(destPtr, oldValue, newValue)          \
    (uint64_t)(InterlockedCompareExchange64((volatile LONG64 *)(destPtr),  \
                            (LONG64)(newValue), (LONG64)(oldValue)))

#define bool_compare_and_swap32(destPtr, oldValue, newValue)       \
    (InterlockedCompareExchange((volatile LONG *)(destPtr),             \
                            (LONG)(newValue), (LONG)(oldValue))         \
                                == (LONG)(oldValue))

#define bool_compare_and_swap64(destPtr, oldValue, newValue)       \
    (InterlockedCompareExchange64((volatile LONG64 *)(destPtr),         \
                            (LONG64)(newValue), (LONG64)(oldValue))     \
                                == (LONG64)(oldValue))

#define test_and_set32(destPtr, newValue)                     \
    (int32_t)(InterlockedExchange((volatile LONG *)(destPtr), (LONG)(newValue)))

#define test_and_set32u(destPtr, newValue)                    \
    (uint32_t)(InterlockedExchange((volatile LONG *)(destPtr), (LONG)(newValue)))

#define test_and_set64(destPtr, newValue)                     \
    (int64_t)(InterlockedExchange64((volatile LONGLONG *)(destPtr),     \
                                    (LONGLONG)(newValue)))

#define test_and_set64u(destPtr, newValue)                    \
    (uint64_t)(InterlockedExchange64((volatile LONGLONG *)(destPtr),    \
                                    (LONGLONG)(newValue)))

#define fetch_and_add32(destPtr, addValue)                         \
    (uint32_t)(InterlockedExchangeAdd((volatile LONG *)(destPtr), (LONG)(addValue)))

#define fetch_and_add64(destPtr, addValue)                         \
    (uint64_t)(InterlockedExchangeAdd64((volatile LONGLONG *)(destPtr), \
                                        (LONGLONG)(addValue)))

#elif defined SAPPHIRE_ANDROID   
//gcc ƒ⁄¥Ê∆¡’œ
#define MEMORY_BARRIER     do { __sync_synchronize(); } while (0) 
#define MEMORY_READ_BARRIER    do { __sync_synchronize(); } while (0)
#define MEMORY_WRITE_BARRIER    do { __sync_synchronize(); } while (0)
#define MEMORY_READWRITE_BARRIER   do { __sync_synchronize(); } while (0)
//gcc ±‡“Î∆˜∆¡’œ
#define COMPILER_READ_BARRIER       do { __asm__ __volatile__ ("" : : : "memory"); } while (0)
#define COMPILER_WRITE_BARRIER       do { __asm__ __volatile__ ("" : : : "memory"); } while (0)
#define COMPILER_BARRIER  do { __asm__ __volatile__ ("" : : : "memory"); } while (0)  


#define val_compare_and_swap32(destPtr, oldValue, newValue)       \
    __sync_val_compare_and_swap((volatile int32_t *)(destPtr),         \
                            (int32_t)(oldValue), (int32_t)(newValue))

#define val_compare_and_swap32u(destPtr, oldValue, newValue)       \
    __sync_val_compare_and_swap((volatile uint32_t *)(destPtr),         \
                            (uint32_t)(oldValue), (uint32_t)(newValue))

#define val_compare_and_swap64(destPtr, oldValue, newValue)        \
    __sync_val_compare_and_swap((volatile int64_t *)(destPtr),          \
                            (int64_t)(oldValue), (int64_t)(newValue))

#define val_compare_and_swap64u(destPtr, oldValue, newValue)       \
    __sync_val_compare_and_swap((volatile uint64_t *)(destPtr),         \
                            (uint64_t)(oldValue), (uint64_t)(newValue))

#define val_compare_and_swap(destPtr, oldValue, newValue)          \
    __sync_val_compare_and_swap((destPtr), (oldValue), (newValue))

#define bool_compare_and_swap32(destPtr, oldValue, newValue)       \
    __sync_bool_compare_and_swap((volatile uint32_t *)(destPtr),        \
                            (uint32_t)(oldValue), (uint32_t)(newValue))

#define bool_compare_and_swap64(destPtr, oldValue, newValue)       \
    __sync_bool_compare_and_swap((volatile uint64_t *)(destPtr),        \
                            (uint64_t)(oldValue), (uint64_t)(newValue))

#define bool_compare_and_swap(destPtr, oldValue, newValue)         \
    __sync_bool_compare_and_swap((destPtr), (oldValue), (newValue))

#define test_and_set32(destPtr, newValue)                     \
    __sync_lock_test_and_set((volatile int32_t *)(destPtr),             \
                             (int32_t)(newValue))

#define test_and_set32u(destPtr, newValue)                    \
    __sync_lock_test_and_set((volatile uint32_t *)(destPtr),            \
                             (uint32_t)(newValue))

#define test_and_set64(destPtr, newValue)                     \
    __sync_lock_test_and_set((volatile int64_t *)(destPtr),             \
                             (int64_t)(newValue))

#define test_and_set64u(destPtr, newValue)                    \
    __sync_lock_test_and_set((volatile uint64_t *)(destPtr),            \
                             (uint64_t)(newValue))

#define fetch_and_add32(destPtr, addValue)                         \
    __sync_fetch_and_add((volatile uint32_t *)(destPtr),                \
                         (uint32_t)(addValue))

#define fetch_and_add64(destPtr, addValue)                         \
    __sync_fetch_and_add((volatile uint64_t *)(destPtr),                \
                         (uint64_t)(addValue))

#endif


	 

}
 