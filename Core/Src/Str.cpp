#include "Sapphire.h"
#include <Str.h>
#include <mathHelper.h>

namespace Sapphire
{

	StringHash::StringHash(const char* str) :value_(Calculate(str))
	{

	}

	StringHash::StringHash(const String& str) : value_(Calculate(str.c_str()))
	{

	}

	Sapphire::String StringHash::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%08X", value_);
		return String(tempBuffer);
	}

	unsigned StringHash::Calculate(const char* str)
	{
		unsigned hash = 0;

		if (!str)
			return hash;

		while (*str)
		{
			// 不區分大小寫
			char c = *str;
			hash = MathHelper::SDBMHash(hash, (unsigned char)tolower(c));
			++str;
		}

		return hash;
	}

	const Sapphire::StringHash StringHash::ZERO;

	String::String()
	{

	}

	String::String(const char* pstr)
	{
		m_str = pstr;
	}

	String::String(std::string& str)
	{
		m_str = str;
	}

	String::String(char c, uint count)
	{
		m_str = std::string(c, count);
	}


	String::String(const wchar_t* pstr)
	{
		m_wstr = pstr;
	}

	String::~String()
	{

	}

	const std::string& String::str() const
	{
		return m_str;
	}

	const std::wstring& String::wstr() const
	{
		return m_wstr;
	}

	const char* String::c_str() const
	{
		return m_str.c_str();
	}

	const wchar_t* String::c_wstr() const
	{
		return m_wstr.c_str();
	}

	 

	unsigned String::ToHash() const
	{
		unsigned hash = 0;
		const char* ptr = m_str.data();
		while (*ptr)
		{
			hash = *ptr + (hash << 6) + (hash << 16) - hash;
			++ptr;
		}

		return hash;
	}

	int String::Compare(const char* lhs, const char* rhs, bool caseSensitive)
	{
		if (!lhs || !rhs)
			return lhs ? 1 : (rhs ? -1 : 0);

		if (caseSensitive)
			return strcmp(lhs, rhs);
		else
		{
			for (;;)
			{
				char l = (char)tolower(*lhs);
				char r = (char)tolower(*rhs);
				if (!l || !r)
					return l ? 1 : (r ? -1 : 0);
				if (l < r)
					return -1;
				if (l > r)
					return 1;

				++lhs;
				++rhs;
			}
		}
	}

	int String::Compare(const char* str, bool caseSensitive) const
	{
		return Compare(m_str.c_str(), str, caseSensitive);
	}

	int String::Compare(String& rhs)
	{
		return Compare(m_str.c_str(), rhs.c_str());
	}

	int String::Find(const char* pstr, int beginPos, int strNo /*= 0*/, bool reverse /*= false*/)
	{
		int ret = 0;
		return ret;
	}

	int String::Find(String rhs, int beginPos, int strNo /*= 0*/, bool reverse /*= false*/, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		return ret;
	}

	int String::Replace(const String& replaceThis, const String& replaceWith, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		return ret;
	}

	int String::Replace(const char* replaceThis, const char* replaceWith, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		return ret;
	}

	int String::ReplaceAll(const String& replaceThis, const String& replaceWith, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		return ret;
	}

	int String::ReplaceAll(const char* replaceThis, const char* replaceWith, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		return ret;
	}

	Sapphire::String String::GetSubString(int nBeginPos, int nEndPos)
	{
		if(nBeginPos == std::string::npos || nEndPos == std::string::npos || nEndPos > m_str.length() || nBeginPos > nEndPos);
			throw StringException(StringException::SError_OutOfRange);

		String ret = m_str.substr(nBeginPos, nEndPos - nBeginPos);
		return ret;
	}

	std::vector<Sapphire::String> String::Split(const char* str, char separator, bool keepEmptyStrings /*= false*/)
	{
		std::vector<String> ret;
		std::string src = str;
		std::string::size_type pos1, pos2;
		pos2 = src.find(separator);
		pos1 = 0;
		String tmp;
		while (pos2 != std::string::npos)
		{
			tmp = src.substr(pos1, pos2 - pos1);
			ret.push_back(tmp);	
			pos1 = pos2 + 1;
			pos2 = src.find(separator, pos1);
		}
		tmp = src.substr(pos1);
		ret.push_back(tmp);
		return ret;
	}

	FORCEINLINE String operator+(const char* lhs, const String& rhs)
	{
		String ret(lhs);
		ret += rhs;
		return ret;
	}

	FORCEINLINE String operator+(const wchar_t* lhs, const String& rhs)
	{
		String ret(lhs);
		ret += rhs;
		return ret;
	}

}