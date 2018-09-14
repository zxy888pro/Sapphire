#pragma once
#include <SapphireDef.h>
#include <string>
namespace Sapphire
{
	template<class _Elem,
	class _Traits,
	class _Alloc>
	class SAPPHIRE_CLASS _String : public std::basic_string < _Elem, _Traits, _Alloc >
	{
	public:

		_String();

		_String(const _Elem*  pstr);

		_String(const std::basic_string< _Elem, _Traits, _Alloc > str);

		unsigned ToHash() const
		{
			unsigned hash = 0;
			const char* ptr = std::basic_string < _Elem, _Traits, _Alloc >::data();
			while (*ptr)
			{
				hash = *ptr + (hash << 6) + (hash << 16) - hash;
				++ptr;
			}

			return hash;
		}

		static int Compare(const char* lhs, const char* rhs, bool caseSensitive)
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

		int Compare(const char* str, bool caseSensitive) const
		{
			return Compare(c_str(), str, caseSensitive);
		}

		static std::vector<String> Split(const char* str, char separator, bool keepEmptyStrings)
		{
			std::vector<String> ret;
			const char* strEnd = str + String::CStringLength(str);

			for (const char* splitEnd = str; splitEnd != strEnd; ++splitEnd)
			{
				if (*splitEnd == separator)
				{
					const ptrdiff_t splitLen = splitEnd - str;
					if (splitLen > 0 || keepEmptyStrings)
						ret.Push(String(str, splitLen));
					str = splitEnd + 1;
				}
			}

			const ptrdiff_t splitLen = strEnd - str;
			if (splitLen > 0 || keepEmptyStrings)
				ret.Push(String(str, splitLen));

			return ret;
		}

	};

	template<class _Elem,
	class _Traits,
	class _Alloc>
		Sapphire::_String<_Elem, _Traits, _Alloc>::_String(const std::basic_string< _Elem, _Traits, _Alloc > str)
	{
		:std::basic_string< _Elem, _Traits, _Alloc >(str);
	}

	template<class _Elem,
	class _Traits,
	class _Alloc>
		Sapphire::_String<_Elem, _Traits, _Alloc>::_String(const _Elem* pstr)
	{
		_Tidy();
		assign(pstr);
	}

	template<class _Elem,
	class _Traits,
	class _Alloc>
		Sapphire::_String<_Elem, _Traits, _Alloc>::_String()
	{

	}

	typedef _String<char, std::char_traits<char>, std::allocator<char> > String;
	typedef _String<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> >  WString;

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

	String ToString() const;

	unsigned ToHash() const { return value_; }

	static unsigned Calculate(const char* str);

	static const StringHash ZERO;

	private:

	uint value_;

	};

	
	
	 
}