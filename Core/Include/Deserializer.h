#pragma once
#include "Sapphire.h"

namespace Sapphire
{
	class Deserializer
	{
	public:
		
		Deserializer();
		Deserializer(unsigned size);

		virtual ~Deserializer();

		virtual unsigned Read(void* dest, unsigned size) = 0;

		virtual unsigned Seek(unsigned position) = 0;

		UINT32 GetPosition() const { return m_uPosition; }

		UINT32 GetSize() const { return m_uSize; }

		virtual unsigned GetChecksum(){};

		bool IsEnd() const { return m_uPosition >= m_uSize; }

		int ReadInt();
		short ReadShort();
		byte ReadByte();
		UINT32 ReadUint();
		ushort ReadUShort();
		ubyte ReadUByte();
		bool ReadBool();
		float ReadFloat();
		double ReadDouble();
		std::string ReadString();

	protected:
		 
		UINT32 m_uPosition;
		UINT32 m_uSize;

	};
}
