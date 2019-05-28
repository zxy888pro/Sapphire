#pragma once
#include "Sapphire.h"


namespace Sapphire
{

	class Serializer
	{
	public: 
		 
		virtual ~Serializer();

		virtual unsigned Write(const void* pData, unsigned size) = 0;

		bool WriteInt(int value);
		bool WriteShort(short value);
		bool WriteByte(signed char value);
		bool WriteUint(unsigned value);
		bool WriteUShort(unsigned short value);
		bool WriteUByte(unsigned char value);
		bool WriteBool(bool value);
		bool WriteFloat(float value);
		bool WriteDouble(double value);
		bool WriteString(const char* str);


	protected:

		 
	};

}