#pragma once
#include <SapphireDef.h>
#include <RefCounter.h>
#include <Str.h>
#include <Color.h>
#include <Math/Vector2.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Rect.h>
#include <Math/Quaternion.h>
#include <Math/Matrix3x3.h>
#include <Math/Matrix3x4.h>

namespace Sapphire
{
	///// 变体类型
	enum VariantType
	{
		VAR_NONE = 0,
		VAR_INT,
		VAR_BOOL,
		VAR_FLOAT,
		VAR_VECTOR2,
		VAR_VECTOR3,
		VAR_VECTOR4,
		VAR_QUATERNION,
		VAR_COLOR,
		VAR_STRING,
		VAR_BUFFER,
		VAR_VOIDPTR,
		VAR_RESOURCEREF,
		VAR_RESOURCEREFLIST,
		VAR_VARIANTVECTOR,
		VAR_VARIANTMAP,
		VAR_INTRECT,
		VAR_INTVECTOR2,
		VAR_PTR,
		VAR_MATRIX3X3,
		VAR_MATRIX3X4,
		VAR_MATRIX4X4,
		VAR_DOUBLE,
		VAR_STRINGVECTOR,
		MAX_VAR_TYPES
	};

	/// Union 结构
	// 保存非POD对象如String和数学对象（矩阵），它不可以超过16字节。对象超过16字节通过堆指针_ptr来保存
	// POD对象直接复制字节数值来赋值
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
	typedef std::vector<String> StringVector;

	/// 变体Map  Key字符串Hash，   Value:變體
	typedef std::unordered_map<std::string, Variant> VariantMap;



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

	// 支持一个不能超过16字节固定类型的集合的变体类型
	// 超过16字节只能以指针方式保存
	class SAPPHIRE_CLASS Variant
	{
	public:
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
		///  用vector2构造 
		Variant(const Vector2& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		///  用vector3构造 
		Variant(const Vector3& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		///  用vector4构造 
		Variant(const Vector4& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		///  用四元数构造 
		Variant(const Quaternion& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		//
		Variant(const Color& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const char* value) :
			type_(VAR_NONE)
		{
			*this = value;
		}


		Variant(const String& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const std::vector<byte>& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(void* value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const ResourceRef& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const ResourceRefList& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const VariantVector& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const VariantMap& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const StringVector& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const IntRect& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const IntVector2& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(RefCounter* value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const Matrix3x3& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const Matrix3x4& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}

		Variant(const Matrix4x4& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		
		Variant(const String& type, const String& value) :
			type_(VAR_NONE)
		{
			FromString(type, value);
		}
		Variant(VariantType type, const String& value) :
			type_(VAR_NONE)
		{
			FromString(type, value);
		}
		Variant(const char* type, const char* value) :
			type_(VAR_NONE)
		{
			FromString(type, value);
		}
		Variant(VariantType type, const char* value) :
			type_(VAR_NONE)
		{
			FromString(type, value);
		}

		Variant(const Variant& value) :
			type_(VAR_NONE)
		{
			*this = value;
		}
		~Variant()
		{
			SetType(VAR_NONE);
		}

		///重置为空
		void Clear()
		{
			SetType(VAR_NONE);
		}

		///从另一个变体赋值
		Variant& operator =(const Variant& rhs);

		 
		Variant& operator =(int rhs)
		{
			SetType(VAR_INT);
			value_.int_ = rhs;
			return *this;
		}

		Variant& operator =(unsigned rhs)
		{
			SetType(VAR_INT);
			value_.int_ = (int)rhs;
			return *this;
		}

		Variant& operator =(const StringHash& rhs)
		{
			SetType(VAR_INT);
			value_.int_ = (int)rhs.Value();
			return *this;
		}

		Variant& operator =(bool rhs)
		{
			SetType(VAR_BOOL);
			value_.bool_ = rhs;
			return *this;
		}

		Variant& operator =(float rhs)
		{
			SetType(VAR_FLOAT);
			value_.float_ = rhs;
			return *this;
		}

		Variant& operator = (double rhs)
		{
			SetType(VAR_DOUBLE);
			*(reinterpret_cast<double*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const Vector2& rhs)
		{
			SetType(VAR_VECTOR2);
			*(reinterpret_cast<Vector2*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const Vector3& rhs)
		{
			SetType(VAR_VECTOR3);
			*(reinterpret_cast<Vector3*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const Vector4& rhs)
		{
			SetType(VAR_VECTOR4);
			*(reinterpret_cast<Vector4*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const Quaternion& rhs)
		{
			SetType(VAR_QUATERNION);
			*(reinterpret_cast<Quaternion*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const Color& rhs)
		{
			SetType(VAR_COLOR);
			*(reinterpret_cast<Color*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const String& rhs)
		{
			SetType(VAR_STRING);
			*(reinterpret_cast<String*>(value_.ptr_)) = rhs;  //使用ptr, String大小超16字节了
			return *this;
		}

		Variant& operator =(const char* rhs)
		{
			SetType(VAR_STRING);
			*(reinterpret_cast<String*>(value_.ptr_)) = String(rhs);
			return *this;
		}

		//用std::vector<byte>分配一个buffer
		Variant& operator =(const std::vector<byte>& rhs)
		{
			SetType(VAR_BUFFER);
			*(reinterpret_cast<std::vector<byte>*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(void* rhs)
		{
			SetType(VAR_VOIDPTR);
			value_.ptr_ = rhs;
			return *this;
		}

		Variant& operator =(const ResourceRef& rhs)
		{
			SetType(VAR_RESOURCEREF);
			*(reinterpret_cast<ResourceRef*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(const ResourceRefList& rhs)
		{
			SetType(VAR_RESOURCEREFLIST);
			*(reinterpret_cast<ResourceRefList*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(const VariantVector& rhs)
		{
			SetType(VAR_VARIANTVECTOR);
			*(reinterpret_cast<VariantVector*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(const StringVector& rhs)
		{
			SetType(VAR_STRINGVECTOR);
			*(reinterpret_cast<StringVector*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(const VariantMap& rhs)
		{
			SetType(VAR_VARIANTMAP);
			*(reinterpret_cast<VariantMap*>(value_.ptr_)) = rhs;
			return *this;
		}

		Variant& operator =(const IntRect& rhs)
		{
			SetType(VAR_INTRECT);
			*(reinterpret_cast<IntRect*>(&value_)) = rhs;
			return *this;
		}

		Variant& operator =(const IntVector2& rhs)
		{
			SetType(VAR_INTVECTOR2);
			*(reinterpret_cast<IntVector2*>(&value_)) = rhs;
			return *this;
		}

		/// 从一个引用计数指针赋值，这个对象将被保存在一个weakptr中
		Variant& operator =(RefCounter* rhs)
		{
			SetType(VAR_PTR);
			*(reinterpret_cast<WeakPtr<RefCounter>*>(&value_)) = rhs;
			return *this;
		}

	 
		Variant& operator =(const Matrix3x3& rhs)
		{
			SetType(VAR_MATRIX3X3);
			*(reinterpret_cast<Matrix3x3*>(value_.ptr_)) = rhs;
			return *this;
		}

		 
		Variant& operator =(const Matrix3x4& rhs)
		{
			SetType(VAR_MATRIX3X4);
			*(reinterpret_cast<Matrix3x4*>(value_.ptr_)) = rhs;
			return *this;
		}

		 
		Variant& operator =(const Matrix4x4& rhs)
		{
			SetType(VAR_MATRIX4X4);
			*(reinterpret_cast<Matrix4x4*>(value_.ptr_)) = rhs;
			return *this;
		}

	 
		bool operator ==(const Variant& rhs) const;

		 
		bool operator ==(int rhs) const { return type_ == VAR_INT ? value_.int_ == rhs : false; }

		 
		bool operator ==(unsigned rhs) const { return type_ == VAR_INT ? value_.int_ == (int)rhs : false; }

		 
		bool operator ==(bool rhs) const { return type_ == VAR_BOOL ? value_.bool_ == rhs : false; }

		 
		bool operator ==(float rhs) const { return type_ == VAR_FLOAT ? value_.float_ == rhs : false; }

		 
		bool operator ==(double rhs) const { return type_ == VAR_DOUBLE ? *(reinterpret_cast<const double*>(&value_)) == rhs : false; }

		 
		bool operator ==(const Vector2& rhs) const
		{
			return type_ == VAR_VECTOR2 ? *(reinterpret_cast<const Vector2*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const Vector3& rhs) const
		{
			return type_ == VAR_VECTOR3 ? *(reinterpret_cast<const Vector3*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const Vector4& rhs) const
		{
			return type_ == VAR_VECTOR4 ? *(reinterpret_cast<const Vector4*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const Quaternion& rhs) const
		{
			return type_ == VAR_QUATERNION ? *(reinterpret_cast<const Quaternion*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const Color& rhs) const
		{
			return type_ == VAR_COLOR ? *(reinterpret_cast<const Color*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const String& rhs) const
		{
			return type_ == VAR_STRING ? *(reinterpret_cast<const String*>(&value_)) == rhs : false;
		}

		bool operator ==(const std::vector<byte>& rhs) const;
		 
		bool operator ==(void* rhs) const
		{
			if (type_ == VAR_VOIDPTR)
				return value_.ptr_ == rhs;
			else if (type_ == VAR_PTR)
				return *(reinterpret_cast<const WeakPtr<RefCounter>*>(&value_)) == rhs;
			else
				return false;
		}

	 
		bool operator ==(const ResourceRef& rhs) const
		{
			return type_ == VAR_RESOURCEREF ? *(reinterpret_cast<const ResourceRef*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const ResourceRefList& rhs) const
		{
			return type_ == VAR_RESOURCEREFLIST ? *(reinterpret_cast<const ResourceRefList*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const VariantVector& rhs) const
		{
			return type_ == VAR_VARIANTVECTOR ? *(reinterpret_cast<const VariantVector*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const StringVector& rhs) const
		{
			return type_ == VAR_STRINGVECTOR ? *(reinterpret_cast<const StringVector*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const VariantMap& rhs) const
		{
			return type_ == VAR_VARIANTMAP ? *(reinterpret_cast<const VariantMap*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const IntRect& rhs) const
		{
			return type_ == VAR_INTRECT ? *(reinterpret_cast<const IntRect*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const IntVector2& rhs) const
		{
			return type_ == VAR_INTVECTOR2 ? *(reinterpret_cast<const IntVector2*>(&value_)) == rhs : false;
		}

		 
		bool operator ==(const StringHash& rhs) const { return type_ == VAR_INT ? (unsigned)value_.int_ == rhs.Value() : false; }

		 
		bool operator ==(RefCounter* rhs) const
		{
			if (type_ == VAR_PTR)
				return *(reinterpret_cast<const WeakPtr<RefCounter>*>(&value_)) == rhs;
			else if (type_ == VAR_VOIDPTR)
				return value_.ptr_ == rhs;
			else
				return false;
		}

		 
		bool operator ==(const Matrix3x3& rhs) const
		{
			return type_ == VAR_MATRIX3X3 ? *(reinterpret_cast<const Matrix3x3*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const Matrix3x4& rhs) const
		{
			return type_ == VAR_MATRIX3X4 ? *(reinterpret_cast<const Matrix3x4*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator ==(const Matrix4x4& rhs) const
		{
			return type_ == VAR_MATRIX4X4 ? *(reinterpret_cast<const Matrix4x4*>(value_.ptr_)) == rhs : false;
		}

		 
		bool operator !=(const Variant& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(int rhs) const { return !(*this == rhs); }

		 
		bool operator !=(unsigned rhs) const { return !(*this == rhs); }

		 
		bool operator !=(bool rhs) const { return !(*this == rhs); }

		 
		bool operator !=(float rhs) const { return !(*this == rhs); }

		 
		bool operator !=(double rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Vector2& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Vector3& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Vector4& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const String& rhs) const { return !(*this == rhs); }

		bool operator !=(const std::vector<byte>& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(void* rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const ResourceRef& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const ResourceRefList& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const VariantVector& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const StringVector& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const VariantMap& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const IntRect& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const IntVector2& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const StringHash& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(RefCounter* rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Matrix3x3& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Matrix3x4& rhs) const { return !(*this == rhs); }

		 
		bool operator !=(const Matrix4x4& rhs) const { return !(*this == rhs); }

		//设置变体类型
		void SetType(VariantType newType);  //有分配需要的根据类型分配空间
		///通过字符串设置类型名字和值
		void FromString(const String& type, const String& value);
		 
		void FromString(const char* type, const char* value);
		 
		void FromString(VariantType type, const String& value);
		 
		void FromString(VariantType type, const char* value);

		void SetBuffer(const void* data, unsigned size);

		/// Return name for variant type.
		static String GetTypeName(VariantType type);
		/// Return variant type from type name.
		static VariantType GetTypeFromName(const String& typeName);
		/// Return variant type from type name.
		static VariantType GetTypeFromName(const char* typeName);

		///如果是浮点型，转换一下
		int GetInt() const
		{
			if (type_ == VAR_INT)
				return value_.int_;
			else if (type_ == VAR_FLOAT)
				return (int)value_.float_;
			else if (type_ == VAR_DOUBLE)
				return (int)*reinterpret_cast<const double*>(&value_);
			else
				return 0;
		}

		/// Return unsigned int or zero on type mismatch. Floats and doubles are converted.
		unsigned GetUInt() const
		{
			if (type_ == VAR_INT)
				return value_.int_;
			else if (type_ == VAR_FLOAT)
				return (unsigned)value_.float_;
			else if (type_ == VAR_DOUBLE)
				return (unsigned)*reinterpret_cast<const double*>(&value_);
			else
				return 0;
		}

		/// Return StringHash or zero on type mismatch.
		StringHash GetStringHash() const { return StringHash(GetUInt()); }

		/// Return bool or false on type mismatch.
		bool GetBool() const { return type_ == VAR_BOOL ? value_.bool_ : false; }

		/// Return float or zero on type mismatch. Ints and doubles are converted.
		float GetFloat() const
		{
			if (type_ == VAR_FLOAT)
				return value_.float_;
			else if (type_ == VAR_DOUBLE)
				return (float)*reinterpret_cast<const double*>(&value_);
			else if (type_ == VAR_INT)
				return (float)value_.int_;
			else
				return 0.0f;
		}

		/// Return double or zero on type mismatch. Ints and floats are converted.
		double GetDouble() const
		{
			if (type_ == VAR_DOUBLE)
				return *reinterpret_cast<const double*>(&value_);
			else if (type_ == VAR_FLOAT)
				return (double)value_.float_;
			else if (type_ == VAR_INT)
				return (double)value_.int_;
			else
				return 0.0;
		}

		/// Return Vector2 or zero on type mismatch.
		const Vector2& GetVector2() const { return type_ == VAR_VECTOR2 ? *reinterpret_cast<const Vector2*>(&value_) : Vector2::ZERO; }

		/// Return Vector3 or zero on type mismatch.
		const Vector3& GetVector3() const { return type_ == VAR_VECTOR3 ? *reinterpret_cast<const Vector3*>(&value_) : Vector3::ZERO; }

		/// Return Vector4 or zero on type mismatch.
		const Vector4& GetVector4() const { return type_ == VAR_VECTOR4 ? *reinterpret_cast<const Vector4*>(&value_) : Vector4::ZERO; }

		/// Return quaternion or identity on type mismatch.
		const Quaternion& GetQuaternion() const
		{
			return type_ == VAR_QUATERNION ? *reinterpret_cast<const Quaternion*>(&value_) : Quaternion::IDENTITY;
		}

		/// Return color or default on type mismatch.
		const Color& GetColor() const { return type_ == VAR_COLOR ? *reinterpret_cast<const Color*>(&value_) : Color::WHITE; }

		/// Return string or empty on type mismatch.
		const String& GetString() const;

		const std::vector<byte>& GetBuffer() const
		{
			return type_ == VAR_BUFFER ? *reinterpret_cast<const std::vector<byte>*>(value_.ptr_) : emptyBuffer;
		}

		/// Return void pointer or null on type mismatch. RefCounted pointer will be converted.
		void* GetVoidPtr() const
		{
			if (type_ == VAR_VOIDPTR)
				return value_.ptr_;
			else if (type_ == VAR_PTR)
				return *reinterpret_cast<const WeakPtr<RefCounter>*>(&value_);
			else
				return 0;
		}

		/// Return a resource reference or empty on type mismatch.
		const ResourceRef& GetResourceRef() const
		{
			return type_ == VAR_RESOURCEREF ? *reinterpret_cast<const ResourceRef*>(value_.ptr_) : emptyResourceRef;
		}

		/// Return a resource reference list or empty on type mismatch.
		const ResourceRefList& GetResourceRefList() const
		{
			return type_ == VAR_RESOURCEREFLIST ? *reinterpret_cast<const ResourceRefList*>(value_.ptr_) : emptyResourceRefList;
		}

		/// Return a variant vector or empty on type mismatch.
		const VariantVector& GetVariantVector() const
		{
			return type_ == VAR_VARIANTVECTOR ? *reinterpret_cast<const VariantVector*>(value_.ptr_) : emptyVariantVector;
		}

		/// Return a string vector or empty on type mismatch.
		const StringVector& GetStringVector() const
		{
			return type_ == VAR_STRINGVECTOR ? *reinterpret_cast<const StringVector*>(value_.ptr_) : emptyStringVector;
		}

		/// Return a variant map or empty on type mismatch.
		const VariantMap& GetVariantMap() const
		{
			return type_ == VAR_VARIANTMAP ? *reinterpret_cast<const VariantMap*>(value_.ptr_) : emptyVariantMap;
		}

		/// Return an integer rect or empty on type mismatch.
		const IntRect& GetIntRect() const { return type_ == VAR_INTRECT ? *reinterpret_cast<const IntRect*>(&value_) : IntRect::ZERO; }

		/// Return an IntVector2 or empty on type mismatch.
		const IntVector2& GetIntVector2() const
		{
			return type_ == VAR_INTVECTOR2 ? *reinterpret_cast<const IntVector2*>(&value_) : IntVector2::ZERO;
		}

		/// Return a RefCounted pointer or null on type mismatch. Will return null if holding a void pointer, as it can not be safely verified that the object is a RefCounted.
		RefCounter* GetPtr() const
		{
			return type_ == VAR_PTR ? *reinterpret_cast<const WeakPtr<RefCounter>*>(&value_) : (RefCounter*)0;
		}

		/// Return a Matrix3 or identity on type mismatch.
		const Matrix3x3& GetMatrix3x3() const
		{
			return type_ == VAR_MATRIX3X3 ? *(reinterpret_cast<const Matrix3x3*>(value_.ptr_)) : Matrix3x3::IDENTITY;
		}

		/// Return a Matrix3x4 or identity on type mismatch.
		const Matrix3x4& GetMatrix3x4() const
		{
			return type_ == VAR_MATRIX3X4 ? *(reinterpret_cast<const Matrix3x4*>(value_.ptr_)) : Matrix3x4::IDENTITY;
		}

		/// Return a Matrix4 or identity on type mismatch.
		const Matrix4x4& GetMatrix4() const
		{
			return type_ == VAR_MATRIX4X4 ? *(reinterpret_cast<const Matrix4x4*>(value_.ptr_)) : Matrix4x4::IDENTITY;
		}

		/// Return value's type.
		VariantType GetType() const { return type_; }

		/// Convert value to string. Pointers are returned as null, and VariantBuffer or VariantMap are not supported and return empty.
		String ToString() const;

		/// Return true when the variant value is considered zero according to its actual type.
		bool IsZero() const;

		/// Return true when the variant is empty (i.e. not initialized yet).
		bool IsEmpty() const { return type_ == VAR_NONE; }

		/// Return the value, template version.
		template <class T> T Get() const;

		std::vector<byte>* GetBufferPtr()
		{
			return type_ == VAR_BUFFER ? reinterpret_cast<std::vector<byte>*>(&value_) : 0;
		}

		/// Return a pointer to a modifiable variant vector or null on type mismatch.
		VariantVector* GetVariantVectorPtr() { return type_ == VAR_VARIANTVECTOR ? reinterpret_cast<VariantVector*>(&value_) : 0; }

		/// Return a pointer to a modifiable string vector or null on type mismatch.
		StringVector* GetStringVectorPtr() { return type_ == VAR_STRINGVECTOR ? reinterpret_cast<StringVector*>(&value_) : 0; }

		/// Return a pointer to a modifiable variant map or null on type mismatch.
		VariantMap* GetVariantMapPtr() { return type_ == VAR_VARIANTMAP ? reinterpret_cast<VariantMap*>(&value_) : 0; }



		/// Empty variant.
		static const Variant EMPTY;
		/// Empty resource reference.
		static const ResourceRef emptyResourceRef;
		/// Empty resource reference list.
		static const ResourceRefList emptyResourceRefList;
		/// Empty variant map.
		static const VariantMap emptyVariantMap;
		/// Empty variant vector.
		static const VariantVector emptyVariantVector;
		/// Empty string vector.
		static const StringVector emptyStringVector;


		static const std::vector<byte> emptyBuffer;

		VariantType type_;
		VariantValue value_;
	};

	/// Return variant type from type.
	template <typename T> VariantType GetVariantType();

	/// Return variant type from concrete types.
	template <> inline VariantType GetVariantType<int>() { return VAR_INT; }

	template <> inline VariantType GetVariantType<unsigned>() { return VAR_INT; }

	template <> inline VariantType GetVariantType<bool>() { return VAR_BOOL; }

	template <> inline VariantType GetVariantType<float>() { return VAR_FLOAT; }

	template <> inline VariantType GetVariantType<double>() { return VAR_DOUBLE; }

	template <> inline VariantType GetVariantType<Vector2>() { return VAR_VECTOR2; }

	template <> inline VariantType GetVariantType<Vector3>() { return VAR_VECTOR3; }

	template <> inline VariantType GetVariantType<Vector4>() { return VAR_VECTOR4; }

	template <> inline VariantType GetVariantType<Quaternion>() { return VAR_QUATERNION; }

	template <> inline VariantType GetVariantType<Color>() { return VAR_COLOR; }

	template <> inline VariantType GetVariantType<String>() { return VAR_STRING; }

	template <> inline VariantType GetVariantType<StringHash>() { return VAR_INT; }

	template <> inline VariantType GetVariantType<std::vector<byte> >() { return VAR_BUFFER; }

	template <> inline VariantType GetVariantType<ResourceRef>() { return VAR_RESOURCEREF; }

	template <> inline VariantType GetVariantType<ResourceRefList>() { return VAR_RESOURCEREFLIST; }

	template <> inline VariantType GetVariantType<VariantVector>() { return VAR_VARIANTVECTOR; }

	template <> inline VariantType GetVariantType<StringVector >() { return VAR_STRINGVECTOR; }

	template <> inline VariantType GetVariantType<VariantMap>() { return VAR_VARIANTMAP; }

	template <> inline VariantType GetVariantType<IntRect>() { return VAR_INTRECT; }

	template <> inline VariantType GetVariantType<IntVector2>() { return VAR_INTVECTOR2; }

	template <> inline VariantType GetVariantType<Matrix3x3>() { return VAR_MATRIX3X3; }

	template <> inline VariantType GetVariantType<Matrix3x4>() { return VAR_MATRIX3X4; }

	template <> inline VariantType GetVariantType<Matrix4x4>() { return VAR_MATRIX4X4; }
}