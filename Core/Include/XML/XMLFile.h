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

		/// ���ַ��������л�
		bool FromString(const String& source);
		/// ����ĵ�������һ��rootԪ��
		XMLElement CreateRoot(const String& name);

		/// ����rootԪ��
		XMLElement GetRoot(const String& name = String::EMPTY);

		/// ���� pugixml document.
		pugi::xml_document* GetDocument() const { return document_; }

		/// ���л�XMLFile���ݵ�һ���ַ���
		String ToString(const String& indentation = "\t") const;

		/// ������һ��XML�ļ��޲�һ��XML�ļ�������RFC 5261
		void Patch(XMLFile* patchFile);
		/// ������һ��XMLԪ���޲�һ��XML�ļ�������RFC 5261
		void Patch(XMLElement patchElement);
		//��һ�������л��������xȡ
		bool Load(Deserializer& deserializer);

		virtual void Clear() override;

		virtual bool Create() override;


		virtual void Destroy() override;


		virtual bool Load() override;

		//���ļ����dXML����
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

		/// ���һ���ڵ㵽Patch
		void PatchAdd(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		/// ���patch���滻һ���ڵ����������
		void PatchReplace(const pugi::xml_node& patch, pugi::xpath_node& original) const;
		///  ���patch���Ƴ�һ���ڵ���������� 
		void PatchRemove(const pugi::xpath_node& original) const;

		///���һ���ڵ㵽Patch
		void AddNode(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// ���һ�����Ե�Patch
		void AddAttribute(const pugi::xml_node& patch, const pugi::xpath_node& original) const;
		/// �ϲ������ڵ�
		bool CombineText(const pugi::xml_node& patch, const pugi::xml_node& original, bool prepend) const;

		pugi::xml_document* document_;

		std::string m_name;

	public:
		
		
	};

}