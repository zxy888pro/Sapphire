#pragma once
#include "Sapphire.h"

namespace Sapphire
{
	class Deserializer
	{
	public:
		Deserializer(IStream* pstream);
		virtual ~Deserializer();

		bool ReadInt(int value);
		bool ReadShort(short value);
		bool ReadByte(signed char value);
		bool ReadUint(unsigned value);
		bool ReadUShort(unsigned short value);
		bool ReadUByte(unsigned char value);
		bool ReadBool(bool value);
		bool ReadFloat(float value);
		bool ReadDouble(double value);
		bool ReadString(const char* str);

	protected:

		IStream*  m_pStream;

	};
}
