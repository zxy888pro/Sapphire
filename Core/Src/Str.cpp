#include "Sapphire.h"
#include <Str.h>
#include <mathHelper.h>

namespace Sapphire
{

	StringHash::StringHash(const char* str) :value_(Calculate(str))
	{

	}

	StringHash::StringHash(const String& str):value_(Calculate(str.c_str()))
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

}