
//#define new              new( __FILE__, __LINE__ )
//#define delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
//#define malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     AT_Malloc       )
//#define calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, AT_Calloc       )
//#define realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     AT_Realloc, ptr )
//#define free(sz)         deAllocateMemory( __FILE__, __LINE__, sz,     AT_Free         )

#define _New              new( __FILE__, __LINE__ )
#define _Delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
#define _Malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     AT_Malloc       )
#define _Calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, AT_Calloc       )
#define _Realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     AT_Realloc, ptr )
#define _Free(sz)         deAllocateMemory( __FILE__, __LINE__, sz,     AT_Free         )