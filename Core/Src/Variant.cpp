#include <Sapphire.h>
#include <RefCounter.h>
#include <Variant.h>



namespace Sapphire
{
	const ResourceRef Variant::emptyResourceRef;
	const ResourceRefList Variant::emptyResourceRefList;
	const VariantVector Variant::emptyVariantVector;
	const StringVector Variant::emptyStringVector;
	const VariantMap Variant::emptyVariantMap;
	const std::vector<byte> Variant::emptyBuffer;


	static const char* typeNames[] =
	{
		"None",
		"Int",
		"Bool",
		"Float",
		"Vector2",
		"Vector3",
		"Vector4",
		"Quaternion",
		"Color",
		"String",
		"VoidPtr",
		//"Buffer",
		"ResourceRef",
		"ResourceRefList",
		"VariantVector",
		"VariantMap",
		"IntRect",
		"IntVector2",
		"Ptr",
		"Matrix3",
		"Matrix3x4",
		"Matrix4",
		"Double",
		"StringVector",
		0
	};

	ResourceRef::ResourceRef(StringHash type):type_(type)
	{

	}

	ResourceRef::ResourceRef(StringHash type, const String& name) :
		type_(type),
		name_(name)
	{

	}

	ResourceRef::ResourceRef(const ResourceRef& rhs) :
		type_(rhs.type_),
		name_(rhs.name_)
	{

	}

	bool ResourceRef::operator!=(const ResourceRef& rhs) const
	{
		return type_ != rhs.type_ && name_ != rhs.name_;
	}

	bool ResourceRef::operator==(const ResourceRef& rhs) const
	{
		return type_ == rhs.type_ || name_ == rhs.name_;

	}

	Variant& Variant::operator =(const Variant& rhs)
	{
		SetType(rhs.GetType());

		switch (type_)
		{
		case VAR_STRING:
			*(reinterpret_cast<String*>(value_.ptr_)) = *(reinterpret_cast<const String*>(&rhs.value_.ptr_)); //字符串对象大小超16字节,在堆上分配，去ptr指针的值
			break;

		case VAR_BUFFER:
			*(reinterpret_cast<std::vector<byte>*>(value_.ptr_)) = *(reinterpret_cast<const std::vector<byte>*>(rhs.value_.ptr_));
			break;

		case VAR_RESOURCEREF:
			*(reinterpret_cast<ResourceRef*>(value_.ptr_)) = *(reinterpret_cast<const ResourceRef*>(rhs.value_.ptr_));
			break;

		case VAR_RESOURCEREFLIST:
			*(reinterpret_cast<ResourceRefList*>(value_.ptr_)) = *(reinterpret_cast<const ResourceRefList*>(rhs.value_.ptr_));
			break;

		case VAR_VARIANTVECTOR:
			*(reinterpret_cast<VariantVector*>(value_.ptr_)) = *(reinterpret_cast<const VariantVector*>(rhs.value_.ptr_));
			break;

		case VAR_STRINGVECTOR:
			*(reinterpret_cast<StringVector*>(value_.ptr_)) = *(reinterpret_cast<const StringVector*>(rhs.value_.ptr_));
			break;

		case VAR_VARIANTMAP:
			*(reinterpret_cast<VariantMap*>(value_.ptr_)) = *(reinterpret_cast<const VariantMap*>(rhs.value_.ptr_));
			break;

		case VAR_PTR:
			*(reinterpret_cast<WeakPtr<RefCounter>*>(&value_)) = *(reinterpret_cast<const WeakPtr<RefCounter>*>(&rhs.value_));
			break;

		case VAR_MATRIX3X3:
			*(reinterpret_cast<Matrix3x3*>(value_.ptr_)) = *(reinterpret_cast<const Matrix3x3*>(rhs.value_.ptr_));
			break;

		case VAR_MATRIX3X4:
			*(reinterpret_cast<Matrix3x4*>(value_.ptr_)) = *(reinterpret_cast<const Matrix3x4*>(rhs.value_.ptr_));
			break;

		case VAR_MATRIX4X4:
			*(reinterpret_cast<Matrix4x4*>(value_.ptr_)) = *(reinterpret_cast<const Matrix4x4*>(rhs.value_.ptr_));
			break;

		default:
			value_ = rhs.value_;
			break;
		}

		return *this;
	}


	bool Variant::operator ==(const Variant& rhs) const
	{
		if (type_ == VAR_VOIDPTR || type_ == VAR_PTR)
			return GetVoidPtr() == rhs.GetVoidPtr();
		else if (type_ != rhs.type_)
			return false;

		switch (type_)
		{
		case VAR_INT:
			return value_.int_ == rhs.value_.int_;

		case VAR_BOOL:
			return value_.bool_ == rhs.value_.bool_;

		case VAR_FLOAT:
			return value_.float_ == rhs.value_.float_;

		case VAR_VECTOR2:
			return *(reinterpret_cast<const Vector2*>(&value_)) == *(reinterpret_cast<const Vector2*>(&rhs.value_));

		case VAR_VECTOR3:
			return *(reinterpret_cast<const Vector3*>(&value_)) == *(reinterpret_cast<const Vector3*>(&rhs.value_));

		case VAR_VECTOR4:
			return *(reinterpret_cast<const Vector4*>(&value_)) == *(reinterpret_cast<const Vector4*>(&rhs.value_));
		case VAR_QUATERNION:
			return *(reinterpret_cast<const Quaternion*>(&value_)) == *(reinterpret_cast<const Quaternion*>(&rhs.value_));
		case VAR_COLOR:
			return *(reinterpret_cast<const Vector4*>(&value_)) == *(reinterpret_cast<const Vector4*>(&rhs.value_));

		case VAR_STRING:
			return *(reinterpret_cast<const String*>(value_.ptr_)) == *(reinterpret_cast<const String*>(rhs.value_.ptr_));

		case VAR_BUFFER:
			return *(reinterpret_cast<const std::vector<byte>*>(value_.ptr_)) ==
				*(reinterpret_cast<const std::vector<byte>*>(rhs.value_.ptr_));

		case VAR_RESOURCEREF:
			return *(reinterpret_cast<const ResourceRef*>(value_.ptr_)) == *(reinterpret_cast<const ResourceRef*>(rhs.value_.ptr_));

		case VAR_RESOURCEREFLIST:
			return *(reinterpret_cast<const ResourceRefList*>(value_.ptr_)) == *(reinterpret_cast<const ResourceRefList*>(rhs.value_.ptr_));

		case VAR_VARIANTVECTOR:
			return *(reinterpret_cast<const VariantVector*>(value_.ptr_)) == *(reinterpret_cast<const VariantVector*>(rhs.value_.ptr_));

		case VAR_STRINGVECTOR:
			return *(reinterpret_cast<const StringVector*>(value_.ptr_)) == *(reinterpret_cast<const StringVector*>(rhs.value_.ptr_));

		case VAR_VARIANTMAP:
			return *(reinterpret_cast<const VariantMap*>(value_.ptr_)) == *(reinterpret_cast<const VariantMap*>(rhs.value_.ptr_));

		case VAR_INTRECT:
			return *(reinterpret_cast<const IntRect*>(&value_)) == *(reinterpret_cast<const IntRect*>(&rhs.value_));

		case VAR_INTVECTOR2:
			return *(reinterpret_cast<const IntVector2*>(&value_)) == *(reinterpret_cast<const IntVector2*>(&rhs.value_));

		case VAR_MATRIX3X3:
			return *(reinterpret_cast<const Matrix3x3*>(value_.ptr_)) == *(reinterpret_cast<const Matrix3x3*>(rhs.value_.ptr_));

		case VAR_MATRIX3X4:
			return *(reinterpret_cast<const Matrix3x4*>(value_.ptr_)) == *(reinterpret_cast<const Matrix3x4*>(rhs.value_.ptr_));

		case VAR_MATRIX4X4:
			return *(reinterpret_cast<const Matrix4x4*>(value_.ptr_)) == *(reinterpret_cast<const Matrix4x4*>(rhs.value_.ptr_));

		case VAR_DOUBLE:
			return *(reinterpret_cast<const double*>(&value_)) == *(reinterpret_cast<const double*>(&rhs.value_));

		default:
			return true;
		}
	}

	bool Variant::operator ==(const std::vector<byte>& rhs) const
	{
		// 用strncmp()来当字符串比较
		const std::vector<byte>& buffer = *(reinterpret_cast<const std::vector<byte>*>(value_.ptr_));
		return type_ == VAR_BUFFER && buffer.size() == rhs.size() ?
			strncmp(reinterpret_cast<const char*>(&buffer[0]), reinterpret_cast<const char*>(&rhs[0]), buffer.size()) == 0 :
			false;
	}

	ResourceRefList::ResourceRefList(StringHash type) :type_(type)
	{

	}

	ResourceRefList::ResourceRefList(StringHash type, const StringVector& names):
		type_(type),
		names_(names)
	{

	}

	bool ResourceRefList::operator!=(const ResourceRefList& rhs) const
	{
		return type_ != rhs.type_ && names_ != rhs.names_;
	}

	bool ResourceRefList::operator==(const ResourceRefList& rhs) const
	{
		return type_ == rhs.type_ || names_ == rhs.names_;
	}


	String Variant::GetTypeName(VariantType type)
	{
		return typeNames[type];
	}

	Sapphire::String Variant::GetTypeName() const
	{
		return typeNames[type_];
	}

	VariantType Variant::GetTypeFromName(const String& typeName)
	{
		return GetTypeFromName(typeName.c_str());
	}

	VariantType Variant::GetTypeFromName(const char* typeName)
	{
		return (VariantType)GetStringListIndex(typeName, typeNames, VAR_NONE);
	}

	const Sapphire::String& Variant::GetString() const
	{
		return type_ == VAR_STRING ? *reinterpret_cast<const String*>(value_.ptr_) : String::EMPTY;;
	}

	Sapphire::String Variant::ToString() const
	{
		switch (type_)
		{
		case VAR_INT:
			return String(value_.int_);

		case VAR_BOOL:
			return String(value_.bool_);

		case VAR_FLOAT:
			return String(value_.float_);

		case VAR_VECTOR2:
			return (reinterpret_cast<const Vector2*>(&value_))->ToString();

		case VAR_VECTOR3:
			return (reinterpret_cast<const Vector3*>(&value_))->ToString();

		case VAR_VECTOR4:
			return (reinterpret_cast<const Vector4*>(&value_))->ToString();

		case VAR_QUATERNION:
			return (reinterpret_cast<const Quaternion*>(&value_))->ToString();

		case VAR_COLOR:
			return (reinterpret_cast<const Color*>(&value_))->ToString();

		case VAR_STRING:
			return *(reinterpret_cast<const String*>(value_.ptr_));

		case VAR_BUFFER:
		{
			std::vector<byte>& buffer = *(reinterpret_cast<std::vector<byte>*>(value_.ptr_));
			std::string ret;
			BufferToString(ret, &buffer[0], buffer.size());
			return ret;
		}

		case VAR_VOIDPTR:
		case VAR_PTR:
			//不支持指针序列化
			return String(0);

		case VAR_INTRECT:
			return (reinterpret_cast<const IntRect*>(&value_))->ToString();

		case VAR_INTVECTOR2:
			return (reinterpret_cast<const IntVector2*>(&value_))->ToString();

		case VAR_MATRIX3X3:
			return (reinterpret_cast<const Matrix3x3*>(value_.ptr_))->ToString();

		case VAR_MATRIX3X4:
			return (reinterpret_cast<const Matrix3x4*>(value_.ptr_))->ToString();

		case VAR_MATRIX4X4:
			return (reinterpret_cast<const Matrix4x4*>(value_.ptr_))->ToString();

		case VAR_DOUBLE:
			return String(*reinterpret_cast<const double*>(&value_));

		default:
			 
			return String::EMPTY;
		}
	}

	bool Variant::IsZero() const
	{
		switch (type_)
		{
		case VAR_INT:
			return value_.int_ == 0;

		case VAR_BOOL:
			return value_.bool_ == false;

		case VAR_FLOAT:
			return value_.float_ == 0.0f;

		case VAR_VECTOR2:
			return *reinterpret_cast<const Vector2*>(&value_) == Vector2::ZERO;

		case VAR_VECTOR3:
			return *reinterpret_cast<const Vector3*>(&value_) == Vector3::ZERO;

		case VAR_VECTOR4:
			return *reinterpret_cast<const Vector4*>(&value_) == Vector4::ZERO;

		case VAR_QUATERNION:
			return *reinterpret_cast<const Quaternion*>(&value_) == Quaternion::IDENTITY;

		case VAR_COLOR:
			 
			return *reinterpret_cast<const Color*>(&value_) == Color::WHITE;
		case VAR_STRING:
			return reinterpret_cast<const String*>(value_.ptr_)->empty();
			//return reinterpret_cast<const String*>(&value_)->Empty();

		case VAR_BUFFER:
			return reinterpret_cast<const std::vector<byte>*>(value_.ptr_)->empty();

		case VAR_VOIDPTR:
			return value_.ptr_ == 0;

		case VAR_RESOURCEREF:
			//return reinterpret_cast<const ResourceRef*>(&value_)->name_.empty();
			return reinterpret_cast<const ResourceRef*>(value_.ptr_)->name_.empty();

		case VAR_RESOURCEREFLIST:
		{
			const StringVector& names = reinterpret_cast<const ResourceRefList*>(value_.ptr_)->names_;
			for (StringVector::const_iterator i = names.begin(); i != names.end(); ++i)
			{
				if (!i->empty())
					return false;
			}
			return true;
		}

		case VAR_VARIANTVECTOR:
			return reinterpret_cast<const VariantVector*>(value_.ptr_)->empty();

		case VAR_STRINGVECTOR:
			return reinterpret_cast<const StringVector*>(value_.ptr_)->empty();

		case VAR_VARIANTMAP:
			return reinterpret_cast<const VariantMap*>(value_.ptr_)->empty();

		case VAR_INTRECT:
			return *reinterpret_cast<const IntRect*>(&value_) == IntRect::ZERO;

		case VAR_INTVECTOR2:
			return *reinterpret_cast<const IntVector2*>(&value_) == IntVector2::ZERO;

		case VAR_PTR:
			return *reinterpret_cast<const WeakPtr<RefCounter>*>(&value_) == (RefCounter*)0;

		case VAR_MATRIX3X3:
			return *reinterpret_cast<const Matrix3x3*>(value_.ptr_) == Matrix3x3::IDENTITY;

		case VAR_MATRIX3X4:
			return *reinterpret_cast<const Matrix3x4*>(value_.ptr_) == Matrix3x4::IDENTITY;

		case VAR_MATRIX4X4:
			return *reinterpret_cast<const Matrix4x4*>(value_.ptr_) == Matrix4x4::IDENTITY;

		case VAR_DOUBLE:
			return *reinterpret_cast<const double*>(&value_) == 0.0;

		default:
			return true;
		}
	}

	void Variant::FromString(const String& type, const String& value)
	{
		return FromString(GetTypeFromName(type), value.c_str());
	}

	void Variant::FromString(const char* type, const char* value)
	{
		return FromString(GetTypeFromName(type), value);
	}

	void Variant::FromString(VariantType type, const String& value)
	{
		return FromString(type, value.c_str());
	}
	void Variant::FromString(VariantType type, const char* value)
	{
		switch (type)
		{
		case VAR_INT:
			*this = ToInt(value);
			break;

		case VAR_BOOL:
			*this = ToBool(value);
			break;

		case VAR_FLOAT:
			*this = ToFloat(value);
			break;

		case VAR_VECTOR2:
			*this = ToVector2(value);
			break;

		case VAR_VECTOR3:
			*this = ToVector3(value);
			break;

		case VAR_VECTOR4:
			*this = ToVector4(value);
			break;

		case VAR_QUATERNION:
			*this = ToQuaternion(value);
			break;

		case VAR_COLOR:
			*this = ToColor(value);
			break;

		case VAR_STRING:
				*this = value;
				break;

		break;
		case VAR_BUFFER:
		{
			SetType(VAR_BUFFER); //预先分配一下
			std::vector<byte>& buffer = *(reinterpret_cast<std::vector<byte>*>(value_.ptr_));
			StringToBuffer(buffer, value);
		}
		break;

		case VAR_VOIDPTR:
			// From string to void pointer not supported, set to null
			*this = (void*)0;
			break;

		case VAR_RESOURCEREF:
		{
			StringVector values = String::Split(value, ';');
			if (values.size() == 2)
			{
				SetType(VAR_RESOURCEREF);
				ResourceRef& ref = *(reinterpret_cast<ResourceRef*>(value_.ptr_));
				ref.type_ = values[0];
				ref.name_ = values[1];
			}
		}
		break;

		case VAR_RESOURCEREFLIST:
		{
			StringVector values = String::Split(value, ';', true);
			if (values.size() >= 1)
			{
				SetType(VAR_RESOURCEREFLIST);
				ResourceRefList& refList = *(reinterpret_cast<ResourceRefList*>(value_.ptr_));
				refList.type_ = values[0];
				refList.names_.resize(values.size() - 1);
				for (unsigned i = 1; i < values.size(); ++i)
					refList.names_[i - 1] = values[i];
			}
		}
		break;

		case VAR_INTRECT:
			*this = ToIntRect(value);
			break;

		case VAR_INTVECTOR2:
			*this = ToIntVector2(value);
			break;

		case VAR_PTR:
			// From string to RefCounted pointer not supported, set to null
			*this = (RefCounter*)0;
			break;

		case VAR_MATRIX3X3:
			*this = ToMatrix3x3(value);
			break;

		case VAR_MATRIX3X4:
			*this = ToMatrix3x4(value);
			break;

		case VAR_MATRIX4X4:
			*this = ToMatrix4x4(value);
			break;

		case VAR_DOUBLE:
			*this = ToDouble(value);
			break;

		default:
			SetType(VAR_NONE);
		}
	}

	void Variant::SetBuffer(const void* data, unsigned size)
	{
		if (size && !data)
			size = 0;

		SetType(VAR_BUFFER);
		std::vector<byte>& buffer = *(reinterpret_cast<std::vector<byte>*>(value_.ptr_));
		buffer.resize(size);
		if (size)
			memcpy(&buffer[0], data, size);
	}

	void Variant::SetType(VariantType newType)
	{
		//如果类型一致，不用重分配内存
		if (type_ == newType)
			return;

		//非POD对象类型，需要进行内存分配
		//如有先释放
		//placement new要显示的调用他们的析构函数来销毁，不要使用delete
		switch (type_)
		{
		case VAR_STRING:
				//(reinterpret_cast<String*>(&value_))->~String();
				delete reinterpret_cast<String*>(value_.ptr_);
				break;

		case VAR_BUFFER:
			delete (reinterpret_cast<std::vector<byte>*>(value_.ptr_)); //删掉分配的vector指针
			break;

		case VAR_RESOURCEREF:
			//(reinterpret_cast<ResourceRef*>(&value_))->~ResourceRef();
			delete reinterpret_cast<ResourceRef*>(value_.ptr_);
			break;

		case VAR_RESOURCEREFLIST:
			//(reinterpret_cast<ResourceRefList*>(&value_))->~ResourceRefList();
			delete reinterpret_cast<ResourceRefList*>(value_.ptr_);
			break;

		case VAR_VARIANTVECTOR:
			//(reinterpret_cast<VariantVector*>(&value_))->~VariantVector();
			delete reinterpret_cast<VariantVector*>(value_.ptr_);
			break;

		case VAR_STRINGVECTOR:
			//(reinterpret_cast<StringVector*>(&value_))->~StringVector();
			delete reinterpret_cast<StringVector*>(value_.ptr_);
			break;

		case VAR_VARIANTMAP:
			//(reinterpret_cast<VariantMap*>(&value_))->~VariantMap();
			delete reinterpret_cast<VariantMap*>(value_.ptr_);
			break;

		case VAR_PTR:
			(reinterpret_cast<WeakPtr<RefCounter>*>(&value_))->~WeakPtr<RefCounter>();
			break;

		case VAR_MATRIX3X3:
			delete reinterpret_cast<Matrix3x3*>(value_.ptr_);
			break;

		case VAR_MATRIX3X4:
			delete reinterpret_cast<Matrix3x4*>(value_.ptr_);
			break;

		case VAR_MATRIX4X4:
			delete reinterpret_cast<Matrix4x4*>(value_.ptr_);
			break;

		default:
			break;
		}

		type_ = newType;

		//分配内存
		//placement new 方式
		switch (type_)
		{
		case VAR_STRING:
			//new(reinterpret_cast<String*>(&value_)) String();
			value_.ptr_ = new String();
			break;

		case VAR_BUFFER:
			//new(reinterpret_cast<std::vector<byte>*>(&value_)) std::vector<byte>();
			value_.ptr_ = new std::vector<byte>();
			break;

		case VAR_RESOURCEREF:
			//new(reinterpret_cast<ResourceRef*>(&value_)) ResourceRef();
			value_.ptr_ = new ResourceRef();
			break;

		case VAR_RESOURCEREFLIST:
			//new(reinterpret_cast<ResourceRefList*>(&value_)) ResourceRefList();
			value_.ptr_ = new ResourceRefList();
			break;

		case VAR_VARIANTVECTOR:
			//new(reinterpret_cast<VariantVector*>(&value_)) VariantVector();
			value_.ptr_ = new VariantVector();
			break;

		case VAR_STRINGVECTOR:
			//new(reinterpret_cast<StringVector*>(&value_)) StringVector();
			value_.ptr_ = new StringVector();
			break;

		case VAR_VARIANTMAP:
			//new(reinterpret_cast<VariantMap*>(&value_)) VariantMap();
			value_.ptr_ = new VariantMap();
			break;

		case VAR_PTR:
			new(reinterpret_cast<WeakPtr<RefCounter>*>(&value_)) WeakPtr<RefCounter>();
			break;

		case VAR_MATRIX3X3:
			value_.ptr_ = new Matrix3x3();
			break;

		case VAR_MATRIX3X4:
			value_.ptr_ = new Matrix3x4();
			break;

		case VAR_MATRIX4X4:
			value_.ptr_ = new Matrix4x4();
			break;

		default:
			break;
		}
	}


	template <> int Variant::Get<int>() const
	{
		return GetInt();
	}

	template <> unsigned Variant::Get<unsigned>() const
	{
		return GetUInt();
	}

	template <> StringHash Variant::Get<StringHash>() const
	{
		return GetStringHash();
	}

	template <> bool Variant::Get<bool>() const
	{
		return GetBool();
	}

	template <> float Variant::Get<float>() const
	{
		return GetFloat();
	}

	template <> double Variant::Get<double>() const
	{
		return GetDouble();
	}

	template <> const Vector2& Variant::Get<const Vector2&>() const
	{
		return GetVector2();
	}

	template <> const Vector3& Variant::Get<const Vector3&>() const
	{
		return GetVector3();
	}

	template <> const Vector4& Variant::Get<const Vector4&>() const
	{
		return GetVector4();
	}

	template <> const Quaternion& Variant::Get<const Quaternion&>() const
	{
		return GetQuaternion();
	}

	template <> const Color& Variant::Get<const Color&>() const
	{
		return GetColor();
	}

	template <> const String& Variant::Get<const String&>() const
	{
		return GetString();
	}

	template <> const IntRect& Variant::Get<const IntRect&>() const
	{
		return GetIntRect();
	}

	template <> const IntVector2& Variant::Get<const IntVector2&>() const
	{
		return GetIntVector2();
	}

	template <> const std::vector<byte>& Variant::Get<const std::vector<byte>&>() const
	{
		return GetBuffer();
	}

	template <> void* Variant::Get<void*>() const
	{
		return GetVoidPtr();
	}

	template <> RefCounter* Variant::Get<RefCounter*>() const
	{
		return GetPtr();
	}

	template <> const Matrix3x3& Variant::Get<const Matrix3x3&>() const
	{
		return GetMatrix3x3();
	}

	template <> const Matrix3x4& Variant::Get<const Matrix3x4&>() const
	{
		return GetMatrix3x4();
	}

	template <> const Matrix4x4& Variant::Get<const Matrix4x4&>() const
	{
		return GetMatrix4();
	}

	template <> ResourceRef Variant::Get<ResourceRef>() const
	{
		return GetResourceRef();
	}

	template <> ResourceRefList Variant::Get<ResourceRefList>() const
	{
		return GetResourceRefList();
	}

	template <> VariantVector Variant::Get<VariantVector>() const
	{
		return GetVariantVector();
	}

	template <> StringVector Variant::Get<StringVector >() const
	{
		return GetStringVector();
	}

	template <> VariantMap Variant::Get<VariantMap>() const
	{
		return GetVariantMap();
	}

	template <> Vector2 Variant::Get<Vector2>() const
	{
		return GetVector2();
	}

	template <> Vector3 Variant::Get<Vector3>() const
	{
		return GetVector3();
	}

	template <> Vector4 Variant::Get<Vector4>() const
	{
		return GetVector4();
	}

	template <> Quaternion Variant::Get<Quaternion>() const
	{
		return GetQuaternion();
	}

	template <> Color Variant::Get<Color>() const
	{
		return GetColor();
	}

	template <> String Variant::Get<String>() const
	{
		return GetString();
	}

	template <> IntRect Variant::Get<IntRect>() const
	{
		return GetIntRect();
	}

	template <> IntVector2 Variant::Get<IntVector2>() const
	{
		return GetIntVector2();
	}

	/*template <> PODVector<unsigned char> Variant::Get<PODVector<unsigned char> >() const
	{
	return GetBuffer();
	}*/

	template <> Matrix3x3 Variant::Get<Matrix3x3>() const
	{
		return GetMatrix3x3();
	}

	template <> Matrix3x4 Variant::Get<Matrix3x4>() const
	{
		return GetMatrix3x4();
	}

	template <> Matrix4x4 Variant::Get<Matrix4x4>() const
	{
		return GetMatrix4();
	}
}
