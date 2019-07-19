#include "Sapphire.h"
#include "FileStream.h"
#include "Graphics.h"
#include "GLShader.h"
#include "ShaderMgr.h"
#include "GLGraphicDriver.h"
#include <GraphicException.h>
#include "json/json.h"


namespace Sapphire
{

	
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
		StringHash strHash(defines);
		std::unordered_map<uint, SharedPtr<GLShaderVariation>>::iterator it;
		//判断类型
		switch (type)
		{
		case Sapphire::VS:
			{
				it = m_vsVariation.find(strHash.Value());
				 if (it != m_vsVariation.end())
				 {
					 pShaderVariation = it->second;
				 }
				 else
				 {
					 std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver)));
					_pair.second->SetDefines(defines);
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(),ShaderType::VS));
					m_vsVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
					
				 }
			}
			break;
		case Sapphire::PS:
			{
				it = m_psVariation.find(strHash.Value());
				if (it != m_psVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type,m_pCore, m_pGraphicDriver)));
					_pair.second->SetDefines(defines);
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(), ShaderType::PS));
					m_psVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
				}
			}
			break;
		case Sapphire::GS:
			{
				it = m_gsVariation.find(strHash.Value());
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver)));
					_pair.second->SetDefines(defines);
					_pair.second->SetName(ShaderMgr::GetFileName(GetName(), ShaderType::GS));
					m_gsVariation.insert(_pair);
					RefreshMemoryUse();
					++m_numVariation;
				}
			}
			break;
		case Sapphire::CS:
			{
				it = m_gsVariation.find(strHash.Value());
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore, m_pGraphicDriver)));
					_pair.second->SetDefines(defines);
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
		m_bIsDisposed = true;
	}

	size_t GLShader::GetSize()
	{
		return 0;
	}

	bool GLShader::IsDisposed()
	{
		return m_bIsDisposed;
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
		m_eState = ResourceState_Loading;	
	}

	void GLShader::OnLoadEnd()
	{
		m_eState = ResourceState_Unload;
	}

	void GLShader::OnLoadError()
	{
		m_eState = ResourceState_Loaded;
	}

	bool GLShader::LoadShaderScript(const char* scriptPath, ShaderType type)
	{
		std::string source = "";
		FileStream fs(scriptPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (!fs.IsOpen())
		{
			return false;
		}
		source = fs.ReadString(MAX_SHADER_SCRIPT_LENGTH);
		fs.Release();
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

	bool GLShader::ProcessSource(std::string source)
	{
		return false;
	}

	std::string GLShader::NormalizeDefines(const std::string& defines)
	{
		return "";
	}

	void GLShader::RefreshMemoryUse()
	{

	}

}