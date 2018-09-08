#pragma once
#include <SapphireDef.h>
#include <Str.h>

namespace Sapphire
{
	/// ��������
	enum VariantType
	{
		VAR_NONE = 0,
		VAR_INT,
		VAR_BOOL,
		VAR_FLOAT,
		VAR_DOUBLE,
		VAR_STRING,
		VAR_VECTOR2,
		VAR_VECTOR3,
		VAR_VECTOR4,
		VAR_QUATERNION,
		VAR_COLOR,
		VAR_VOIDPTR,
		VAR_PTR,
		VAR_MATRIX3X3,
		VAR_MATRIX3X4,
		VAR_MATRIX4X4,
		MAX_VAR_TYPES
	};

	/// Union �ṹ
	// �����POD������String����ѧ���󣨾��󣩣��������Գ���16�ֽڡ����󳬹�16�ֽ�ͨ����ָ��_ptr������
	struct VariantValue
	{
		union
		{
			int int_;
			bool bool_;
			float float_;
			void* ptr_;
		};

		union
		{
			int int2_;
			float float2_;
			void* ptr2_;
		};

		union
		{
			int int3_;
			float float3_;
			void* ptr3_;
		};

		union
		{
			int int4_;
			float float4_;
			void* ptr4_;
		};
	};

	/// ��Դ����
	struct SAPPHIRE_CLASS ResourceRef
	{
		ResourceRef()
		{
		}

		ResourceRef(StringHash type);

		ResourceRef(StringHash type, const String& name);
		
		ResourceRef(const ResourceRef& rhs);

		///��������
		StringHash type_;
		/// ������
		String name_;


		bool operator ==(const ResourceRef& rhs) const;


		bool operator !=(const ResourceRef& rhs) const;
	};

	

	class Variant;

	/// ��������
	typedef std::vector<Variant> VariantVector;

	/// String����
	typedef std::vector<std::string> StringVector;

	/// ����Map  Key�ַ���Hash��   Value:׃�w
	typedef std::unordered_map<StringHash, Variant> VariantMap;


	//��Դ���������б�
	struct SAPPHIRE_API ResourceRefList
	{
		ResourceRefList()
		{
		}

		ResourceRefList(StringHash type);

		ResourceRefList(StringHash type, const StringVector& names);



		StringHash type_;
		/// �������б�
		StringVector names_;

		bool operator ==(const ResourceRefList& rhs) const;

		bool operator !=(const ResourceRefList& rhs) const;
	};

	// ֧��һ���̶����͵ļ��ϵı�������
	class SAPPHIRE_CLASS Variant
	{
		Variant() :
			type_(VAR_NONE)
		{
		}

		/// ��integer����
		Variant(int value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		/// ��unsigned integer.����
		Variant(unsigned value) :
			type_(VAR_NONE)
		{
			*this = (int)value;
		}
		/// ��bool���� 
		Variant(bool value)
			:type_(VAR_NONE)
		{
			*this = value;
		}
		///  ��float���� 
		Variant(float value)
			:type_(VAR_NONE)
		{
			*this = value;
		}
		///  ��double���� 
		Variant(double value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

	private:

		void SetType(VariantType newType);

		VariantType type_;
		VariantValue value_;
	};


}