#pragma once


#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

// 平台定义
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( WIN32 ) || defined(_WIN64)
#	define SAPPHIRE_WIN
#ifndef WIN32
#	define WIN32
#endif
#elif defined(ANDROID) 
#	define SAPPHIRE_ANDROID
#elif defined(IOS)
#	define IOS
#else
# define SAPPHIRE_LINUX
#endif

#define MAXPATH 260
#define STRING_BUFFERSIZE  256

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
#include <assert.h>

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
//#ifdef _DEBUG 
//#define ACTIVATE_MEMORY_MANAGER
//#endif

#define ACTIVATE_MEMORY_MANAGER


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



//枚举转字符串
#define ENUM2STR(val) #val

//#define ACTIVATE_MEMORY_MANAGER

#ifdef SAPPHIRE_WIN
#define m_assert(x) if ((x) == false) __asm { int 3 }

//template<class _Kty,class _Ty,class _Hasher = hash<_Kty>,class _Keyeq = equal_to<_Kty>,class _Alloc = allocator<pair<const _Kty, _Ty> >
//using HashMap = std::unordered_map < _Kty, _Ty, _Hasher, _Keyeq > ;
#ifndef FORCEINLINE
#define FORCEINLINE __forceinline
#else 
#define FORCEINLINE inline
#endif
#ifndef  m_assert
#define m_assert(x) assert(x)
#endif // ! m_assert

#endif // !SAPPHIRE_WIN




//数据类型
typedef unsigned char byte;
typedef unsigned int bit32;
typedef unsigned int  uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned long long  ulonglong;


#include <EventDef.h>