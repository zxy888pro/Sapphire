#pragma once
#include <SapphireDef.h>
#include <stdlib.h>


SAPPHIRE_API	void* AllocateMemory(const char *file, int line, size_t size, MemAllocType type, void *address = NULL);

SAPPHIRE_API void DeallocateMemory(const char *file, int line, void *address, MemAllocType type);

SAPPHIRE_API void BreakOnAllocation(int allocationCount);

SAPPHIRE_API void BreakOnDeallocation(void *address);

SAPPHIRE_API void BreakOnReallocation(void *address);

SAPPHIRE_API void setPaddingSize(int size = 4);

SAPPHIRE_API void setOwner(const char *file, int line);

#ifdef ACTIVATE_MEMORY_MANAGER

void* operator new(size_t size);

void* operator new[](size_t size);

void* operator new(size_t size, const char *file, int line);

void* operator new[](size_t size, const char *file, int line);

void* operator new(uint size, const char *file, int line);

void operator delete(void *address);

void operator delete[](void *address);


void operator delete(void *address, const char *file, int line);

void operator delete[](void *address, const char *file, int line);

#ifndef _New
#define _New              new( __FILE__, __LINE__ )
#endif // !_New
#ifndef _Delete
#define _Delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
#endif // !_Delete
#ifndef _Malloc
#define _Malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     MAT_Malloc       )
#endif
#ifndef _Calloc
#define _Calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, MAT_Calloc       )
#endif
#ifndef _Realloc
#define _Realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     MAT_Realloc, ptr )
#endif
#ifndef _Free
#define _Free(sz)         DeallocateMemory( __FILE__, __LINE__, sz,     MAT_Free         )
#endif
#else
#ifndef _New
#define _New new
#endif // !_New
#ifndef _Delete
#define _Delete delete
#endif // !_Delete
#ifndef _Malloc
#define _Malloc malloc
#endif
#ifndef _Calloc
#define _Calloc calloc
#endif
#ifndef _Realloc
#define _Realloc realloc
#endif
#ifndef _Free
#define _Free free
#endif
#endif


#define PADDING  0xDEADC0DE
#define BODY  0xBAADC0DE
#define BREAK_ON_DEALLOC  0x1
#define BREAK_ON_REALLOC  0x2
#define HASH_SIZE  1024*256


namespace Sapphire
{
#ifdef ACTIVATE_MEMORY_MANAGER

//#undef new
//#undef delete
//#undef malloc
//#undef calloc
//#undef realloc
//#undef free


	//#define _New new
	//#define _Delete delete
	//#define _Malloc malloc
	//#define _Calloc calloc
	//#define _Realloc realloc
	//#define _Free free


//#define new              new( __FILE__, __LINE__ )
//#define delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
//#define malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     MAT_Malloc       )
//#define calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, MAT_Calloc       )
//#define realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     MAT_Realloc, ptr )
//#define free(sz)         DeallocateMemory( __FILE__, __LINE__, sz,     MAT_Free         )
#endif
   struct  StackNode
   {
	   const char *fileName;
	   unsigned short lineNumber;
	   StackNode *next;
   };

   struct MemoryNode
   {
	   size_t         actualSize;
	   size_t         reportedSize;
	   void          *actualAddress;
	   void          *reportedAddress;
	   char           sourceFile[30];
	   unsigned short sourceLine;
	   unsigned short paddingSize;
	   char           options;
	   long           predefinedBody;  //分配内存初始化的值
	   MemAllocType   allocationType;
	   MemoryNode    *next, *prev;

	   void InitializeMemory(long body = BODY);
   };

   class myStack
   {
   public:
	   myStack();
	   void init();
	 
	   bool empty();
	   
	   StackNode* top();
	  
	   void push(StackNode *n);
	    
	   StackNode* pop();
	  
   private:
	   StackNode *m_head;
	   int m_count;
   };

   class MemoryManager
   {
   public:
	   MemoryManager(void);
	   ~MemoryManager(void);

	   void initialize(void);      
	   void release(void);          


	   void insertMemoryNode(MemoryNode *node);      
	   MemoryNode *getMemoryNode(void *address);    
	   MemoryNode *removeMemoryNode(void *address); 
	   bool validateMemoryUnit(MemoryNode *node);  

	 
	   void deallocateMemory(MemoryNode *node);
	   MemoryNode* allocateMemory(void);   


	   void dumpLogReport(void);
	   void dumpMemoryAllocations(void);
	   void log(char *s, ...);
	   bool IsInitialized() const;

	   int           m_breakOnAllocationCount;	//分配中断数
	   unsigned int  m_paddingSize;     //padding大小
	   bool          m_logAlways;
	   bool          m_cleanLogFileOnFirstRun;


	   int          m_totalMemoryAllocations;   //总分配次数  
	   unsigned int m_totalMemoryAllocated;   //总分配内存
	   unsigned int m_totalMemoryUsed;         //总使用内存

	   unsigned int m_peakMemoryAllocation;    //分配内存峰值
	   unsigned int m_peakTotalNumAllocations; //分配数峰值
	   unsigned int m_overheadMemoryCost;     //内存开销
	   unsigned int m_peakOverHeadMemoryCost;  //峰值开销
	   unsigned int m_totalOverHeadMemoryCost;  //总开销
	   unsigned int m_allocatedMemory;        //以分配内存
	   unsigned int m_numBoundsViolations;    //越界次数

	  
	   myStack m_topStack;

	   unsigned int m_numAllocations;    
	   

   private:
	   int getHashIndex(void *address);  //通过地址获取Hash表的索引
	   int calculateUnAllocatedMemory();   //计算未分配的内存

	   MemoryNode *m_hashTable[HASH_SIZE];   //跟踪内存分配的hash链表

	   MemoryNode *m_memoryCache;          //用于缓存不用的内存节点
	   bool m_bIsInitialized;
   };
  
}