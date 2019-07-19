#include "GLShader.h"
#include "GLGraphicDriver.h"
#include "GLShaderVariation.h"



namespace Sapphire
{

	GLShaderVariation::GLShaderVariation(ShaderType type, Core* pCore, IGraphicDriver* pDriver) :
		BaseObject(pCore),
		m_eType(type),
		GPUObject(pDriver)
	{	
		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(pCore);
	}

	GLShaderVariation::~GLShaderVariation()
	{
		Release();
	}
	
	bool GLShaderVariation::Create()
	{
		Release();
		if (!m_owner)
		{
			m_compilerOutput = "Owner shader has expired";
			return false;
		}
		m_uHwUID = glCreateShader(m_eType == VS ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		if (!m_uHwUID)
		{
			m_compilerOutput = "Could not create shader object";
			return false;
		}

		const std::string originalShaderCode = m_owner->GetSourceCode(m_eType);  //获取相对应shader源码
		const char* shaderCodeCstr = originalShaderCode.c_str();
		
		glShaderSource(m_uHwUID, 1, &shaderCodeCstr, 0);
		glCompileShader(m_uHwUID);

		int compiled, length;
		glGetShaderiv(m_uHwUID, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			glGetShaderiv(m_uHwUID, GL_INFO_LOG_LENGTH, &length);
			int outLength;
			m_compilerOutput.resize(length);
			glGetShaderInfoLog(m_uHwUID, length, &outLength, &m_compilerOutput[0]);
			glDeleteShader(m_uHwUID);
			m_uHwUID = 0;
		}
		else
		{
			m_compilerOutput.clear();
		}






		return true;
	}

	void GLShaderVariation::SetName(const std::string& name)
	{
		m_name = name;
	}

	void GLShaderVariation::SetDefines(const std::string& defines)
	{
		m_defines = defines;
	}

	Sapphire::IShader* GLShaderVariation::GetOwner() const
	{
		return m_owner;
	}

	const std::string& GLShaderVariation::GetName() const
	{
		return m_name;
	}

	const std::string& GLShaderVariation::GetDefines() const
	{
		return m_defines;
	}

	const std::string& GLShaderVariation::GetCompilerOutput() const
	{
		return m_compilerOutput;
	}

	std::string GLShaderVariation::GetFullName() const
	{
		return m_name + "(" + m_defines + ")";
	}

	void GLShaderVariation::Release()
	{
		if (m_uHwUID)
		{
			if (!m_pGraphicDriver)
				return;

			if (!m_pGraphicDriver->IsDeviceLost())
			{
				if (m_eType == VS)
				{
					if (m_pGraphicDriver->GetVertexShader() == this)
						m_pGraphicDriver->SetShaders(0, 0);
				}
				else
				{
					if (m_pGraphicDriver->GetPixelShader() == this)
						m_pGraphicDriver->SetShaders(0, 0);
				}

				glDeleteShader(m_uHwUID);
			}

			m_uHwUID = 0;
			m_pGraphicDriver->CleanupShaderPrograms(this);
		}

		 
	}

	void GLShaderVariation::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
		m_compilerOutput.clear();
	}

}