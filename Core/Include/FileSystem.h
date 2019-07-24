#pragma once


#include "Sapphire.h"
#include "Thread.h"
#include <unordered_set>

namespace Sapphire
{
	//文件子系统
	class SAPPHIRE_CLASS FileSystem : public SubSystem, public Thread
	{
		SAPPHIRE_OBJECT(FileSystem, SubSystem);

	public:

		FileSystem(Core* pCore);
		virtual ~FileSystem();

		std::string SetCurrentPath(const char* path);

		std::string GetCurrentPath() const;

		std::string CreateDir();

		void		Delete(const char*  fileName);

		void		Rename(const char*  oldName, const char*  newName);

		void        IsFileExist(const char*  fileName) const;

		void		RegisterAccessPath(const char* path);

		bool		CheckAccess(const char* path) const;


		virtual void ThreadFunc() override;


	private:

		std::unordered_set<std::string>   m_allowPaths;

		mutable std::string				  m_programPath;

	};



}