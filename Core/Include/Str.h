#pragma once
#include <SapphireDef.h>
#include <string>

namespace Sapphire
{
	class SAPPHIRE_CLASS String
	{
	public:

		static const String EMPTY;

		String();

		String(const char* pstr);

		String(const wchar_t* pstr);

		String(std::string& str);

		String(const std::string& str);

		String(char c, uint count);

		String(const String& other);

		explicit String(int val);

		explicit String(short value);

		explicit String(long value);

		explicit String(long long value);

		explicit String(unsigned value);

		explicit String(unsigned short value);

		explicit String(unsigned long value);

		explicit String(unsigned long long value);

		explicit String(float value);

		explicit String(double value);

		explicit String(bool value);

		explicit String(char value);



		virtual ~String();

		const std::string& str() const;

		const std::wstring& wstr() const;

		const char* c_str() const;

		const wchar_t*  c_wstr() const;

		bool empty() const
		{
			return m_str.empty();
		}

		String& operator =(const char* rhs)
		{

			m_str = rhs;
			return *this;
		}

		String& operator =(std::string& rhs)
		{

			m_str = rhs.c_str();
			return *this;
		}


		String& operator =(const String& rhs)
		{
			m_str = rhs.c_str();
			return *this;
		}

		String& operator +=(const String& rhs)
		{
			m_str += rhs.str();
			return *this;
		}

		String& operator +=(const char* rhs)
		{
			m_str += rhs;
			return *this;
		}

		String& operator +=(char rhs)
		{
			m_str += rhs;
			return *this;
		}

		bool operator ==(const String& rhs) const { return m_str == rhs.str(); }

		bool operator !=(const String& rhs) const { return m_str != rhs.str(); }

		char& operator [](unsigned index)
		{
			m_assert(index < m_str.length());
			return m_str[index];
		}

		const char& operator [](unsigned index) const
		{
			m_assert(index < m_str.length());
			return m_str[index];
		}

		char& At(unsigned index)
		{
			m_assert(index < m_str.length());
			return m_str[index];
		}

		const char& At(unsigned index) const
		{
			m_assert(index < m_str.length());
			return m_str[index];
		}

		unsigned ToHash() const;

		int Compare(const char* str, bool caseSensitive) const;

		int Compare(String& rhs);

		int Find(const char* pstr, int beginPos, int strNo = 0, bool reverse = false);

		int Find(String rhs, int beginPos, int strNo = 0, bool reverse = false, bool caseSensitive = true);

		int Replace(const String& replaceThis, const String& replaceWith, bool caseSensitive = true);

		int Replace(const char* replaceThis, const char* replaceWith, bool caseSensitive = true);

		int ReplaceAll(const String& replaceThis, const String& replaceWith, bool caseSensitive = true);

		int ReplaceAll(const char* replaceThis, const char* replaceWith, bool caseSensitive = true);

		std::vector<String> Split(char separator, bool keepEmptyStrings = false) const;

		String GetSubString(int nBeginPos, int nEndPos) const;

		String ToUpper() const;

		String ToLower() const;

		int	   Length() const;

		String Trimmed() const;

		int    Capacity() const;

		void   Clear();

		void   Reserve(unsigned newCapacity);

		static int Compare(const char* lhs, const char* rhs, bool caseSensitive);

		

		static std::vector<String> Split(const char* str, char separator, bool keepEmptyStrings = false);


	private:

		std::string m_str;
		std::wstring m_wstr;

	};

	FORCEINLINE String operator +(const char* lhs, const String& rhs);



	FORCEINLINE String operator +(const wchar_t* lhs, const String& rhs);



	class SAPPHIRE_CLASS StringHash
	{
	public:

		StringHash() :
			value_(0)
		{
		}

		StringHash(const StringHash& rhs) :
			value_(rhs.value_)
		{
		}
		explicit StringHash(unsigned value) :
			value_(value)
		{
		}

		StringHash(const char* str);

		StringHash(const std::string& str);

		StringHash(const String& str);

		StringHash& operator =(const StringHash& rhs)
		{
			value_ = rhs.value_;
			return *this;
		}

		StringHash operator +(const StringHash& rhs) const
		{
			StringHash ret;
			ret.value_ = value_ + rhs.value_;
			return ret;
		}

		StringHash& operator +=(const StringHash& rhs)
		{
			value_ += rhs.value_;
			return *this;
		}



		bool operator ==(const StringHash& rhs) const { return value_ == rhs.value_; }

		bool operator !=(const StringHash& rhs) const { return value_ != rhs.value_; }

		bool operator <(const StringHash& rhs) const { return value_ < rhs.value_; }

		bool operator >(const StringHash& rhs) const { return value_ > rhs.value_; }

		operator bool() const { return value_ != 0; }

		unsigned Value() const { return value_; }

		String  ToString() const;

		unsigned ToHash() const { return value_; }

		static unsigned Calculate(const char* str);

		static const StringHash ZERO;

	private:

		uint value_;

	};

	struct SAPPHIRE_CLASS StringHashFunc
	{
		size_t operator()(const StringHash &strhash) const
		{
			return strhash.Value();
		}
		
 
	};
	struct SAPPHIRE_CLASS StringHashCMP  
	{
		bool operator()(const StringHash &strhash1, const StringHash &strhash2) const
		{
			return strhash1 == strhash2;
		}
	};

}