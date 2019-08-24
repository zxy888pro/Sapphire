#include "Sapphire.h"
#include "path.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <direct.h>
#include "sys/stat.h"
#ifdef SAPPHIRE_WIN
#include <windows.h>
#include <psapi.h>
#include <Shlwapi.h>
#endif // SAPPHIRE_WIN


SAPPHIRE_API bool Sapphire::FileIsExistA(const char* path)
{
#ifdef SAPPHIRE_WIN
	return PathFileExistsA(path);
#else
	struct _stat fileStat;
	if (!((_stat(path, &fileStat) == 0) && (fileStat.st_mode & _S_IFREG)))
	{
		return false;

	}
	return true;
#endif
	
}

SAPPHIRE_API bool Sapphire::FileIsExistW(const wchar_t* path)
{
#ifdef SAPPHIRE_WIN
	return PathFileExistsW(path);
#else
	struct _stat fileStat;
	if (!((_wstat(path, &fileStat) == 0) && (fileStat.st_mode & _S_IFREG)))
	{
		return false;

	}
	return true;
#endif
	
}

SAPPHIRE_API bool Sapphire::DirIsExistA(const char* path)
{
#ifdef SAPPHIRE_WIN
	return PathIsDirectoryA(path);
#else
	struct _stat fileStat;
	if (!((_stat(path, &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
	{
		return true;

	}
	return false;
#endif
	
}

SAPPHIRE_API bool Sapphire::DirIsExistW(const wchar_t* path)
{
#ifdef SAPPHIRE_WIN
	return PathIsDirectoryW(path);
#else
	struct _stat fileStat;
	if (!((_wstat(path, &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
	{
		return true;

	}
	return false;
#endif
	
}

SAPPHIRE_API size_t Sapphire::GetFileSizeA(const char* fileName)
{
	size_t size = 0;
	if (_access(fileName, 0) != -1)
	{
		FILE* f = fopen(fileName, "r+");
		_fseeki64(f, 0, SEEK_END);
		size = _ftelli64(f);
		fclose(f);
	}
	return size;
}

SAPPHIRE_API size_t Sapphire::GetFileSizeW(const wchar_t* fileName)
{
	size_t size = 0;
	if (_waccess(fileName, 0) != -1)
	{
		FILE* f = _wfopen(fileName, L"r+");
		_fseeki64(f, 0, SEEK_END);
		size = _ftelli64(f);
		fclose(f);
	}
	return size;
}

 std::string Sapphire::GetCurrentDirA()
{
#ifdef SAPPHIRE_WIN
	char   buffer[MAXPATH];
	_getcwd(buffer, MAXPATH);
	return std::string(buffer);
#else
	return "";
#endif
	
	
}

 std::wstring Sapphire::GetCurrentDirW()
{
#ifdef SAPPHIRE_WIN
	wchar_t   buffer[MAXPATH];
	_wgetcwd(buffer, MAXPATH);
	return std::wstring(buffer);
#else
	return L"";
#endif
}


 std::string Sapphire::GetCurrentProcessName()
 {
	 std::string processName;
#ifdef SAPPHIRE_WIN
	 char szPath[MAX_PATH] = { 0 };
	 GetModuleFileName(NULL, szPath, MAX_PATH);
	 processName = szPath;
#endif // SAPPHIRE_WIN
	 return processName;
 }
