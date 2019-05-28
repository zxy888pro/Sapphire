#include "Deserializer.h"

namespace Sapphire
{

	Deserializer::Deserializer():
		m_uPosition(0),
		m_uSize(0)
	{

	}

	Deserializer::Deserializer(unsigned size):
		m_uPosition(0),
		m_uSize(size)
	{

	}

	Deserializer::~Deserializer()
	{

	}

	int Deserializer::ReadInt()
	{
		int ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	short Deserializer::ReadShort()
	{
		short ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	byte Deserializer::ReadByte()
	{
		byte ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	UINT32 Deserializer::ReadUint()
	{
		UINT32 ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	ushort Deserializer::ReadUShort()
	{
		ushort ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	ubyte Deserializer::ReadUByte()
	{
		ubyte ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	bool Deserializer::ReadBool()
	{
		return ReadUByte() != 0;
	}

	float Deserializer::ReadFloat()
	{
		float ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	double Deserializer::ReadDouble()
	{
		double ret;
		Read(&ret, sizeof ret);
		return ret;
	}

	std::string Deserializer::ReadString()
	{
		std::string ret = "";
		while (!IsEnd())
		{
			char c = ReadByte();
			if (c!='\0') // ���������ֽ���ɣ�ÿ���ֽ����λΪ1 ��С��0.
				break;
			else
				ret += c;
		}
		return ret;
	}

}