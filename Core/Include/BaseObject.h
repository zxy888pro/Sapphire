#pragma once

#include <SapphireDef.h>
#include "EventContext.h"
#include "Ptr.h"
#include "stringHelper.h"
//#include <sigslot/sigslot.h>

namespace Sapphire
{

	//运行时类信息
	class SAPPHIRE_CLASS ClassTypeInfo
	{
	public:

		ClassTypeInfo(const char* className, const ClassTypeInfo* pBaseClassInfo);
		virtual ~ClassTypeInfo();

		bool IsTypeOf(StringHash hashVal) const;
		bool IsTypeOf(const ClassTypeInfo*  typeInfo) const;

		StringHash GetClassType() const { return m_classHash; }

		const std::string& GetClassTypeName() const { return m_className; }

		const ClassTypeInfo* GetBaseClassTypeInfo() const { return m_pBaseClassInfo; }


	private:

		StringHash  m_classHash;
		std::string m_className;
		const ClassTypeInfo* m_pBaseClassInfo;

	};

#define SAPPHIRE_OBJECT(typeName, baseTypeName) \
    public: \
	        typedef typeName ClassName; \
	        typedef baseTypeName BaseClassName; \
	        virtual Sapphire::StringHash GetClassType() const { return GetClassTypeInfoStatic()->GetClassType(); } \
	        virtual const std::string& GetClassTypeName() const { return GetClassTypeInfoStatic()->GetClassTypeName(); } \
	        virtual const Sapphire::ClassTypeInfo* GetClassTypeInfo() const { return GetClassTypeInfoStatic(); } \
	        static Sapphire::StringHash GetClassTypeStatic() { return GetClassTypeInfoStatic()->GetClassType(); } \
	        static const std::string& GetClassTypeNameStatic() { return GetClassTypeInfoStatic()->GetClassTypeName(); } \
	        static const Sapphire::ClassTypeInfo* GetClassTypeInfoStatic() { static const Sapphire::ClassTypeInfo m_classTypeInfoStatic(#typeName, BaseClassName::GetClassTypeInfoStatic()); return &m_classTypeInfoStatic; } \

	class SAPPHIRE_CLASS BaseObject :public EventContext, public RefCounter
	{
	public:

		BaseObject();
		BaseObject(Core* pCore);
		virtual ~BaseObject();

		static const ClassTypeInfo* GetClassTypeInfoStatic() { return 0; }

	protected:

		Core* m_pCore;

	private:




	};
}