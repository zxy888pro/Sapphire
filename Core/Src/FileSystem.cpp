#include "FileSystem.h"


namespace Sapphire
{

	FileSystem::FileSystem(Core* pCore) :SubSystem(pCore)
	{

	}

	FileSystem::~FileSystem()
	{

	}

	std::string FileSystem::SetCurrentPath(const char* path)
	{
		return path;
	}

	std::string FileSystem::GetCurrentPath() const
	{
		return "";
	}

	std::string FileSystem::CreateDir()
	{
		return "";
	}

	void FileSystem::Delete(const char* fileName)
	{

	}

	void FileSystem::Rename(const char* oldName, const char* newName)
	{

	}

	void FileSystem::IsFileExist(const char* fileName) const
	{

	}

	void FileSystem::RegisterAccessPath(const char* path)
	{

	}

	bool FileSystem::CheckAccess(const char* path) const
	{
		return DirIsExistA(path);
	}

	void FileSystem::ThreadFunc()
	{
		
	}

}