#include <Sapphire.h>
#include <Variant.h>


namespace Sapphire
{

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

}
