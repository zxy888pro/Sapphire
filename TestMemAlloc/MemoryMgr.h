#pragma  once


#ifdef _DEBUG 
#define ACTIVATE_MEMORY_MANAGER
#endif

#define ACTIVATE_MEMORY_MANAGER

enum AllocType
{
	AT_Unknown,
	AT_New,
	AT_NewArray,
	AT_Malloc,
	AT_Calloc,
	AT_Realloc,
	AT_Delete,
	AT_DeleteArray,
	AT_Free
};
const char MM_FREE = 8;
#ifdef ACTIVATE_MEMORY_MANAGER

#include "new_off.h"
#include <stdlib.h>
/*******************************************************************************************/
// ***** User interface, these methods can be used to set parameters within the Memory 
// ***** Manager to control the type and extent of the memory tests that are performed.  Note 
// ***** that it is not necessary to call any of these methods, you will get the default 
// ***** Memory Manager automatically.

void dumpLogReport(void);
/* dumpLogReport():
*  Dump the log report to the file, this is the same method that is automatically called
*  upon the programs termination to report all statistical information.
*/

void dumpMemoryAllocations(void);
/* dumpMemoryAllocations():
*  Report all allocated memory to the log file.
*/

void setLogFile(char *file);
/* setLogFile():
*  Allows for the log file to be changed from the default.
*/

void setExhaustiveTesting(bool test = true);
/* setExhaustiveTesting():
*  This method allows for exhaustive testing.  It has the same functionality as the following
*  function calls => setLogAlways( true ); setPaddingSize( 1024 );
*/

void setLogAlways(bool log = true);
/* setLogAlways():
*  Sets the flag for exhaustive information logging.  All information is sent to the log file.
*/

void setPaddingSize(int size = 4);
/* setPaddingSize():
*  Sets the padding size for memory bounds checks.
*/

void cleanLogFile(bool clean = true);
/* cleanLogFile():
*  Cleans out the log file by deleting it.
*/

void breakOnAllocation(int allocationCount);
/* breakOnAllocation():
*  Allows you to set a break point on the n-th allocation.
*/

void breakOnDeallocation(void *address);
/* breakOnDeallocation():
*  Sets a flag that will set a break point when the specified memory is deallocated.
*/

void breakOnReallocation(void *address);
/* breakOnReallocation():
*  Sets a flag that will set a break point when the specified memory is reallocated by
*  using the realloc() method.
*/

void *AllocateMemory(const char *file, int line, size_t size, AllocType type, void *address = NULL);
/* AllocateMemory():
*  This is the main memory allocation routine, this is called by all of the other
*  memory allocation routines to allocate and track memory.
*/

void deAllocateMemory(const char *file, int line, void *address, AllocType type);
/* deAllocateMemory():
*  This is the main memory deallocation routine.  This method is used by all of the
*  other de-allocation routines for de-allocating and tracking memory.
*/

void setOwner(const char *file, int line);
/* setOwner():
*  This method is used by the deallocation methods to record the source file and line
*  number that is requesting the allocation.  Note that it is important to create a
*  seperate method for deallocation since we can not pass the addition parameters to
*  the delete methods like we do with the new methods.
*/


class Initialize { public: Initialize(); };
static Initialize InitMemoryManager;

/*******************************************************************************************/
// ***** Global overloaded new/delete operators

// ***** These two routines should never get called, however they are provided here for 
// ***** clarity and to through.
inline void* operator new(size_t size){ return malloc(size); }
inline void* operator new[](size_t size)  { return malloc(size); }

/**
* operator new():
*  Here is the overloaded new operator, responsible for allocating and tracking the requested
*  memory.
*
*  Return Type: void* -> A pointer to the requested memory.
*  Arguments:
*  	size_t size	     : The size of memory requested in BYTES
*  	const char *file : The file responsible for requesting the allocation.
*  	int line	       : The line number within the file requesting the allocation.
*/
inline void* operator new(size_t size, const char *file, int line)
{
	return AllocateMemory(file, line, size, AllocType::AT_New);
}

/*******************************************************************************************/

/**
* operator new[]():
*  Here is the overloaded new[] operator, responsible for allocating and tracking the
*  requested memory.
*
*  Return Type: void* -> A pointer to the requested memory.
*  Arguments:
*  	size_t size	     : The size of memory requested in BYTES
*  	const char *file : The file responsible for requesting the allocation.
*  	int line	       : The line number within the file requesting the allocation.
*/
inline void* operator new[](size_t size, const char *file, int line)
{
	return AllocateMemory(file, line, size, AllocType::AT_NewArray);
}

/*******************************************************************************************/

/**
* operator delete():
*  This routine is responsible for de-allocating the requested memory.
*
*  Return Type: void
*  Arguments:
*  	void *address	: A pointer to the memory to be de-allocated.
*/
inline void operator delete(void *address)
{
	if (!address) return;  // ANSI states that delete will allow NULL pointers.

	deAllocateMemory(__FILE__, __LINE__, address, AllocType::AT_Delete);
}

/*******************************************************************************************/

/**
* operator delete[]():
*  This routine is responsible for de-allocating the requested memory.
*
*  Return Type: void
*  Arguments:
*  	void *address	: A pointer to the memory to be de-allocated.
*/
inline void operator delete[](void *address)
{
	if (!address) return;  // ANSI states that delete will allow NULL pointers.

	deAllocateMemory(__FILE__, __LINE__, address, AllocType::AT_DeleteArray);
}

// ***** These two routines should never get called, unless an error occures during the 
// ***** allocation process.  These need to be defined to make Visual C++ 6.0 happy.
// ***** If there was an allocation problem these method would be called automatically by 
// ***** the operating system.  C/C++ Users Journal (Vol. 19 No. 4 -> April 2001 pg. 60)  
// ***** has an excellent explanation of what is going on here.
inline void operator delete(void *address, const char *file, int line)   { free(address); }
inline void operator delete[](void *address, const char *file, int line) { free(address); }

bool InitializeMemoryManager();
void releaseMemoryManager();
#include "new_on.h"

#endif // ACTIVATE_MEMORY_MANAGER

