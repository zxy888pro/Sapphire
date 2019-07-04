#pragma once
#include "Sapphire.h"
#include "Math/BoundingBox.h"
#include "Variant.h"

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
		bool WriteIntRect(const IntRect& value);

		bool WriteIntVector2(const IntVector2& value);

		bool WriteRect(const Rect& value);

		bool WriteVector2(const Vector2& value);

		bool WriteVector3(const Vector3& value);

		bool WritePackedVector3(const Vector3& value, float maxAbsCoord);

		bool WriteVector4(const Vector4& value);

		bool WriteQuaternion(const Quaternion& value);
		//写入一个每个分量打包到16位的四元数
		bool WritePackedQuaternion(const Quaternion& value);
		
		bool WriteMatrix3x3(const Matrix3x3& value);

		bool WriteMatrix3x4(const Matrix3x4& value);

		bool WriteMatrix4x4(const Matrix4x4& value);

		bool WriteColor(const Color& value);

		bool WriteBoundingBox(const BoundingBox& value);
		/// 写入一个4字母的文件ID，如果字符串长度不够，追加空格
		
		bool WriteFileID(const std::string& value);
		
		bool WriteStringHash(const StringHash& value);
		/// 写入一个Buffer，buffer大小用VLE编码
		
		bool WriteBuffer(const std::vector<byte>& buffer);
		
		bool WriteResourceRef(const ResourceRef& value);
		
		bool WriteResourceRefList(const ResourceRefList& value);

		bool WriteVariant(const Variant& value);

		bool WriteVariantData(const Variant& value);

		bool WriteVariantVector(const VariantVector& value);

		bool WriteStringVector(const StringVector& value);

		bool WriteVariantMap(const VariantMap& value);
		/// 写入一个编码到最长29位的可变长无符号整形 1标志位 7数据位
		bool WriteVLE(unsigned value);
		/// 写入一个24位的网络对象ID
		bool WriteNetID(unsigned value);
		/// 写入一行字符串，自动追加字符编码13和10
		bool WriteLine(const std::string& value);

	protected:

		 
	};

}