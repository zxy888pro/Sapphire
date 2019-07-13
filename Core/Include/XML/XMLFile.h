#pragma once

#include "Sapphire.h"
#include "XML/XMLElement.h"
#include "Deserializer.h"

namespace pugi
{

	class xml_document;
	class xml_node;
	class xpath_node;

}

namespace Sapphire
{

	class SAPPHIRE_CLASS XMLFile : public BaseResource
	{
		SAPPHIRE_OBJECT(XMLFile, BaseResource)

	public: 
		XMLFile(Core* pcore, const char* resName="");
		virtual ~XMLFile();

		/// 从字符串反序列化
		bool FromString(const String& source);
		/// 清空文档，创建一个root元素
		XMLElement CreateRoot(const String& name);

		/// 返回root元素
		XMLElement GetRoot(const String& name = String::EMPTY);

		/// 返回 pugixml document.
		pugi::xml_document* GetDocument() const { return document_; }

		/// 序列化XMLFile内容到一个字符串
		String ToString(const String& indentation = "\t") const;

		/// 用另外一个XML文件修补一个XML文件，基于RFC 5261
		void Patch(XMLFile* patchFile);
		/// 用另外一个XML元素修补一个XML文件，基于RFC 5261
		void Patch(XMLElement patchElement);
		//從一個反序列化對象中讀取
		bool Load(Deserializer& deserializer);

		virtual void Clear() override;

		virtual bool Create() override;


		virtual void Destroy() override;


		virtual bool Load() override;

		//從文件加載XML對象
		virtual bool Load(const char* resPath) override;


		virtual bool Recreate() override;


		virtual void Dispose() override;


		virtual void OnLoadStart() override;


		virtual void OnLoadEnd() override;


		virtual void OnLoadError() override;


		virtual size_t GetSize() override;


		virtual bool IsDisposed() override;


		virtual bool operator <(BaseResource& container) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

	private:

		/// 添加一个节点到Patch
		void PatchAdd(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		/// 这个patch中替换一个节点或者属性在
		void PatchReplace(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		///  这个patch中移除一个节点或者属性在 
		void PatchRemove(const pugi::xpath_node& original) const;

		///添加一个节点到Patch
		void AddNode(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// 添加一个属性到Patch
		void AddAttribute(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// 合并两个节点
		bool CombineText(const pugi::xml_node& patch, const pugi::xml_node& original, bool prepend) const;

		pugi::xml_document* document_;

		std::string m_name;

	public:
		
		
	};

}