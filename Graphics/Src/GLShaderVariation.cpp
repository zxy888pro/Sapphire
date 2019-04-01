#include "GLShader.h"
#include "GLGraphicDriver.h"
#include "GLShaderVariation.h"



namespace Sapphire
{

	GLShaderVariation::GLShaderVariation(ShaderType type):
		m_eType(type)
	{
		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(ESST_GRAPHICDRIVER));
	}

	GLShaderVariation::~GLShaderVariation()
	{

	}
	
	bool GLShaderVariation::Create()
	{
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
	}

}