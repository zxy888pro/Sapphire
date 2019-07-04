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
		/// Write an IntVector2.
		bool WriteIntVector2(const IntVector2& value);
		/// Write a Rect.
		bool WriteRect(const Rect& value);
		/// Write a Vector2.
		bool WriteVector2(const Vector2& value);
		/// Write a Vector3.
		bool WriteVector3(const Vector3& value);
		/// Write a Vector3 packed into 3 x 16 bits with the specified maximum absolute range.
		bool WritePackedVector3(const Vector3& value, float maxAbsCoord);
		/// Write a Vector4.
		bool WriteVector4(const Vector4& value);
		/// Write a quaternion.
		bool WriteQuaternion(const Quaternion& value);
		/// Write a quaternion with each component packed in 16 bits.
		bool WritePackedQuaternion(const Quaternion& value);
		/// Write a Matrix3.
		bool WriteMatrix3x3(const Matrix3x3& value);
		/// Write a Matrix3x4.
		bool WriteMatrix3x4(const Matrix3x4& value);
		/// Write a Matrix4.
		bool WriteMatrix4x4(const Matrix4x4& value);
		/// Write a color.
		bool WriteColor(const Color& value);
		/// Write a bounding box.
		bool WriteBoundingBox(const BoundingBox& value);
		/// Write a four-letter file ID. If the string is not long enough, spaces will be appended.
		bool WriteFileID(const std::string& value);
		/// Write a 32-bit StringHash.
		bool WriteStringHash(const StringHash& value);
		/// Write a buffer, with size encoded as VLE.
		bool WriteBuffer(const std::vector<byte>& buffer);
		/// Write a resource reference.
		bool WriteResourceRef(const ResourceRef& value);
		/// Write a resource reference list.
		bool WriteResourceRefList(const ResourceRefList& value);
		/// Write a variant.
		bool WriteVariant(const Variant& value);
		/// Write a variant without the type information.
		bool WriteVariantData(const Variant& value);
		/// Write a variant vector.
		bool WriteVariantVector(const VariantVector& value);
		/// Write a variant vector.
		bool WriteStringVector(const StringVector& value);
		/// Write a variant map.
		bool WriteVariantMap(const VariantMap& value);
		/// Write a variable-length encoded unsigned integer, which can use 29 bits maximum.
		bool WriteVLE(unsigned value);
		/// Write a 24-bit network object ID.
		bool WriteNetID(unsigned value);
		/// Write a text line. Char codes 13 & 10 will be automatically appended.
		bool WriteLine(const std::string& value);

	protected:

		 
	};

}