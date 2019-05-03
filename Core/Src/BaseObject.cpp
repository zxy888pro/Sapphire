#include "Sapphire.h"
#include "BaseObject.h"

namespace Sapphire
{

	
	 
	/*BaseObject::BaseObject(Core* pCore)
	{

	}*/

	/*void BaseObject::Awake()
	{

	}


	void BaseObject::Start()
	{

	}

	void BaseObject::Update()
	{

	}

	void BaseObject::Release()
	{

	}

	void BaseObject::NotifyEvent()
	{

	}*/



	ClassTypeInfo::ClassTypeInfo(const char* className, const ClassTypeInfo* pBaseClassInfo) :
		m_classHash(className),
		m_className(className),
		m_pBaseClassInfo(pBaseClassInfo)
	{

	}

	ClassTypeInfo::~ClassTypeInfo()
	{

	}

	bool ClassTypeInfo::IsTypeOf(StringHash hashVal) const
	{
		const ClassTypeInfo* current = this;
		while (current)
		{
			if (current->GetClassType() == hashVal)
				return true;

			current = current->GetBaseClassTypeInfo();
		}

		return false;
	}

	bool ClassTypeInfo::IsTypeOf(const ClassTypeInfo* typeInfo) const
	{
		const ClassTypeInfo* current = this;
		while (current)
		{
			if (current == typeInfo)
				return true;

			current = current->GetBaseClassTypeInfo();
		}

		return false;
	}

	 

 

	BaseObject::BaseObject():
		m_pCore(NULL)
	{
		 
	}

	BaseObject::BaseObject(Core* pCore)
	{
		m_assert(pCore);
		m_pCore = pCore;
	}

	BaseObject::~BaseObject()
	{

	}

}