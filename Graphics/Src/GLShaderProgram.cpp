#include "Graphics.h"
#include "GLShaderProgram.h"



namespace Sapphire
{

	GLShaderProgram::GLShaderProgram()
	{

	}

	GLShaderProgram::~GLShaderProgram()
	{

	}

	void GLShaderProgram::Release()
	{

	}

	void GLShaderProgram::OnDeviceLost()
	{

	}

	bool GLShaderProgram::Link()
	{
		return false;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetVertexShader() const
	{
		return NULL;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetPixelShader() const
	{
		return NULL;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetGeometryShader() const
	{
		return NULL;
	}

	bool GLShaderProgram::HasParameter(std::string param) const
	{
		return false;
	}

	const Sapphire::ShaderParameter* GLShaderProgram::GetParameter(std::string param) const
	{
		return NULL;
	}

	bool GLShaderProgram::NeedParameterUpdate(ShaderParameterGroup group, const void* source)
	{
		return false;
	}

	void GLShaderProgram::ClearParameterSource(ShaderParameterGroup group)
	{

	}

	void GLShaderProgram::ClearParameterSources()
	{

	}

	void GLShaderProgram::ClearGlobalParameterSource(ShaderParameterGroup group)
	{

	}

	ulong GLShaderProgram::g_uFrameNumber;

}

