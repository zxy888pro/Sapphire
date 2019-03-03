#include "Graphics.h"
#include "GLShader.h"
#include "ShaderMgr.h"

namespace Sapphire
{

	GLShader::GLShader():
		m_timeStamp(0),
		m_numVariation(0),
		m_bIsDisposed(false)
	{
		RefreshMemoryUse();
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
		std::string str = defines;
		std::unordered_map<std::string, SharedPtr<GLShaderVariation>>::iterator it;
		switch (type)
		{
		case Sapphire::VS:
			{
				 it = m_vsVariation.find(str);
				 if (it != m_vsVariation.end())
				 {
					 pShaderVariation = it->second;
				 }
				 else
				 {
					std::pair<std::string, SharedPtr<GLShaderVariation>> _pair = std::make_pair(str, SharedPtr<GLShaderVariation>(new GLShaderVariation(type)));
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
				it = m_psVariation.find(str);
				if (it != m_psVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<std::string, SharedPtr<GLShaderVariation>> _pair = std::make_pair(str, SharedPtr<GLShaderVariation>(new GLShaderVariation(type)));
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
				it = m_gsVariation.find(str);
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<std::string, SharedPtr<GLShaderVariation>> _pair = std::make_pair(str, SharedPtr<GLShaderVariation>(new GLShaderVariation(type)));
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
				it = m_gsVariation.find(str);
				if (it != m_gsVariation.end())
				{
					pShaderVariation = it->second;
				}
				else
				{
					std::pair<std::string, SharedPtr<GLShaderVariation>> _pair = std::make_pair(str, SharedPtr<GLShaderVariation>(new GLShaderVariation(type)));
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


	bool GLShader::Load(HSHADERSCRIPT shaderPath)
	{

	}

	bool GLShader::ProcessSource(std::string source)
	{
		return false;
	}

	std::string GLShader::NormalizeDefines(const std::string& defines)
	{
		return "";
	}

}