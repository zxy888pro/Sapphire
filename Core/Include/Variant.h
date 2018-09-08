#pragma once
#include <SapphireDef.h>
#include <Str.h>

namespace Sapphire
{
	/// 变体类型
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

	/// Union 结构
	// 保存非POD对象如String和数学对象（矩阵），它不可以超过16字节。对象超过16字节通过堆指针_ptr来保存
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

	/// 资源引用
	struct SAPPHIRE_CLASS ResourceRef
	{
		ResourceRef()
		{
		}

		ResourceRef(StringHash type);

		ResourceRef(StringHash type, const String& name);
		
		ResourceRef(const ResourceRef& rhs);

		///对象类型
		StringHash type_;
		/// 对象名
		String name_;


		bool operator ==(const ResourceRef& rhs) const;


		bool operator !=(const ResourceRef& rhs) const;
	};

	

	class Variant;

	/// 变体数组
	typedef std::vector<Variant> VariantVector;

	/// String数组
	typedef std::vector<std::string> StringVector;

	/// 变体Map  Key字符串Hash，   Value:變體
	typedef std::unordered_map<StringHash, Variant> VariantMap;


	//资源引用类型列表
	struct SAPPHIRE_API ResourceRefList
	{
		ResourceRefList()
		{
		}

		ResourceRefList(StringHash type);

		ResourceRefList(StringHash type, const StringVector& names);



		StringHash type_;
		/// 对象名列表
		StringVector names_;

		bool operator ==(const ResourceRefList& rhs) const;

		bool operator !=(const ResourceRefList& rhs) const;
	};

	// 支持一个固定类型的集合的变体类型
	class SAPPHIRE_CLASS Variant
	{
		Variant() :
			type_(VAR_NONE)
		{
		}

		/// 用integer构造
		Variant(int value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		/// 用unsigned integer.构造
		Variant(unsigned value) :
			type_(VAR_NONE)
		{
			*this = (int)value;
		}
		/// 用bool构造 
		Variant(bool value)
			:type_(VAR_NONE)
		{
			*this = value;
		}
		///  用float构造 
		Variant(float value)
			:type_(VAR_NONE)
		{
			*this = value;
		}
		///  用double构造 
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