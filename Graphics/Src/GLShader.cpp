#include "Graphics.h"
#include "GLShader.h"


namespace Sapphire
{

	GLShader::GLShader()
	{

	}

	GLShader::~GLShader()
	{

	}

	Sapphire::IShaderVariation* GLShader::GetVariation(ShaderType type, const std::string& defines)
	{
		return NULL;
	}

	Sapphire::IShaderVariation* GLShader::GetVariation(ShaderType type, const char* defines)
	{
		return NULL;
	}

	const std::string& GLShader::GetSourceCode(ShaderType type) const
	{
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
		throw std::logic_error("The method or operation is not implemented.");
	}

	size_t GLShader::GetSize()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLShader::IsDisposed()
	{
		throw std::logic_error("The method or operation is not implemented.");
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