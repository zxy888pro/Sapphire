#include "Serializer.h"
#include "IStream.h"

namespace Sapphire
{

	 


	bool Serializer::WriteInt(int value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteShort(short value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteByte(signed char value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteUint(unsigned value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteUShort(unsigned short value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteUByte(unsigned char value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteBool(bool value)
	{
		return WriteUByte((unsigned char)(value ? 1 : 0)) == 1;
	}

	bool Serializer::WriteFloat(float value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteDouble(double value)
	{
		return Write(&value, sizeof value) == sizeof value;
	}

	bool Serializer::WriteString(const char* str)
	{
		int len = strlen(str);
		if (len)
		{
			return Write(str, len + 1) == (len + 1);
		}
		return false;
	}

}