#pragma once
#include <SapphireDef.h>

namespace Sapphire
{
#ifdef ACTIVATE_MEMORY_MANAGER

	#define PADDING  0xDEADC0DE
	#define BODY  0xBAADC0DE
	#define BREAK_ON_DEALLOC  0x1
	#define BREAK_ON_REALLOC  0x2
	#define HASH_SIZE  1024*256

#define _New new
#define _Delete delete
#define _Malloc malloc
#define _Calloc calloc
#define _Realloc realloc
#define _Free free

   SAPPHIRE_API	void* AllocateMemory(const char *file, int line, size_t size, MemAllocType type, void *address = NULL);

   SAPPHIRE_API void DeallocateMemory(const char *file, int line, void *address, MemAllocType type);

   SAPPHIRE_API void BreakOnAllocation(int allocationCount);

   SAPPHIRE_API void BreakOnDeallocation(void *address);

   SAPPHIRE_API void BreakOnReallocation(void *address);

   SAPPHIRE_API void setPaddingSize(int size = 4);

   SAPPHIRE_API void setOwner(const char *file, int line);

   SAPPHIRE_CLASS void* operator new(size_t size);

   SAPPHIRE_CLASS void* operator new[](size_t size);

   SAPPHIRE_CLASS  void* operator new(size_t size, const char *file, int line);

   SAPPHIRE_CLASS void* operator new[](size_t size, const char *file, int line);
 

   SAPPHIRE_CLASS	void operator delete(void *address);

   SAPPHIRE_CLASS   void operator delete[](void *address);
  
	
   SAPPHIRE_CLASS void operator delete(void *address, const char *file, int line);
   SAPPHIRE_CLASS void operator delete[](void *address, const char *file, int line);

#define _New              new( __FILE__, __LINE__ )
#define _Delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
#define _Malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     AT_Malloc       )
#define _Calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, AT_Calloc       )
#define _Realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     AT_Realloc, ptr )
#define _Free(sz)         deAllocateMemory( __FILE__, __LINE__, sz,     AT_Free         )
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


	   int           m_breakOnAllocationCount;
	   unsigned int  m_paddingSize;
	   bool          m_logAlways;
	   bool          m_cleanLogFileOnFirstRun;


	   int          m_totalMemoryAllocations;  
	   unsigned int m_totalMemoryAllocated;   
	   unsigned int m_totalMemoryUsed;         

	   unsigned int m_peakMemoryAllocation;    
	   unsigned int m_peakTotalNumAllocations; 
	   unsigned int m_overheadMemoryCost;     
	   unsigned int m_peakOverHeadMemoryCost; 
	   unsigned int m_totalOverHeadMemoryCost;
	   unsigned int m_allocatedMemory;        
	   unsigned int m_numBoundsViolations;    

	  
	   myStack m_topStack;

	   unsigned int m_numAllocations;    
   private:
	   int getHashIndex(void *address);  //通过地址获取Hash表的索引
	   int calculateUnAllocatedMemory();   //计算未分配的内存

	   MemoryNode *m_hashTable[HASH_SIZE];   //跟踪内存分配的hash链表

	   MemoryNode *m_memoryCache;          //用于缓存不用的内存节点
   };
  
}