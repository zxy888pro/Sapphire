#include "Serializer.h"
#include "IStream.h"

namespace Sapphire
{

	Serializer::Serializer(IStream* pstream) :m_pStream(pstream)
	{

	}

	Serializer::~Serializer()
	{

	}

	unsigned Serializer::Write(const void* pData, unsigned size)
	{
		if (!m_pStream && !m_pStream->IsOpen())
		{
			SAPPHIRE_LOGERROR("Serializer::Write Error! stream is not open!");
			return;
		}
	}

	bool Serializer::WriteInt(int value)
	{

	}

	bool Serializer::WriteShort(short value)
	{

	}

	bool Serializer::WriteByte(signed char value)
	{

	}

	bool Serializer::WriteUint(unsigned value)
	{

	}

	bool Serializer::WriteUShort(unsigned short value)
	{

	}

	bool Serializer::WriteUByte(unsigned char value)
	{

	}

	bool Serializer::WriteBool(bool value)
	{

	}

	bool Serializer::WriteFloat(float value)
	{

	}

	bool Serializer::WriteDouble(double value)
	{

	}

	bool Serializer::WriteString(const char* str)
	{

	}

}