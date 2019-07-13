#include "Sapphire.h"
#include <Str.h>
#include <mathHelper.h>

namespace Sapphire
{
	const String String::EMPTY;
	static const int CONVERSION_BUFFER_LENGTH = 128;
	static const int MATRIX_CONVERSION_BUFFER_LENGTH = 256;

	StringHash::StringHash(const char* str) :value_(Calculate(str))
	{

	}

	StringHash::StringHash(const String& str) : value_(Calculate(str.c_str()))
	{

	}

	StringHash::StringHash(const std::string& str) : value_(Calculate(str.c_str()))
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
		m_wstr = String2WString(m_str);
	}

	String::String(std::string& str)
	{
		m_str = str;
		m_wstr = String2WString(m_str);
	}

	String::String(char c, uint count)
	{
		m_str = std::string(c, count);
		m_wstr = String2WString(m_str);
	}


	String::String(const wchar_t* pstr)
	{
		m_wstr = pstr;
		m_str = WString2String(m_wstr);
	}

	String::String(const String& other)
	{
		m_str = other.str();
		m_wstr = String2WString(m_str);
	}

	String::String(int val)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%d", val);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(short value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%d", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(long value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%ld", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(long long value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%lld", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(unsigned value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%u", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(unsigned short value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%u", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(unsigned long value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%lu", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(unsigned long long value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%llu", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(float value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(double value)
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%.15g", value);
		*this = tempBuffer;
		m_str = tempBuffer;
		m_wstr = String2WString(m_str);
	}

	String::String(bool value)
	{
		if (value)
			*this = "true";
		else
			*this = "false";
		 
	}

	String::String(char value)
	{
		m_str.resize(1);
		m_str[0] = value;
		m_wstr = String2WString(m_str);
	}

	

	

	String::String(const std::string& str)
	{
		m_str = str.c_str();
		m_wstr = String2WString(m_str);
	}

	String::String(const char* pstr, uint length)
	{
		m_str.resize(length);
		memcpy(&m_str[0], pstr, length);
		m_wstr = String2WString(m_str);
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

	 

	String& String::operator=(const String& rhs)
	{
		m_str = rhs.c_str();
		m_wstr = String2WString(m_str);
		return *this;
	}

	String& String::operator=(std::string& rhs)
	{
		m_str = rhs.c_str();
		m_wstr = String2WString(m_str);
		return *this;
	}

	String& String::operator=(const char* rhs)
	{
		m_str = rhs;
		m_wstr = String2WString(m_str);
		return *this;
	}

	String& String::operator+=(const String& rhs)
	{
		m_str += rhs.str();
		m_wstr = String2WString(m_str);
		return *this;
	}

	String& String::operator+=(const char* rhs)
	{
		m_str += rhs;
		m_wstr = String2WString(m_str);
		return *this;
	}

	String& String::operator+=(char rhs)
	{
		m_str += rhs;
		m_wstr = String2WString(m_str);
		return *this;
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

	int String::Find(const char* pstr, int beginPos, bool reverse /*= false*/, bool caseSensitive)
	{
		int ret = 0;
		std::string source = m_str.c_str();
		if (!caseSensitive)
		{
			source = String(pstr).ToLower().str();
		}
		if (!reverse)
		{
			ret = source.find(pstr, beginPos);
		}
		else
		{
			ret = source.rfind(pstr, beginPos);
		}
		
		return ret;
	}

	int String::Find(String rhs, int beginPos,  bool reverse /*= false*/, bool caseSensitive /*= true*/)
	{
		int ret = 0;
		std::string source = m_str.c_str();
		if (!caseSensitive)
		{
			source = rhs.ToLower().str();
		}
		if (!reverse)
		{
			ret = source.find(rhs.str(), beginPos);
		}
		else
		{
			ret = source.rfind(rhs.str(), beginPos);
		}

		return ret;
	}

	int String::ReplaceSubString(const String& replaceThis, const String& replaceWith, bool caseSensitive /*= true*/,  int repNum)
	{
		unsigned nextPos = 0;
		int ret = 0;
		std::string source = "";
		std::string src = "";
		std::string dst = "";
		if (caseSensitive)
		{
			source = m_str.c_str();
			src = replaceThis.c_str();
			dst = replaceWith.c_str();
		}
		else
		{
			source = ToLower().c_str();
			src = replaceThis.ToLower().c_str();
			dst = replaceWith.c_str();
		}
		  

		while (nextPos < m_str.length())
		{
			unsigned pos = source.find(src, nextPos);
			if (pos == std::string::npos)
				break;
			source.replace(pos, src.length(), dst);
			nextPos = pos + dst.length();
			ret++;
			if (ret > repNum && repNum > 0)
				break;
		}
		(*this) = source;
		return ret;
	}

	int String::ReplaceSubString(const char* replaceThis, const char* replaceWith, bool caseSensitive /*= true*/, int repNum)
	{
		unsigned nextPos = 0;
		int ret = 0;
		std::string source = "";
		std::string src = "";
		std::string dst = "";
		if (caseSensitive)
		{
			source = m_str.c_str();
			src = replaceThis;
			dst = replaceWith;
		}
		else
		{
			source = ToLower().c_str();
			src = String(replaceThis).ToLower().c_str();
			dst = replaceWith;
		}


		while (nextPos < m_str.length())
		{
			unsigned pos = source.find(src, nextPos);
			if (pos == std::string::npos)
				break;
			source.replace(pos, src.length(), dst);
			nextPos = pos + dst.length();
			ret++;
			if (ret > repNum && repNum > 0)
				break;
		}
		(*this) = source;
		return ret;
	}



	

	Sapphire::String String::ToUpper() const
	{
		String ret(*this);
		for (unsigned i = 0; i < ret.Length(); ++i)
			ret[i] = (char)toupper(m_str[i]);

		return ret;
	}

	Sapphire::String String::ToLower() const
	{
		String ret(*this);
		for (unsigned i = 0; i < ret.Length(); ++i)
			ret[i] = (char)tolower(m_str[i]);

		return ret;
	}

	int String::Length() const
	{
		return m_str.length();
	}

	String String::Trimmed() const
	{
		unsigned trimStart = 0;
		unsigned trimEnd = m_str.length();

		while (trimStart < trimEnd)
		{
			char c = m_str[trimStart];
			if (c != ' ' && c != 9)
				break;
			++trimStart;
		}
		while (trimEnd > trimStart)
		{
			char c = m_str[trimEnd - 1];
			if (c != ' ' && c != 9)
				break;
			--trimEnd;
		}

		return GetSubString(trimStart, trimEnd - trimStart);
	}

	int String::Capacity() const
	{
		return m_str.capacity();
	}

	void String::Clear()
	{
		m_str.clear();
		m_wstr.clear();

	}

	void String::Reserve(unsigned newCapacity)
	{
		m_str.reserve(newCapacity);
		m_wstr = String2WString(m_str);
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

	std::vector<Sapphire::String> String::Split(char separator, bool keepEmptyStrings /*= false*/) const
	{
		return Split(str().c_str(), separator, keepEmptyStrings);
	}

	Sapphire::String String::GetSubString(int nBeginPos, int nEndPos) const
	{
		if (nBeginPos == std::string::npos || nEndPos == std::string::npos || nEndPos > m_str.length() || nBeginPos > nEndPos)
			throw StringException(StringException::SError_OutOfRange);

		String ret = m_str.substr(nBeginPos, nEndPos - nBeginPos);
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