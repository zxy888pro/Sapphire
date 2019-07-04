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
		//д��һ��ÿ�����������16λ����Ԫ��
		bool WritePackedQuaternion(const Quaternion& value);
		
		bool WriteMatrix3x3(const Matrix3x3& value);

		bool WriteMatrix3x4(const Matrix3x4& value);

		bool WriteMatrix4x4(const Matrix4x4& value);

		bool WriteColor(const Color& value);

		bool WriteBoundingBox(const BoundingBox& value);
		/// д��һ��4��ĸ���ļ�ID������ַ������Ȳ�����׷�ӿո�
		
		bool WriteFileID(const std::string& value);
		
		bool WriteStringHash(const StringHash& value);
		/// д��һ��Buffer��buffer��С��VLE����
		
		bool WriteBuffer(const std::vector<byte>& buffer);
		
		bool WriteResourceRef(const ResourceRef& value);
		
		bool WriteResourceRefList(const ResourceRefList& value);

		bool WriteVariant(const Variant& value);

		bool WriteVariantData(const Variant& value);

		bool WriteVariantVector(const VariantVector& value);

		bool WriteStringVector(const StringVector& value);

		bool WriteVariantMap(const VariantMap& value);
		/// д��һ�����뵽�29λ�Ŀɱ䳤�޷������� 1��־λ 7����λ
		bool WriteVLE(unsigned value);
		/// д��һ��24λ���������ID
		bool WriteNetID(unsigned value);
		/// д��һ���ַ������Զ�׷���ַ�����13��10
		bool WriteLine(const std::string& value);

	protected:

		 
	};

}