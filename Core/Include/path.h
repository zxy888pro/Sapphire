#ifndef  __PATH__H__
#define  __PATH__H__

#include "SapphireDef.h"

#define  CHAR_SLASH			'\\'
#define  CHAR_DOT			'.'
#define  CHAR_SLASH2		'/'

namespace Sapphire
{
	SAPPHIRE_API bool FileIsExistA(const char* path);

	SAPPHIRE_API bool FileIsExistW(const wchar_t* path);

	SAPPHIRE_API bool DirIsExistA(const char* path);

	SAPPHIRE_API bool DirIsExistW(const wchar_t* path);

	SAPPHIRE_API size_t GetFileSizeA(const char* fileName);

	SAPPHIRE_API size_t GetFileSizeW(const wchar_t* fileName);

	std::string GetCurrentProcessName();

	std::string  GetCurrentDirA();

	std::wstring  GetCurrentDirW();


	template <class _Elem>
	class basic_path : public std::basic_string<_Elem>
	{
	public:
		typedef typename std::basic_string<_Elem>	_String;
		typedef typename basic_path<_Elem>			_MyType;

		///////////////////////////////////////////////////////////////////////////////
		// 构造函数
		basic_path()
			: _isFile(true)
		{
		}
		basic_path(const _Elem* p, bool isFilePath = true)
			: _isFile(isFilePath), _String(p)
		{
			//normalize();
		}
		basic_path(const _Elem* p, size_type roff, size_type count, bool isFilePath = true)
			: _isFile(isFilePath), _String(p, roff, count)
		{
			normalize();
		}
		basic_path(const _String& str, bool isFilePath = true)
			: _isFile(isFilePath), _String(str)
		{
			normalize();
		}
		basic_path(const _MyType& path)
			: _isFile(path._isFile), _String(path)
		{

		}
		basic_path(const _MyType& path, bool isFilePath)
			: _isFile(isFilePath), _String(path)
		{

		}
		_MyType& operator+(_Elem _Ch)
		{
			_String::assign(1, _Ch);
			return (*this);
		}

		/*
		~basic_path()
		{
		outputStringA("%s\n", (const char*)(*this));
		}*/
		/*
		operator _String()
		{
		return (*this);
		}*/

		/** 标准化路径:
		* 工序:
		*		1.修剪两边的空格
		*		2.把['/']统一替换成['\']
		*/
		void normalize()
		{
			_Elem slash[2] = { CHAR_SLASH };
			_Elem slash2[2] = { CHAR_SLASH2 };
#ifdef SAPPHIRE_WIN
			replace_all(_String(slash2), _String(slash));
#else
			replace_all(_String(slash), _String(slash2));
#endif
		}

		void replace_all(const _String& substr, const _String& str)
		{
			size_type search_len = substr.length();
			if (search_len == 0)
				return;

			long/*size_type*/ pos = (long)(length() - search_len); // 原来size_type是无符号的
			for (; pos >= 0;)
			{
				if (compare(pos, search_len, substr) == 0)
				{
					replace(pos, search_len, str);
					pos -= (long)search_len;
				}
				else
				{
					--pos;
				}
			}
		}

		_String getFileName() const
		{
			if (!isFile())
				return _String();

			size_type pos = getLastSlashPos();

			return substr(pos + 1, length() - pos - 1);
		}

		_String getFileTitle() const
		{
			if (!isFile())
				return _String();

			std::string path = this->c_str();
			char filename[MAX_PATH] = { 0 };
			_splitpath(path.c_str(), NULL, NULL, filename, NULL);
			if (!filename)
				return _String();

			return filename;

			//size_type slashPos = getLastSlashPos();
			//size_type dotPos = getLastDotPos();
			//return substr(slashPos + 1, dotPos - slashPos - 1);
		}

		_String getFileExt() const
		{
			if (!isFile() || empty())
				return _String();

			std::string path = this->c_str();
			char ext[16] = { 0 };
			_splitpath(path.c_str(), NULL, NULL, NULL, ext);
			if (!ext)
				return _String();

			return ext + 1;

			//size_type pos = getLastDotPos();
			//if (pos == npos) // no ext
			//	return _String();

			//return substr(pos + 1, length() - pos - 1);
		}

		_String getParentDir()
		{
			if (!isFile())
				removeTailSlash();

			size_type pos = getLastSlashPos();
			if (pos == npos)
				return _String();
			return substr(0, pos);
		}

		_MyType& addTailSlash()
		{
#ifdef SAPPHIRE_WIN
			if (*(end() - 1) != CHAR_SLASH)
				*this += CHAR_SLASH;
#else
			if (*(end() - 1) != CHAR_SLASH2)
				*this += CHAR_SLASH2;
#endif
			return (*this);
		}

		_MyType& removeTailSlash()
		{
#ifdef SAPPHIRE_WIN
			if (*(end() - 1) == CHAR_SLASH)
				erase(end() - 1);
#else
			if (*(end() - 1) == CHAR_SLASH2)
				erase(end() - 1);
#endif
			return (*this);
		}

		void isFile(bool isfile)
		{
			_isFile = isfile;
		}
		bool isFile() const
		{
			return _isFile;
		}
	protected:
		inline size_type getLastDotPos() const
		{
			return find_last_of(CHAR_DOT);
		}
		inline size_type getLastSlashPos() const
		{
#ifdef SAPPHIRE_WIN
			return find_last_of(CHAR_SLASH);
#else
			return find_last_of(CHAR_SLASH2);
#endif
		}
		inline size_type getLastSlashPos(int count) const
		{
			size_type pos;
#ifdef SAPPHIRE_WIN
			pos = find_last_of(CHAR_SLASH);
			while (pos != npos && --count > 0)
				pos = find_last_of(CHAR_SLASH, pos - 1);
			return pos;
#else
			pos = find_last_of(CHAR_SLASH2);
			while (pos != npos && --count > 0)
				pos = find_last_of(CHAR_SLASH2, pos - 1);
			return pos;
#endif
		}
	private:
		bool	_isFile;
	};

	typedef basic_path<char>			PathA, CPathA, Path, CPath;
	typedef basic_path<wchar_t>			PathW, CPathW;

}




#endif // ! __PATH__
