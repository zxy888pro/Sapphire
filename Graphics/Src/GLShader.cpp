#include "Graphics.h"
#include "GLShader.h"
#include "ShaderMgr.h"
#include "GLGraphicDriver.h"
#include <GraphicException.h>
#include "ShaderScriptMgr.h"

namespace Sapphire
{

	
	GLShader::GLShader(Core* pCore, const char* name) :
		BaseResource(pCore,name),
		m_timeStamp(0),
		m_numVariation(0),
		m_bIsDisposed(false),
		m_name(name)
	{
		
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
					 std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore)));
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
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore)));
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
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore)));
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
					std::pair<uint, SharedPtr<GLShaderVariation>> _pair = std::make_pair(strHash.Value(), SharedPtr<GLShaderVariation>(new GLShaderVariation(type, m_pCore)));
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
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLShader::Recreate()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLShader::Dispose()
	{
		m_bIsDisposed = true;
	}

	size_t GLShader::GetSize()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLShader::IsDisposed()
	{
		return m_bIsDisposed;
	}


	bool GLShader::Load(HSHADERSCRIPT hscript)
	{
		ShaderScriptMgr*   pScriptMgr = m_pGraphicDriver->getShaderScriptMgr();
		Core* pCore = Core::GetSingletonPtr();
		if (pScriptMgr == NULL || pCore == NULL)
		{
			throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
		}
		if (hscript.IsNull())
		{
			LogUtil::LogMsgLn("load Shader script failed! handle is null");
			return false;
		}
		std::string name = pScriptMgr->GetName(hscript);	//脚本路径
		std::string content = pScriptMgr->GetContent(hscript);    //脚本内容
		ShaderType type = pScriptMgr->GetType(hscript);			//脚本类型
		if (pScriptMgr->GetType(hscript) != UNKNOWN)
		{
		switch (type)
		{
		case Sapphire::VS:
			m_vsSource = content;
		break;
		case Sapphire::PS:
			m_psSource = content;
		break;
		case Sapphire::GS:
			m_gsSource = content;
		break;
		case Sapphire::CS:
			m_csSource = content;
		break;
		case Sapphire::UNKNOWN:
		break;
		default:
		break;
		}
		}
		//++m_numVariation;
		RefreshMemoryUse();

	}

	void GLShader::Load()
	{
		 
	}

	bool GLShader::Load(const char* resourcePath)
	{
		return false;
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