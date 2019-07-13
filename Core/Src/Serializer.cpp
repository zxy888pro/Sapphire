#include "Serializer.h"
#include "IStream.h"

namespace Sapphire
{

	 
	static const float q = 32767.0f;

	Serializer::~Serializer()
	{

	}

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

	bool Serializer::WriteIntRect(const IntRect& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteIntVector2(const IntVector2& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteRect(const Rect& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteVector2(const Vector2& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteVector3(const Vector3& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}


	bool Serializer::WritePackedVector3(const Vector3& value, float maxAbsCoord)
	{
		short coords[3];
		float v = 32767.0f / maxAbsCoord;

		coords[0] = (short)(MathHelper::Clamp(value.x_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
		coords[1] = (short)(MathHelper::Clamp(value.y_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
		coords[2] = (short)(MathHelper::Clamp(value.z_, -maxAbsCoord, maxAbsCoord) * v + 0.5f);
		return Write(&coords[0], sizeof coords) == sizeof coords;
	}

	bool Serializer::WriteVector4(const Vector4& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}


	bool Serializer::WriteQuaternion(const Quaternion& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WritePackedQuaternion(const Quaternion& value)
	{
		short coords[4];
		Quaternion norm = value.Normalized();

		coords[0] = (short)(MathHelper::Clamp(norm.w_, -1.0f, 1.0f) * q + 0.5f);
		coords[1] = (short)(MathHelper::Clamp(norm.x_, -1.0f, 1.0f) * q + 0.5f);
		coords[2] = (short)(MathHelper::Clamp(norm.y_, -1.0f, 1.0f) * q + 0.5f);
		coords[3] = (short)(MathHelper::Clamp(norm.z_, -1.0f, 1.0f) * q + 0.5f);
		return Write(&coords[0], sizeof coords) == sizeof coords;
	}

	bool Serializer::WriteMatrix3x3(const Matrix3x3& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteMatrix3x4(const Matrix3x4& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteMatrix4x4(const Matrix4x4& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteColor(const Color& value)
	{
		return Write(value.Data(), sizeof value) == sizeof value;
	}

	bool Serializer::WriteBoundingBox(const BoundingBox& value)
	{
		bool success = true;
		success &= WriteVector3(value.min_);
		success &= WriteVector3(value.max_);
		return success;
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

	bool Serializer::WriteFileID(const std::string& value)
	{
		bool success = true;
		unsigned length = (unsigned)MIN((int)value.length(), 4);

		success &= Write(value.c_str(), length) == length;
		for (unsigned i = value.length(); i < 4; ++i)
			success &= WriteByte(' ');
		return success;
	}

	bool Serializer::WriteStringHash(const StringHash& value)
	{
		return WriteUint(value.Value());
	}

	bool Serializer::WriteBuffer(const std::vector<byte>& value)
	{
		bool success = true;
		unsigned size = value.size();

		success &= WriteVLE(size);
		if (size)
			success &= Write(&value[0], size) == size;
		return success;
	}

	bool Serializer::WriteResourceRef(const ResourceRef& value)
	{
		bool success = true;
		success &= WriteStringHash(value.type_);
		success &= WriteString(value.name_.c_str());
		return success;
	}

	bool Serializer::WriteResourceRefList(const ResourceRefList& value)
	{
		bool success = true;

		success &= WriteStringHash(value.type_);
		success &= WriteVLE(value.names_.size());
		for (unsigned i = 0; i < value.names_.size(); ++i)
			success &= WriteString(value.names_[i].c_str());

		return success;
	}

	bool Serializer::WriteVariant(const Variant& value)
	{
		bool success = true;
		VariantType type = value.GetType();

		success &= WriteUByte((unsigned char)type);
		success &= WriteVariantData(value);
		return success;
	}

	bool Serializer::WriteVariantData(const Variant& value)
	{
		switch (value.GetType())
		{
		case VAR_NONE:
			return true;

		case VAR_INT:
			return WriteInt(value.GetInt());

		case VAR_BOOL:
			return WriteBool(value.GetBool());

		case VAR_FLOAT:
			return WriteFloat(value.GetFloat());

		case VAR_VECTOR2:
			return WriteVector2(value.GetVector2());

		case VAR_VECTOR3:
			return WriteVector3(value.GetVector3());

		case VAR_VECTOR4:
			return WriteVector4(value.GetVector4());

		case VAR_QUATERNION:
			return WriteQuaternion(value.GetQuaternion());

		case VAR_COLOR:
			return WriteColor(value.GetColor());

		case VAR_STRING:
			return WriteString(value.GetString().c_str());

		case VAR_BUFFER:
			return WriteBuffer(value.GetBuffer());

			// Serializing pointers is not supported. Write null
		case VAR_VOIDPTR:
		case VAR_PTR:
			return WriteUint(0);

		case VAR_RESOURCEREF:
			return WriteResourceRef(value.GetResourceRef());

		case VAR_RESOURCEREFLIST:
			return WriteResourceRefList(value.GetResourceRefList());

		case VAR_VARIANTVECTOR:
			return WriteVariantVector(value.GetVariantVector());

		case VAR_STRINGVECTOR:
			return WriteStringVector(value.GetStringVector());

		case VAR_VARIANTMAP:
			return WriteVariantMap(value.GetVariantMap());

		case VAR_INTRECT:
			return WriteIntRect(value.GetIntRect());

		case VAR_INTVECTOR2:
			return WriteIntVector2(value.GetIntVector2());

		case VAR_MATRIX3X3:
			return WriteMatrix3x3(value.GetMatrix3x3());

		case VAR_MATRIX3X4:
			return WriteMatrix3x4(value.GetMatrix3x4());

		case VAR_MATRIX4X4:
			return WriteMatrix4x4(value.GetMatrix4());

		case VAR_DOUBLE:
			return WriteDouble(value.GetDouble());

		default:
			return false;
		}
	}

	bool Serializer::WriteVariantVector(const VariantVector& value)
	{
		bool success = true;
		success &= WriteVLE(value.size());
		for (VariantVector::const_iterator i = value.begin(); i != value.end(); ++i)
			success &= WriteVariant(*i);
		return success;
	}

	bool Serializer::WriteStringVector(const StringVector& value)
	{
		bool success = true;
		success &= WriteVLE(value.size());
		for (StringVector::const_iterator i = value.begin(); i != value.end(); ++i)
			success &= WriteString((*i).c_str());
		return success;
	}

	bool Serializer::WriteVariantMap(const VariantMap& value)
	{
		bool success = true;
		success &= WriteVLE(value.size());
		for (VariantMap::const_iterator i = value.begin(); i != value.end(); ++i)
		{
			WriteString(i->first.c_str());
			WriteVariant(i->second);
		}
		return success;
	}

	bool Serializer::WriteVLE(unsigned value)
	{
		unsigned char data[4];
		//VLE变长整型采用1个标志位和7个数据位的方式来对一个正整型数值进行压缩,标志位的作用是表明除7位数据位外余下数据位的值是否为零
		//具体计算方式为：将数值与~0x7f进行与运算，如果数值为0表明该数值的有效数据位为7位，否则数值为1表明仍然有有效数据将数值>>>7位后继续前面的步骤，直至为0。
		//把一个uint拆分为4个7位
		/*如127 二进制表示为0b01111111，编码为 0b 0_1111111,
		128二进制表示为0b10000000，编码为 0b 00000001_10000000,
		16384二进制表示为 0b01000000_00000000, 编码为0b 00000001_10000000_10000000
		这里可以看到128采用变长整型编码后占2个字节，如果采用无符号整型编码只占1个字节，说明变长整型编码在某些情况下反而占用更多的空间，以下是变长编码占用字节的详细情况：
		由于负数的符号位是1，那么采用该编码后是不能达到压缩数据的效果的，改进的方法之一是采用zigzag编码
		*/
		if (value < 0x80)   //< 2^7
			return WriteUByte((unsigned char)value);
		else if (value < 0x4000)   //<2^14
		{
			data[0] = (unsigned char)(value | 0x80); //低7位
			data[1] = (unsigned char)(value >> 7);  //高7位
			return Write(data, 2) == 2;
		}
		else if (value < 0x200000) //2^21
		{
			data[0] = (unsigned char)(value | 0x80);
			data[1] = (unsigned char)((value >> 7) | 0x80);
			data[2] = (unsigned char)(value >> 14);
			return Write(data, 3) == 3;
		}
		else
		{				//2^28
			data[0] = (unsigned char)(value | 0x80);
			data[1] = (unsigned char)((value >> 7) | 0x80);
			data[2] = (unsigned char)((value >> 14) | 0x80);
			data[3] = (unsigned char)(value >> 21);
			return Write(data, 4) == 4;
		}
	}

	bool Serializer::WriteNetID(unsigned value)
	{
		return Write(&value, 3) == 3;
	}

	bool Serializer::WriteLine(const std::string& value)
	{
		bool success = true;
		success &= Write(value.c_str(), value.length()) == value.length();
		success &= WriteUByte(13);
		success &= WriteUByte(10);
		return success;
	}
}