#include "Sapphire.h"
#include "FileStream.h"
#include "ResourceCache.h"
#include "Graphics.h"
#include "GLShader.h"
#include "ShaderMgr.h"
#include <algorithm>
#include "GLGraphicDriver.h"
#include <GraphicException.h>
#include "json/json.h"


namespace Sapphire
{
	bool CompareDefines(const String& lhs, const String& rhs)
	{
		return lhs.ToHash() < rhs.ToHash();
	}
	
	GLShader::GLShader(Core* pCore, const char* name) :
		BaseResource(pCore,name),
		m_timeStamp(0),
		m_numVariation(0),
		m_bIsDisposed(true),
		m_name(name)
	{
		m_eType = ResourceType_Shader;
		m_eState = ResourceState_Unload;
	}

	GLShader::~GLShader()
	{
		Dispose();
	}

	Sapphire::IShaderVariation* GLShader::GetVariation(ShaderType type, const std::string& defines)
	{
		return GetVariation(type, defines);
	}

	Sapphire::IShaderVariation* GLShader::GetVariation(ShaderType type, const char* defines)
	{
		IShaderVariation* pShaderVariation = NULL;
		String normalizeDefines = NormalizeDefines(defines); //先标准化一下
		StringHash strHash(normalizeDefines);
		SHADERVARIATION_MAP_ITERATOR it;
		//判断类型
		switch (type)
		{
		case Sapphire::VS:
			{
				it = m_vsVariation.find(strHash);
				 if (it != m_vsVariation.end())
				 {
					 pShaderVariation = it->second;
				 }
				 else
				 {
					 std::pair<StringHash, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash, SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver,this)));
					 _pair.second->SetDefines(normalizeDefines.str());
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(),ShaderType::VS));
					m_vsVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
					
				 }
			}
			break;
		case Sapphire::PS:
			{
				it = m_psVariation.find(strHash);
				if (it != m_psVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<StringHash, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash, SharedPtr<GLShaderVariation>(new GLShaderVariation(type,m_pCore, m_pGraphicDriver,this)));
					_pair.second->SetDefines(normalizeDefines.str());
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(), ShaderType::PS));
					m_psVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
				}
			}
			break;
		case Sapphire::GS:
			{
				it = m_gsVariation.find(strHash);
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<StringHash, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash, SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver,this)));
					_pair.second->SetDefines(normalizeDefines.str());
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(), ShaderType::GS));
					m_gsVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
				}
			}
			break;
		case Sapphire::CS:
			{
				it = m_gsVariation.find(strHash);
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<StringHash, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash, SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver,this)));
					_pair.second->SetDefines(normalizeDefines.str());
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(), ShaderType::CS));
					m_csVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
				}
			}
			break;
		default:
			break;
		}
		return pShaderVariation;
	}

	const std::string& GLShader::GetSourceCode(ShaderType type) const
	{
		switch (type)
		{
		case Sapphire::VS:
			return m_vsSource;
		case Sapphire::PS:
			return m_psSource;
		case Sapphire::GS:
			return m_gsSource;
		case Sapphire::CS:
			return m_csSource;
		default:
			break;
		}
		return "";
	}

	bool GLShader::Create()
	{
		if (IsDisposed())
		{
			return Load();
		}
		return false;
	}

	bool GLShader::Recreate()
	{
		if (!IsDisposed())
		{
			Dispose();
		}
		return Load();
	}

	void GLShader::Dispose()
	{
		if (!m_bIsDisposed)
		{
			m_bIsDisposed = true;
			Release();
		}
		
		
	}

	size_t GLShader::GetSize()
	{
		return 0;
	}

	bool GLShader::IsDisposed()
	{
		return m_bIsDisposed;
	}


	void GLShader::Release()
	{

	}

	bool GLShader::Load(HSHADERSCRIPT hscript)
	{
		//ShaderScriptMgr*   pScriptMgr = m_pGraphicDriver->getShaderScriptMgr();
		//Core* pCore = Core::GetSingletonPtr();
		//if (pScriptMgr == NULL || pCore == NULL)
		//{
		//	throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
		//}
		//if (hscript.IsNull())
		//{
		//	LogUtil::LogMsgLn("load Shader script failed! handle is null");
		//	return false;
		//}
		//std::string name = pScriptMgr->GetName(hscript);	//脚本路径
		//std::string content = pScriptMgr->GetContent(hscript);    //脚本内容
		//ShaderType type = pScriptMgr->GetType(hscript);			//脚本类型
		//if (pScriptMgr->GetType(hscript) != UNKNOWN)
		//{
		//switch (type)
		//{
		//case Sapphire::VS:
		//	m_vsSource = content;
		//break;
		//case Sapphire::PS:
		//	m_psSource = content;
		//break;
		//case Sapphire::GS:
		//	m_gsSource = content;
		//break;
		//case Sapphire::CS:
		//	m_csSource = content;
		//break;
		//case Sapphire::UNKNOWN:
		//break;
		//default:
		//break;
		//}
		//}
		////++m_numVariation;
		//RefreshMemoryUse();
		return true;

	}

	bool GLShader::Load()
	{
		//加载资源名	 
		FileStream fs(m_resName.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			bool ret = false;
			std::string source;
			//ret = ProcessSource(source, fs); //解析shader源码

			//解析json
			std::string jsonStr = fs.ReadString(MAX_JSON_LENGTH);
			fs.Release();
			Json::CharReaderBuilder builder;
			Json::CharReader* reader = builder.newCharReader();
			JSONCPP_STRING errs;
			Json::Value rootNode;
			if (reader->parse(jsonStr.c_str(), jsonStr.c_str() + strlen(jsonStr.c_str()), &rootNode, &errs))
			{
				//从配置文件中读取每个shader的路径
				//四种shader
				Path vsFile = rootNode["vertexShader"]["name"].asCString();
				LoadShaderScript(vsFile.c_str(), ShaderType::VS);
				Path psFile = rootNode["pixelShader"]["name"].asCString();
				LoadShaderScript(psFile.c_str(), ShaderType::PS);
				Path gsFile = rootNode["geometryShader"]["name"].asCString();
				LoadShaderScript(gsFile.c_str(), ShaderType::GS);
				Path csFile = rootNode["computeShader"]["name"].asCString();
				LoadShaderScript(csFile.c_str(), ShaderType::CS);
				m_bIsDisposed = false;
				return true;
			}
		}
		return false;
	}

	bool GLShader::Load(const char* resourcePath)
	{
		m_resName = resourcePath;
		return Load();
	}

	void GLShader::Clear()
	{
		 
	}

	void GLShader::Destroy()
	{
		 
	}

	void GLShader::OnLoadStart()
	{
		/*m_eState = ResourceState_Loading;
		ResourceCache* cache = dynamic_cast<ResourceCache*>(m_pCore->GetSubSystemWithType(ESST_RESOURCECACHE));
		if (cache)
		{
			cache->InsertResource(m_resName.c_str(), this);
		}*/
		BaseResource::OnLoadStart();
	}

	void GLShader::OnLoadEnd()
	{
		//m_eState = ResourceState_Loaded;		
		BaseResource::OnLoadEnd();
	}

	void GLShader::OnLoadError()
	{
		//m_eState = ResourceState_Loaded;
		BaseResource::OnLoadError();
	}

	bool GLShader::LoadShaderScript(const char* scriptPath, ShaderType type)
	{
		std::string source = "";
		FileStream fs(scriptPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (!fs.IsOpen())
		{
			return false;
		}
		bool ret = ProcessSource(source, fs);
		fs.Release();
		if (!ret)
			return false;
		switch (type)
		{
		case Sapphire::VS:
			m_vsSource = source;
			break;
		case Sapphire::PS:
			m_psSource = source;
			break;
		case Sapphire::GS:
			m_gsSource = source;
			break;
		case Sapphire::CS:
			m_csSource = source;
			break;
		case Sapphire::UNKNOWN:
			break;
		default:
			break;
		}
		RefreshMemoryUse();
		return true;
	}

	

	bool GLShader::ProcessSource(std::string& source, FileStream& istream)
	{
		ResourceCache* cache = m_pCore->GetSubSystem<ResourceCache>();
		std::string line;
		bool ret = istream.ReadLine(line);
		while (ret)
		{
			if (line.find("#include") == 0)
			{
				//处理包含文件
				Path curPath = istream.GetPath();
				curPath = curPath.getParentDir();
				curPath.addTailSlash();
				std::string fileName = line.substr(9, line.length() - 10);
				curPath += fileName;
				FileStream fs(curPath.c_str(),FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
				if (fs.IsOpen())
				{
					ProcessSource(source, fs); //递归处理
					fs.Release();
				}
				else
				{
					return false;
				}
				source += line;
				source += "\n";
			}
			ret = istream.ReadLine(line);
		}
		return true;
	}

	std::string GLShader::NormalizeDefines(const std::string& defines)
	{
		String _defines = defines;
		_defines = _defines.ToUpper();
		std::vector<String> definesVec = String::Split(_defines.c_str(),' ');
		if (definesVec.size() == 0)
			return "";
		std::sort(definesVec.begin(), definesVec.end(), CompareDefines);
		std::string ret = definesVec[0].str();
		for (int i = 1; i < definesVec.size(); ++i)
		{
			ret = ret + " " + definesVec[i].str();
		}
		return ret;
	}

	void GLShader::RefreshMemoryUse()
	{

	}

}