#include "Sapphire.h"
#include "Ptr.h"
#include "ArrayPtr.h"
#include "FileStream.h"
#include "Serializer.h"
#include "VectorBuffer.h"
#include "MemoryBuffer.h"
#include "XML/XMLFile.h"
#include "pugixml.hpp"
#include "ResourceCache.h"


namespace Sapphire
{
	 
	class XMLWriter : public pugi::xml_writer
	{
	public:
		 
		XMLWriter(Serializer& dest) :
			dest_(dest),
			success_(true)
		{
		}

		 
		void write(const void* data, size_t size)
		{
			if (dest_.Write(data, (unsigned)size) != size)
				success_ = false;
		}

		 
		Serializer& dest_;
		bool success_;
	};

	XMLFile::XMLFile(Core* pcore,const char* resName) :BaseResource(pcore,resName),
		document_(new pugi::xml_document())
	{
		m_eType = ResourceType_XML;
		m_eState = ResourceState_Unload;
		 
	}

	XMLFile::~XMLFile()
	{
		safeDelete(document_);
	}

	bool XMLFile::FromString(const String& source)
	{
		if (source.empty())
			return false;

		MemoryBuffer buffer(source.c_str(), source.Length());
		return Load(buffer);
	}

	Sapphire::XMLElement XMLFile::CreateRoot(const String& name)
	{
		document_->reset();
		pugi::xml_node root = document_->append_child(name.c_str());
		return XMLElement(this, root.internal_object());
	}

	Sapphire::XMLElement XMLFile::GetRoot(const String& name /*= String::EMPTY*/)
	{
		pugi::xml_node root = document_->first_child();
		if (root.empty())
			return XMLElement();

		if (!name.empty() && name != root.name())
			return XMLElement();
		else
			return XMLElement(this, root.internal_object());
	}

	Sapphire::String XMLFile::ToString(const String& indentation /*= "\t"*/) const
	{
		VectorBuffer dest;
		XMLWriter writer(dest);
		document_->save(writer, indentation.c_str());
		return String((const char*)dest.GetData(), dest.GetSize());
	}

	void XMLFile::Patch(XMLFile* patchFile)
	{
		Patch(patchFile->GetRoot());
	}

	void XMLFile::Patch(XMLElement patchElement)
	{
		//取得xml_node
		pugi::xml_node root = pugi::xml_node(patchElement.GetNode());

		//遍历该node
		for (pugi::xml_node::iterator patch = root.begin(); patch != root.end(); patch++)
		{
			pugi::xml_attribute sel = patch->attribute("sel"); //有可选择属性
			if (sel.empty())
			{
				SAPPHIRE_LOGERROR("XML Patch failed due to node not having a sel attribute.");
				continue;
			}
			//通过sel.value的值选择一个单个节点
			pugi::xpath_node original = document_->select_single_node(sel.value());//一次只能选择单个节点
			if (!original)
			{
				SAPPHIRE_LOGERROR("XML Patch failed with bad select: %s.", sel.value());
				continue;
			}

			if (strcmp(patch->name(), "add") == 0)
				PatchAdd(*patch, original);
			else if (strcmp(patch->name(), "replace") == 0)
				PatchReplace(*patch, original);
			else if (strcmp(patch->name(), "remove") == 0)
				PatchRemove(original);
			else
				SAPPHIRE_LOGERROR("XMLFiles used for patching should only use 'add', 'replace' or 'remove' elements.");
		}
	}

	void XMLFile::Clear()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool XMLFile::Create()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void XMLFile::Destroy()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool XMLFile::Load()
	{
		Path fpath = m_resName;
		if (!FileIsExistA(fpath.c_str()))
		{
			LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
			return false;
		}
		std::string xmlStr = "";
		FileStream fs(m_resName.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			xmlStr = fs.ReadString();
			fs.Release();
		}
		return FromString(xmlStr);
	}

	bool XMLFile::Load(const char* resPath)
	{
		m_resName = resPath;
		return Load();
	}

	bool XMLFile::Load(Deserializer& source)
	{
		//获得数据大小
		unsigned dataSize = source.GetSize();
		if (!dataSize && !source.GetName().empty())
		{
			SAPPHIRE_LOGERROR("Zero sized XML data in " + source.GetName());
			return false;
		}

		//准备读取字符到buffer中
		SharedArrayPtr<char> buffer(new char[dataSize]);
		if (source.Read(buffer.Get(), dataSize) != dataSize)
			return false;

		//pugi_xml加载xml doc
		if (!document_->load_buffer(buffer.Get(), dataSize))
		{
			SAPPHIRE_LOGERROR("Could not parse XML data from " + source.GetName());
			document_->reset();
			return false;
		}

		XMLElement rootElem = GetRoot();
		String inherit = rootElem.GetAttribute("inherit"); 
		if (!inherit.empty())
		{
			
			ResourceCache* cache = dynamic_cast<ResourceCache*>(m_pCore->GetSubSystemWithType(ESST_RESOURCECACHE));

			//到缓存里去找，没有再去加载
			XMLFile* inheritedXMLFile = dynamic_cast<XMLFile*>(cache->GetResource(inherit.c_str())); 
			if (inheritedXMLFile = NULL)
			{
				//没有找到，直接加载了
				inheritedXMLFile = new XMLFile(m_pCore, inherit.c_str());
				inheritedXMLFile->Load();
			}
				
			if (!inheritedXMLFile || (inheritedXMLFile->GetState() != ResourceState_Loaded));
			{
				SAPPHIRE_LOGERROR("Could not find inherited XML file: %s", inherit.c_str());
				return false;
			}

			
			pugi::xml_document* patchDocument = document_;
			document_ = new pugi::xml_document();
			document_->reset(*inheritedXMLFile->document_);
			Patch(rootElem);
			delete patchDocument;

			//保存依赖关系
			cache->StoreResourceDependency(this, inherit.c_str());

			// 增加patch 的data size
			dataSize += inheritedXMLFile->GetSize();
		}
		
		return false;
	}

	bool XMLFile::Recreate()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void XMLFile::Dispose()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void XMLFile::OnLoadStart()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void XMLFile::OnLoadEnd()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void XMLFile::OnLoadError()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	size_t XMLFile::GetSize()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool XMLFile::IsDisposed()
	{
		return (!document_ || document_->empty());
	}

	void XMLFile::PatchAdd(const pugi::xml_node& patch, pugi::xpath_node& original) const
	{
		// 如果不是节点
		if (original.attribute())
		{
			SAPPHIRE_LOGERROR("XML Patch failed calling Add due to not selecting a node, %s attribute was selected.",
				original.attribute().name());
			return;
		}

		// 如果没有添加节点的类型，如果包含‘@’作为一个属性
		pugi::xml_attribute type = patch.attribute("type");
		if (!type || strlen(type.value()) <= 0)
			AddNode(patch, original);
		else if (type.value()[0] == '@')
			AddAttribute(patch, original);
	}

	void XMLFile::PatchReplace(const pugi::xml_node& patch, pugi::xpath_node& original) const
	{
		// 如果没有属性，而且它又是一个节点 
		if (!original.attribute() && original.node())
		{
			pugi::xml_node parent = original.node().parent();//取父节点

			parent.insert_copy_before(patch.first_child(), original.node());
			parent.remove_child(original.node());
		}
		else if (original.attribute())
		{
			original.attribute().set_value(patch.child_value());
		}
	}

	void XMLFile::PatchRemove(const pugi::xpath_node& original) const
	{
		 
		if (!original.attribute() && original.node())
		{
			pugi::xml_node parent = original.parent();
			parent.remove_child(original.node());
		}
		else if (original.attribute())
		{
			pugi::xml_node parent = original.parent();
			parent.remove_attribute(original.attribute());
		}
	}

	void XMLFile::AddNode(const pugi::xml_node& patch, const pugi::xpath_node& original) const
	{
		
		pugi::xml_attribute pos = patch.attribute("pos");
		if (!pos || strlen(pos.value()) <= 0 || strcmp(pos.value(), "append") == 0)
		{
			pugi::xml_node::iterator start = patch.begin();
			pugi::xml_node::iterator end = patch.end();

			 
			if (CombineText(patch.first_child(), original.node().last_child(), false))
				start++;

			for (; start != end; start++)
				original.node().append_copy(*start);
		}
		else if (strcmp(pos.value(), "prepend") == 0)
		{
			pugi::xml_node::iterator start = patch.begin();
			pugi::xml_node::iterator end = patch.end();

		 
			if (CombineText(patch.last_child(), original.node().first_child(), true))
				end--;

			pugi::xml_node pos = original.node().first_child();
			for (; start != end; start++)
				original.node().insert_copy_before(*start, pos);
		}
		else if (strcmp(pos.value(), "before") == 0)
		{
			pugi::xml_node::iterator start = patch.begin();
			pugi::xml_node::iterator end = patch.end();

			 
			if (CombineText(patch.first_child(), original.node().previous_sibling(), false))
				start++;

			 
			if (CombineText(patch.last_child(), original.node(), true))
				end--;

			for (; start != end; start++)
				original.parent().insert_copy_before(*start, original.node());
		}
		else if (strcmp(pos.value(), "after") == 0)
		{
			pugi::xml_node::iterator start = patch.begin();
			pugi::xml_node::iterator end = patch.end();

			 
			if (CombineText(patch.first_child(), original.node(), false))
				start++;

			 
			if (CombineText(patch.last_child(), original.node().next_sibling(), true))
				end--;

			pugi::xml_node pos = original.node();
			for (; start != end; start++)
				pos = original.parent().insert_copy_after(*start, pos);
		}
	}

	void XMLFile::AddAttribute(const pugi::xml_node& patch, const pugi::xpath_node& original) const
	{
		pugi::xml_attribute attribute = patch.attribute("type");

		if (!patch.first_child() && patch.first_child().type() != pugi::node_pcdata)
		{
			SAPPHIRE_LOGERROR("XML Patch failed calling Add due to attempting to add non text to an attribute for %s.", attribute.value());
			return;
		}

		String name(attribute.value());
		name = name.GetSubString(1,name.Length());

		pugi::xml_attribute newAttribute = original.node().append_attribute(name.c_str());
		newAttribute.set_value(patch.child_value());
	}

	bool XMLFile::CombineText(const pugi::xml_node& patch, const pugi::xml_node& original, bool prepend) const
	{
		if (!patch || !original)
			return false;

		if ((patch.type() == pugi::node_pcdata && original.type() == pugi::node_pcdata) ||
			(patch.type() == pugi::node_cdata && original.type() == pugi::node_cdata))
		{
			if (prepend)
				const_cast<pugi::xml_node&>(original).set_value(Sapphire::ToString("%s%s", patch.value(), original.value()).c_str());
			else
				const_cast<pugi::xml_node&>(original).set_value(Sapphire::ToString("%s%s", original.value(), patch.value()).c_str());

			return true;
		}

		return false;
	}

}
