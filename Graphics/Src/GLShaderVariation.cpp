#include "GLShader.h"
#include "GLShaderVariation.h"



namespace Sapphire
{

	GLShaderVariation::GLShaderVariation(ShaderType type):
		m_eType(type)
	{

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
		return "";
	}

	void GLShaderVariation::Release()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLShaderVariation::OnDeviceLost()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}