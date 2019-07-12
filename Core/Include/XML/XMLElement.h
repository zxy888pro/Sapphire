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

		//����һ����Ԫ��
		XMLElement();
		//��һ���ĵ���һ���ڵ�ָ�빹��
		XMLElement(XMLFile* file, pugi::xml_node_struct* node);
		//��xpathQueryResultset����
		XMLElement(XMLFile* file, const XPathResultSet* resultSet, const pugi::xpath_node* xpathNode, unsigned xpathResultIndex);

		XMLElement(const XMLElement& rhs);


		~XMLElement();


		XMLElement& operator =(const XMLElement& rhs);

		/// ����һ����Ԫ��
		XMLElement CreateChild(const String& name);
		/// ����һ����Ԫ��
		XMLElement CreateChild(const char* name);
		//	�Ƴ�һ����Ԫ��
		bool RemoveChild(const XMLElement& element);
		//  ͨ�����֣��Ƴ�һ����Ԫ��
		bool RemoveChild(const String& name);
		
		bool RemoveChild(const char* name);
		/// �Ƴ�һ���������ֵ���Ԫ�أ��������Ϊ�գ����Ƴ�������Ԫ��
		bool RemoveChildren(const String& name = String::EMPTY);
		/// �Ƴ�һ���������ֵ���Ԫ�أ��������Ϊ�գ����Ƴ�������Ԫ��
		bool RemoveChildren(const char* name);
		/// �Ƴ�һ������
		bool RemoveAttribute(const String& name = String::EMPTY);
		
		bool RemoveAttribute(const char* name);

		///��xpath queryѡ��һ��Ԫ��/����
		XMLElement SelectSingle(const String& query, pugi::xpath_variable_set* variables = 0) const;
		///��xpath queryѡ��һ��Ԫ��/����
		XMLElement SelectSinglePrepared(const XPathQuery& query) const;
		///��xpath queryѡ��һ��Ԫ��/����
		XPathResultSet Select(const String& query, pugi::xpath_variable_set* variables = 0) const;
		///��xpath queryѡ��һ��Ԫ��/����
		XPathResultSet SelectPrepared(const XPathQuery& query) const;

		/// ������ĸ�ʽ<node>value</node>������һ���ڲ��ڵ��ֵ
		bool SetValue(const String& value);
		/// ������ĸ�ʽ<node>value</node>������һ���ڲ��ڵ��ֵ
		bool SetValue(const char* value);
		/// ����һ������
		bool SetAttribute(const String& name, const String& value);
		/// ����һ������
		bool SetAttribute(const char* name, const char* value);
		/// ����һ������. ֻ��XPath Query result��һ�����Ե���Ч
		bool SetAttribute(const String& value);
		/// ����һ������. ֻ��XPath Query result��һ�����Ե���Ч
		bool SetAttribute(const char* value);
	
		bool SetBool(const String& name, bool value);
		//��������"min"��"max"��Vector3����
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
		 
		//���ñ���
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
		/// �Ƿ����õ�һ��Ԫ�ػ���xpath�ڵ�
		operator bool() const;
		/// ����Ԫ������(�������� xpathQueryResult��һ������)
		String GetName() const;
		
		bool HasChild(const String& name) const;
		
		bool HasChild(const char* name) const;
	 
		XMLElement GetChild(const String& name = String::EMPTY) const;
	 
		XMLElement GetChild(const char* name) const;
		/// ������һ���ֵ�Ԫ��
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

		/// ����pugixml xml_node_struct.
		pugi::xml_node_struct* GetNode() const { return node_; }

		/// ����XPath query result��
		const XPathResultSet* GetXPathResultSet() const { return xpathResultSet_; }

		/// ���� pugixml xpath_node.
		const pugi::xpath_node* GetXPathNode() const { return xpathNode_; }

		/// ���� ��ǰ�������
		unsigned GetXPathResultIndex() const { return xpathResultIndex_; }

		/// ������һ��Xpath query result ֻ�����Ԫ�ص�ʵ���ǽ�����е�һ��query result�Ĳ���Ч
		XMLElement NextResult() const;
	 
		static const XMLElement EMPTY;

	private:
		 
		WeakPtr<XMLFile> file_;
		 
		pugi::xml_node_struct* node_;
		 
		const XPathResultSet* xpathResultSet_;
		 
		const pugi::xpath_node* xpathNode_;
		/// ��ǰ��ѯ���������
		mutable unsigned xpathResultIndex_;

	};

	///  
	class SAPPHIRE_CLASS XPathResultSet
	{
	public:
		/// ����һ���ս����
		XPathResultSet();
		/// Construct with result set from XPath query.
		XPathResultSet(XMLFile* file, pugi::xpath_node_set* resultSet);
		// Copy-construct.
		XPathResultSet(const XPathResultSet& rhs);
		/// Destruct.
		~XPathResultSet();
		/// Assignment operator.
		XPathResultSet& operator =(const XPathResultSet& rhs);
		/// Return the n-th result in the set. Call XMLElement::GetNextResult() to get the subsequent result in the set.
		/// Note: The XPathResultSet return value must be stored in a lhs variable to ensure the underlying xpath_node_set* is still valid while performing XPathResultSet::FirstResult(), XPathResultSet::operator [], and XMLElement::NextResult().
		XMLElement operator [](unsigned index) const;
		/// Return the first result in the set. Call XMLElement::GetNextResult() to get the subsequent result in the set.
		/// Note: The XPathResultSet return value must be stored in a lhs variable to ensure the underlying xpath_node_set* is still valid while performing XPathResultSet::FirstResult(), XPathResultSet::operator [], and XMLElement::NextResult().
		XMLElement FirstResult();
		/// Return size of result set.
		unsigned Size() const;
		/// Return whether result set is empty.
		bool Empty() const;

		/// Return pugixml xpath_node_set.
		pugi::xpath_node_set* GetXPathNodeSet() const { return resultSet_; }

	private:
		/// XML file.
		WeakPtr<XMLFile> file_;
		/// Pugixml xpath_node_set.
		pugi::xpath_node_set* resultSet_;
	};

	/// XPath query.
	class SAPPHIRE_CLASS XPathQuery
	{
	public:
		/// Construct empty.
		XPathQuery();
		/// Construct XPath query object with query string and variable string. The variable string format is "name1:type1,name2:type2,..." where type is one of "Bool", "Float", "String", "ResultSet".
		XPathQuery(const String& queryString, const String& variableString = String::EMPTY);
		/// Destruct.
		~XPathQuery();
		/// Bind query object with variable set.
		void Bind();
		/// Add/Set a bool variable. Return true if successful.
		bool SetVariable(const String& name, bool value);
		/// Add/Set a float variable. Return true if successful.
		bool SetVariable(const String& name, float value);
		/// Add/Set a string variable. Return true if successful.
		bool SetVariable(const String& name, const String& value);
		/// Add/Set a string variable. Return true if successful.
		bool SetVariable(const char* name, const char* value);
		/// Add/Set an XPath query result set variable. Return true if successful.
		bool SetVariable(const String& name, const XPathResultSet& value);
		/// Set XPath query string and variable string. The variable string format is "name1:type1,name2:type2,..." where type is one of "Bool", "Float", "String", "ResultSet".
		bool SetQuery(const String& queryString, const String& variableString = String::EMPTY, bool bind = true);
		/// Clear by removing all variables and XPath query object.
		void Clear();
		/// Evaluate XPath query and expecting a boolean return value.
		bool EvaluateToBool(XMLElement element) const;
		/// Evaluate XPath query and expecting a float return value.
		float EvaluateToFloat(XMLElement element) const;
		/// Evaluate XPath query and expecting a string return value.
		String EvaluateToString(XMLElement element) const;
		/// Evaluate XPath query and expecting an XPath query result set as return value.
		/// Note: The XPathResultSet return value must be stored in a lhs variable to ensure the underlying xpath_node_set* is still valid while performing XPathResultSet::FirstResult(), XPathResultSet::operator [], and XMLElement::NextResult().
		XPathResultSet Evaluate(XMLElement element) const;

		/// Return query string.
		String GetQuery() const { return queryString_; }

		/// Return pugixml xpath_query.
		pugi::xpath_query* GetXPathQuery() const { return query_; }

		/// Return pugixml xpath_variable_set.
		pugi::xpath_variable_set* GetXPathVariableSet() const { return variables_; }

	private:
		/// XPath query string.
		String queryString_;
		/// Pugixml xpath_query.
		pugi::xpath_query* query_;
		/// Pugixml xpath_variable_set.
		pugi::xpath_variable_set* variables_;
	};

}
