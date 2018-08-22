#ifndef  __SAPPHIRE__
#define  __SAPPHIRE__

#pragma warning(disable: 4251)
#pragma warning(disable: 4275)

// ƽ̨����
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined( WIN32 ) || defined(_WIN64)
#	define SAPPHIRE_WIN
#else
#	define SAPPHIRE_LINUX
#endif

#define MAXPATH 260

///�o�B��/�ӑB��
//#define SAPPHIRE_STATIC_LIB

#ifdef SAPPHIRE_WIN
//WIN
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <tchar.h>
#include<time.h>
#include <unordered_map>

#if defined(SAPPHIRE_STATIC_LIB) && defined(SAPPHIRE_LIB)
#  define SAPPHIRE_CLASS
#  define SAPPHIRE_API   extern "C"
#elif defined(SAPPHIRE_DYNAMIC_LIB)&& defined(SAPPHIRE_LIB)
// �ӑB��
#  define SAPPHIRE_CLASS  __declspec(dllexport)
#  define SAPPHIRE_API    extern "C" __declspec(dllexport)
#else
#  define SAPPHIRE_CLASS  
#  define SAPPHIRE_API
#endif

//win32��unicode�Ͷ��ֽ��ַ����ĺ����ض���
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

// ��ȫɾ�����ͷ�
#ifndef safeDelete
#	define safeDelete(ptr)		if ((ptr)) {delete (ptr); (ptr) = 0;}
#endif
#ifndef safeDeleteArray
#	define safeDeleteArray(ptr)	if ((ptr)) {delete[] (ptr); (ptr) = 0;}
#endif
#ifndef safeRelease
#	define safeRelease(ptr)		if ((ptr)) {(ptr)->release(); (ptr) = 0;}
#endif

//��������
typedef unsigned char byte;
typedef unsigned int bit32;
typedef unsigned int  uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned long long  ulonglong;


#endif //__SAPPHIRE__
