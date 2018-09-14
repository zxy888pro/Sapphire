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
		"String",
		"Stream",
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

		case VAR_STRING:
			*this = value;
			break;

		case VAR_STREAM:
		{
			SetType(VAR_STREAM);
			IStream& buffer = *(reinterpret_cast<IStream*>(&value_));
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
}
