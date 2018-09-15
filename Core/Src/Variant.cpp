#include <Sapphire.h>
#include <RefCounter.h>
#include <Variant.h>



namespace Sapphire
{
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
		//"String",
		"VoidPtr",
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
		return type_ == rhs.type_ && name_ == rhs.name_;
	}

	bool ResourceRef::operator==(const ResourceRef& rhs) const
	{
		return type_ != rhs.type_ || name_ != rhs.name_;

	}

	Variant& Variant::operator =(const Variant& rhs)
	{
		SetType(rhs.GetType());

		switch (type_)
		{
		/*case VAR_STRING:
			*(reinterpret_cast<String*>(&value_)) = *(reinterpret_cast<const String*>(&rhs.value_));
			break;*/

		case VAR_RESOURCEREF:
			*(reinterpret_cast<ResourceRef*>(&value_)) = *(reinterpret_cast<const ResourceRef*>(&rhs.value_));
			break;

		case VAR_RESOURCEREFLIST:
			*(reinterpret_cast<ResourceRefList*>(&value_)) = *(reinterpret_cast<const ResourceRefList*>(&rhs.value_));
			break;

		case VAR_VARIANTVECTOR:
			*(reinterpret_cast<VariantVector*>(&value_)) = *(reinterpret_cast<const VariantVector*>(&rhs.value_));
			break;

		case VAR_STRINGVECTOR:
			*(reinterpret_cast<StringVector*>(&value_)) = *(reinterpret_cast<const StringVector*>(&rhs.value_));
			break;

		case VAR_VARIANTMAP:
			*(reinterpret_cast<VariantMap*>(&value_)) = *(reinterpret_cast<const VariantMap*>(&rhs.value_));
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
		case VAR_QUATERNION:
		case VAR_COLOR:
			// Hack: use the Vector4 compare for all these classes, as they have the same memory structure
			return *(reinterpret_cast<const Vector4*>(&value_)) == *(reinterpret_cast<const Vector4*>(&rhs.value_));

		/*case VAR_STRING:
			return *(reinterpret_cast<const String*>(&value_)) == *(reinterpret_cast<const String*>(&rhs.value_));*/

		case VAR_RESOURCEREF:
			return *(reinterpret_cast<const ResourceRef*>(&value_)) == *(reinterpret_cast<const ResourceRef*>(&rhs.value_));

		case VAR_RESOURCEREFLIST:
			return *(reinterpret_cast<const ResourceRefList*>(&value_)) == *(reinterpret_cast<const ResourceRefList*>(&rhs.value_));

		case VAR_VARIANTVECTOR:
			return *(reinterpret_cast<const VariantVector*>(&value_)) == *(reinterpret_cast<const VariantVector*>(&rhs.value_));

		case VAR_STRINGVECTOR:
			return *(reinterpret_cast<const StringVector*>(&value_)) == *(reinterpret_cast<const StringVector*>(&rhs.value_));

		case VAR_VARIANTMAP:
			return *(reinterpret_cast<const VariantMap*>(&value_)) == *(reinterpret_cast<const VariantMap*>(&rhs.value_));

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
		return type_ == rhs.type_ && names_ == rhs.names_;
	}

	bool ResourceRefList::operator==(const ResourceRefList& rhs) const
	{
		return type_ != rhs.type_ || names_ != rhs.names_;
	}


	String Variant::GetTypeName(VariantType type)
	{
		return typeNames[type];
	}

	VariantType Variant::GetTypeFromName(const String& typeName)
	{
		return GetTypeFromName(typeName.c_str());
	}

	VariantType Variant::GetTypeFromName(const char* typeName)
	{
		return (VariantType)GetStringListIndex(typeName, typeNames, VAR_NONE);
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

			/*case VAR_STRING:
				*this = value;
				break;*/

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
				ResourceRef& ref = *(reinterpret_cast<ResourceRef*>(&value_));
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
				ResourceRefList& refList = *(reinterpret_cast<ResourceRefList*>(&value_));
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

	void Variant::SetType(VariantType newType)
	{
		if (type_ == newType)
			return;

		//如有先释放
		//placement new要显示的调用他们的析构函数来销毁，不要使用delete
		switch (type_)
		{
			/*case VAR_STRING:
				(reinterpret_cast<String*>(&value_))->~String();
				break;*/

		case VAR_RESOURCEREF:
			(reinterpret_cast<ResourceRef*>(&value_))->~ResourceRef();
			break;

		case VAR_RESOURCEREFLIST:
			(reinterpret_cast<ResourceRefList*>(&value_))->~ResourceRefList();
			break;

		case VAR_VARIANTVECTOR:
			(reinterpret_cast<VariantVector*>(&value_))->~VariantVector();
			break;

		case VAR_STRINGVECTOR:
			(reinterpret_cast<StringVector*>(&value_))->~StringVector();
			break;

		case VAR_VARIANTMAP:
			(reinterpret_cast<VariantMap*>(&value_))->~VariantMap();
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
		/*case VAR_STRING:
			new(reinterpret_cast<String*>(&value_)) String();
			break;*/

		case VAR_RESOURCEREF:
			new(reinterpret_cast<ResourceRef*>(&value_)) ResourceRef();
			break;

		case VAR_RESOURCEREFLIST:
			new(reinterpret_cast<ResourceRefList*>(&value_)) ResourceRefList();
			break;

		case VAR_VARIANTVECTOR:
			new(reinterpret_cast<VariantVector*>(&value_)) VariantVector();
			break;

		case VAR_STRINGVECTOR:
			new(reinterpret_cast<StringVector*>(&value_)) StringVector();
			break;

		case VAR_VARIANTMAP:
			new(reinterpret_cast<VariantMap*>(&value_)) VariantMap();
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
}
