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
		/// Read an IntRect.
		IntRect ReadIntRect();
		/// Read an IntVector2.
		IntVector2 ReadIntVector2();
		/// Read a Rect.
		Rect ReadRect();
		/// Read a Vector2.
		Vector2 ReadVector2();
		/// Read a Vector3.
		Vector3 ReadVector3();
		/// Read a Vector3 packed into 3 x 16 bits with the specified maximum absolute range.
		Vector3 ReadPackedVector3(float maxAbsCoord);
		/// Read a Vector4.
		Vector4 ReadVector4();
		/// Read a quaternion.
		Quaternion ReadQuaternion();
		/// Read a quaternion with each component packed in 16 bits.
		Quaternion ReadPackedQuaternion();
		/// Read a Matrix3.
		Matrix3x3 ReadMatrix3x3();
		/// Read a Matrix3x4.
		Matrix3x4 ReadMatrix3x4();
		/// Read a Matrix4.
		Matrix4x4 ReadMatrix4x4();
		/// Read a color.
		Color ReadColor();
		/// Read a bounding box.
		BoundingBox ReadBoundingBox();
		/// Read a four-letter file ID.
		std::string ReadFileID();
		/// Read a 32-bit StringHash.
		StringHash ReadStringHash();
		/// Read a buffer with size encoded as VLE.
		std::vector<byte> ReadBuffer();
		/// Read a resource reference.
		ResourceRef ReadResourceRef();
		/// Read a resource reference list.
		ResourceRefList ReadResourceRefList();
		/// Read a variant.
		Variant ReadVariant();
		/// Read a variant whose type is already known.
		Variant ReadVariant(VariantType type);
		/// Read a variant vector.
		VariantVector ReadVariantVector();
		/// Read a string vector.
		StringVector ReadStringVector();
		/// Read a variant map.
		VariantMap ReadVariantMap();
		/// Read a variable-length encoded unsigned integer, which can use 29 bits maximum.
		unsigned ReadVLE();
		/// Read a 24-bit network object ID.
		unsigned ReadNetID();
		/// Read a text line.
		String ReadLine();
		std::string ReadString();

	protected:
		 
		UINT32 m_uPosition;
		UINT32 m_uSize;

	};
}
