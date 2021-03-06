#pragma once

#include "Sapphire.h"
#include "Variant.h"
#include "Math/BoundingBox.h"


namespace pugi
{

	struct xml_node_struct;
	class xpath_node;
	class xpath_node_set;
	class xpath_query;
	class xpath_variable_set;

}


namespace Sapphire
{

	class XMLFile;
	class XPathQuery;
	class XPathResultSet;


	class SAPPHIRE_CLASS XMLElement
	{
	public:

		//构造一个空元素
		XMLElement();
		//由一个文档和一个节点指针构造
		XMLElement(XMLFile* file, pugi::xml_node_struct* node);
		//由xpathQueryResultset构造
		XMLElement(XMLFile* file, const XPathResultSet* resultSet, const pugi::xpath_node* xpathNode, unsigned xpathResultIndex);

		XMLElement(const XMLElement& rhs);


		~XMLElement();


		XMLElement& operator =(const XMLElement& rhs);

		/// 创建一个子元素
		XMLElement CreateChild(const String& name);
		/// 创建一个子元素
		XMLElement CreateChild(const char* name);
		//	移除一个子元素
		bool RemoveChild(const XMLElement& element);
		//  通过名字，移除一个子元素
		bool RemoveChild(const String& name);
		
		bool RemoveChild(const char* name);
		/// 移除一个包含名字的子元素，如果名字为空，则移除所有子元素
		bool RemoveChildren(const String& name = String::EMPTY);
		/// 移除一个包含名字的子元素，如果名字为空，则移除所有子元素
		bool RemoveChildren(const char* name);
		/// 移除一个属性
		bool RemoveAttribute(const String& name = String::EMPTY);
		
		bool RemoveAttribute(const char* name);

		///用xpath query选择一个元素/属性
		XMLElement SelectSingle(const String& query, pugi::xpath_variable_set* variables = 0) const;
		///用xpath query选择一个元素/属性
		XMLElement SelectSinglePrepared(const XPathQuery& query) const;
		///用xpath query选择一个元素/属性
		XPathResultSet Select(const String& query, pugi::xpath_variable_set* variables = 0) const;
		///用xpath query选择一个元素/属性
		XPathResultSet SelectPrepared(const XPathQuery& query) const;

		/// 以下面的格式<node>value</node>，设置一个内部节点的值
		bool SetValue(const String& value);
		/// 以下面的格式<node>value</node>，设置一个内部节点的值
		bool SetValue(const char* value);
		/// 设置一个属性
		bool SetAttribute(const String& name, const String& value);
		/// 设置一个属性
		bool SetAttribute(const char* name, const char* value);
		/// 设置一个属性. 只对XPath Query result是一个属性的有效
		bool SetAttribute(const String& value);
		/// 设置一个属性. 只对XPath Query result是一个属性的有效
		bool SetAttribute(const char* value);
	
		bool SetBool(const String& name, bool value);
		//设置两个"min"和"max"的Vector3属性
		bool SetBoundingBox(const BoundingBox& value);

		bool SetBuffer(const String& name, const void* data, unsigned size);

		bool SetBuffer(const String& name, const std::vector<byte>& value);

		bool SetColor(const String& name, const Color& value);

		bool SetFloat(const String& name, float value);

		bool SetDouble(const String& name, double value);

		bool SetUInt(const String& name, unsigned value);

		bool SetInt(const String& name, int value);

		bool SetIntRect(const String& name, const IntRect& value);
		 
		bool SetIntVector2(const String& name, const IntVector2& value);
		 
		bool SetRect(const String& name, const Rect& value);
		 
		bool SetQuaternion(const String& name, const Quaternion& value);
		 
		bool SetString(const String& name, const String& value);
		 
		bool SetVariant(const Variant& value);
		 
		//设置变体
		bool SetVariantValue(const Variant& value);
		 
		bool SetResourceRef(const ResourceRef& value);
		 
		bool SetResourceRefList(const ResourceRefList& value);
		 
		bool SetVariantVector(const VariantVector& value);
		 
		bool SetStringVector(const StringVector& value);
		 
		bool SetVariantMap(const VariantMap& value);
		 
		bool SetVector2(const String& name, const Vector2& value);
		 
		bool SetVector3(const String& name, const Vector3& value);
		 
		bool SetVector4(const String& name, const Vector4& value);
		 
		bool SetVectorVariant(const String& name, const Variant& value);
		 
		bool SetMatrix3x3(const String& name, const Matrix3x3& value);
		 
		bool SetMatrix3x4(const String& name, const Matrix3x4& value);
		 
		bool SetMatrix4x4(const String& name, const Matrix4x4& value);

		 
		bool IsNull() const;
		 
		bool NotNull() const;
		/// 是否引用到一个元素或者xpath节点
		operator bool() const;
		/// 返回元素名字(或属性名 xpathQueryResult是一个属性)
		String GetName() const;
		
		bool HasChild(const String& name) const;
		
		bool HasChild(const char* name) const;
	 
		XMLElement GetChild(const String& name = String::EMPTY) const;
	 
		XMLElement GetChild(const char* name) const;
		/// 返回下一个兄弟元素
		XMLElement GetNext(const String& name = String::EMPTY) const;
		
		XMLElement GetNext(const char* name) const;
		
		XMLElement GetParent() const;
		  
		unsigned GetNumAttributes() const;
		 
		bool HasAttribute(const String& name) const;
		 
		bool HasAttribute(const char* name) const;
		
		String GetValue() const;
		
		String GetAttribute(const String& name = String::EMPTY) const;
		
		String GetAttribute(const char* name) const;
		 
		const char* GetAttributeCString(const char* name) const;
		 
		String GetAttributeLower(const String& name) const;
		 
		String GetAttributeLower(const char* name) const;
		 
		String GetAttributeUpper(const String& name) const;
		 
		String GetAttributeUpper(const char* name) const;
		 
		std::vector<std::string> GetAttributeNames() const;
		 
		bool GetBool(const String& name) const;
		 
		std::vector<byte> GetBuffer(const String& name) const;
		 
		bool GetBuffer(const String& name, void* dest, unsigned size) const;
		 
		BoundingBox GetBoundingBox() const;
		 
		Color GetColor(const String& name) const;
		 
		float GetFloat(const String& name) const;
		 
		double GetDouble(const String& name) const;
		 
		unsigned GetUInt(const String& name) const;
		 
		int GetInt(const String& name) const;
		 
		IntRect GetIntRect(const String& name) const;
		 
		IntVector2 GetIntVector2(const String& name) const;
		 
		Rect GetRect(const String& name) const;
		 
		Quaternion GetQuaternion(const String& name) const;
		 
		Variant GetVariant() const;
		 
		Variant GetVariantValue(VariantType type) const;
		 
		ResourceRef GetResourceRef() const;
		 
		ResourceRefList GetResourceRefList() const;
		 
		VariantVector GetVariantVector() const;
		 
		StringVector GetStringVector() const;
		 
		VariantMap GetVariantMap() const;
		 
		Vector2 GetVector2(const String& name) const;
		 
		Vector3 GetVector3(const String& name) const;
		 
		Vector4 GetVector4(const String& name) const;
		 
		Vector4 GetVector(const String& name) const;
		 
		Variant GetVectorVariant(const String& name) const;
		 
		Matrix3x3 GetMatrix3x3(const String& name) const;
		 
		Matrix3x4 GetMatrix3x4(const String& name) const;
		 
		Matrix4x4 GetMatrix4x4(const String& name) const;
		 
		XMLFile* GetFile() const;

		/// 返回pugixml xml_node_struct.
		pugi::xml_node_struct* GetNode() const { return node_; }

		/// 返回XPath query result集
		const XPathResultSet* GetXPathResultSet() const { return xpathResultSet_; }

		/// 返回 pugixml xpath_node.
		const pugi::xpath_node* GetXPathNode() const { return xpathNode_; }

		/// 返回 当前结果索引
		unsigned GetXPathResultIndex() const { return xpathResultIndex_; }

		/// 返回下一个Xpath query result 只有这个元素的实例是结果集中的一个query result的才有效
		XMLElement NextResult() const;
	 
		static const XMLElement EMPTY;

	private:
		 
		WeakPtr<XMLFile> file_;
		 
		pugi::xml_node_struct* node_;
		 
		const XPathResultSet* xpathResultSet_;
		 
		const pugi::xpath_node* xpathNode_;
		/// 当前查询结果集索引
		mutable unsigned xpathResultIndex_;


	};

	///  
	class SAPPHIRE_CLASS XPathResultSet
	{
	public:
		/// 构造一个空结果集
		XPathResultSet();
		/// 从xpath query构造一个结果集
		XPathResultSet(XMLFile* file, pugi::xpath_node_set* resultSet);
		// 拷贝构造
		XPathResultSet(const XPathResultSet& rhs);
		
		~XPathResultSet();
		
		XPathResultSet& operator =(const XPathResultSet& rhs);
		/// 返回集合中第n个结果, 调用 XMLElement::GetNextResult()来获取这个集合的下一个
		/// 注意：当使用XPathResultSet::FirstResult(), XPathResultSet::operator [],  XMLElement::NextResult()的时候，XPathResultSet返回值必须保持在左边变量相关的xpath_node_set是有效的
		XMLElement operator [](unsigned index) const;
		/// 返回集合中第n个结果, 调用 XMLElement::GetNextResult()来获取这个集合的下一个
		/// 注意：当使用XPathResultSet::FirstResult(), XPathResultSet::operator [],  XMLElement::NextResult()的时候，XPathResultSet返回值必须保持在左边变量相关的xpath_node_set是有效的
		XMLElement FirstResult();
		///返回结果集的大小
		unsigned Size() const;
		
		bool Empty() const;

		/// 返回 pugixml xpath_node_set.
		pugi::xpath_node_set* GetXPathNodeSet() const { return resultSet_; }

	private:
		 
		WeakPtr<XMLFile> file_;
		 
		pugi::xpath_node_set* resultSet_;
	};

	/// XPath query.
	// 对pugi xml query包装
	class SAPPHIRE_CLASS XPathQuery
	{
	public:
		 
		XPathQuery();
		/// 用查询字符串和可变字符串构造Xpath 查询对象， 可变字符串格式是"name1:type1,name2:type2,..."，type可以是"Bool", "Float", "String", "ResultSet".
		XPathQuery(const String& queryString, const String& variableString = String::EMPTY);
		
		~XPathQuery();
		/// 用一般可变集绑定查询对象
		void Bind();
		/// 添加设置bool变量
		bool SetVariable(const String& name, bool value);

		bool SetVariable(const String& name, float value);

		bool SetVariable(const String& name, const String& value);
	
		bool SetVariable(const char* name, const char* value);

		bool SetVariable(const String& name, const XPathResultSet& value);
		/// 设置Xpath查询字符串和可变字符串
		bool SetQuery(const String& queryString, const String& variableString = String::EMPTY, bool bind = true);
		/// 清空所有变量和Xpath query object
		void Clear();
		 
		bool EvaluateToBool(XMLElement element) const;
 
		float EvaluateToFloat(XMLElement element) const;
 
		String EvaluateToString(XMLElement element) const;
		 
		XPathResultSet Evaluate(XMLElement element) const;

	 
		String GetQuery() const { return queryString_; }

 
		pugi::xpath_query* GetXPathQuery() const { return query_; }

 
		pugi::xpath_variable_set* GetXPathVariableSet() const { return variables_; }

	private:
		 
		String queryString_;
	 
		pugi::xpath_query* query_;
		/// Pugixml xpath_variable_set.
		pugi::xpath_variable_set* variables_;
	};

}
