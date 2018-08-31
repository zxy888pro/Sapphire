#pragma once


#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

// 平台定义
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( WIN32 ) || defined(_WIN64)
#	define SAPPHIRE_WIN
#elif defined(ANDROID) 
#	define SAPPHIRE_ANDROID
#elif defined(IOS)
#	define IOS
#else
# define SAPPHIRE_LINUX
#endif

#define MAXPATH 260

///靜態庫/動態庫
//#define SAPPHIRE_STATIC_LIB

#ifdef SAPPHIRE_WIN
//WIN
#include <ctype.h>
#include <string.h>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <tchar.h>
#include<time.h>
#include <stdlib.h>
#include <unordered_map>

#if defined(SAPPHIRE_STATIC_LIB) && defined(SAPPHIRE_LIB)
#  define SAPPHIRE_CLASS
#  define SAPPHIRE_API   extern "C"
#elif defined(SAPPHIRE_DYNAMIC_LIB)&& defined(SAPPHIRE_LIB)
// 動態庫
#  define SAPPHIRE_CLASS  __declspec(dllexport)
#  define SAPPHIRE_API    extern "C" __declspec(dllexport)
#else
#  define SAPPHIRE_CLASS  
#  define SAPPHIRE_API
#endif

//win32下unicode和多字节字符集的函数重定义
#ifdef UNICODE

#define  FileOpen _wfopen
#define  GetCwd   _wgetcwd
#define  Sprintf  wsprintf
#define  TString std::wstring
#define  Strcpy wstrcpy
#define  Strlen wstrlen
#define  StringFormat  StringFormatW
#define  Replace ReplaceW
#define  FileIsExist FileIsExistW
#define	 DirIsExist DirIsExistW
#define  GetFileSize GetFileSizeW   
#define  TFStream    std::wfstream;
#define  GetCurrentDir GetCurrentDirW
#else

#define  TString std::string
#define  FileOpen fopen
#define  GetCwd   _getcwd
#define  Sprintf  sprintf_s
#define  Strcpy  strcpy
#define  Strlen strlen
#define  StringFormat  StringFormatA
#define  Replace ReplaceA
#define  FileIsExist FileIsExistA
#define	 DirIsExist DirIsExistA
#define  GetFileSize GetFileSizeA
#define   TFStream    std::fstream
#define  GetCurrentDir GetCurrentDirA
#endif


#else
//LINUX

#endif //END PARTFORM

#ifndef NULL
#define NULL 0
#endif

// 安全删除和释放
#ifndef safeDelete
#	define safeDelete(ptr)		if ((ptr)) {delete (ptr); (ptr) = 0;}
#endif
#ifndef safeDeleteArray
#	define safeDeleteArray(ptr)	if ((ptr)) {delete[] (ptr); (ptr) = 0;}
#endif
#ifndef safeRelease
#	define safeRelease(ptr)		if ((ptr)) {(ptr)->release(); (ptr) = 0;}
#endif


//内存管理器
#ifdef _DEBUG 
#define ACTIVATE_MEMORY_MANAGER
#endif

#ifdef ACTIVATE_MEMORY_MANAGER
//#undef new
//#undef delete
//#undef malloc
//#undef calloc
//#undef realloc
//#undef free
//#define new              new( __FILE__, __LINE__ )
//#define delete           (setOwner( __FILE__, __LINE__ ), false) ? setOwner( "", 0 ) : delete
//#define malloc(sz)       AllocateMemory(   __FILE__, __LINE__, sz,     AT_Malloc       )
//#define calloc(num, sz)  AllocateMemory(   __FILE__, __LINE__, sz*num, AT_Calloc       )
//#define realloc(ptr, sz) AllocateMemory(   __FILE__, __LINE__, sz,     AT_Realloc, ptr )
//#define free(sz)         deAllocateMemory( __FILE__, __LINE__, sz,     AT_Free         )
#endif // ACTIVATE_MEMORY_MANAGER



enum MemAllocType
{
	MAT_Unknown,
	MAT_New,
	MAT_NewArray,
	MAT_Malloc,
	MAT_Calloc,
	MAT_Realloc,
	MAT_Delete,
	MAT_DeleteArray,
	MAT_Free
};

#ifdef _DEBUG 
#define ACTIVATE_MEMORY_MANAGER
#endif

//数据类型
typedef unsigned char byte;
typedef unsigned int bit32;
typedef unsigned int  uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned long long  ulonglong;