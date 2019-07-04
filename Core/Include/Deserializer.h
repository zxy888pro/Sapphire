#pragma once
#include "Sapphire.h"
#include "Math/BoundingBox.h"
#include "Variant.h"

namespace Sapphire
{
	class Deserializer
	{
	public:
		
		Deserializer();
		Deserializer(unsigned size);

		virtual ~Deserializer();

		virtual const std::string& GetName() const;

		virtual unsigned Read(void* dest, unsigned size) = 0;

		virtual unsigned Seek(unsigned position) = 0;

		UINT32 GetPosition() const { return m_uPosition; }

		UINT32 GetSize() const { return m_uSize; }

		virtual unsigned GetChecksum(){ return 0; };

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

		IntRect ReadIntRect();

		IntVector2 ReadIntVector2();

		Rect ReadRect();

		Vector2 ReadVector2();

		Vector3 ReadVector3();
	
		Vector3 ReadPackedVector3(float maxAbsCoord);

		Vector4 ReadVector4();

		Quaternion ReadQuaternion();

		Quaternion ReadPackedQuaternion();

		Matrix3x3 ReadMatrix3x3();

		Matrix3x4 ReadMatrix3x4();

		Matrix4x4 ReadMatrix4x4();

		Color ReadColor();

		BoundingBox ReadBoundingBox();

		std::string ReadFileID();

		StringHash ReadStringHash();

		std::vector<byte> ReadBuffer();

		ResourceRef ReadResourceRef();

		ResourceRefList ReadResourceRefList();
		
		Variant ReadVariant();
		
		Variant ReadVariant(VariantType type);
		
		VariantVector ReadVariantVector();
		
		StringVector ReadStringVector();
		
		VariantMap ReadVariantMap();
		
		unsigned ReadVLE();
		
		unsigned ReadNetID();
		
		String ReadLine();
		std::string ReadString();

	protected:
		 
		UINT32 m_uPosition;
		UINT32 m_uSize;

	};
}
