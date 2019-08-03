#include "GLShader.h"
#include "GLGraphicDriver.h"
#include "GLShaderVariation.h"
#include "stringHelper.h"



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

	GLShaderVariation::GLShaderVariation(ShaderType type, Core* pCore, IGraphicDriver* pDriver, GLShader* owner):
		BaseObject(pCore),
		m_eType(type),
		GPUObject(pDriver),
		m_owner(owner)
	{

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

		const std::string& originalShaderCode = m_owner->GetSourceCode(m_eType);  //获取相对应shader源码
		//预编译
			
		std::string _shaderCode = Precompile(originalShaderCode);
		const char* _shaderCodeCstr = _shaderCode.c_str();

		
		glShaderSource(m_uHwUID, 1, &_shaderCodeCstr, 0);
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
		if (!m_uHwUID)
			return false;

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

	void GLShaderVariation::SetOwner(GLShader* owner)
	{
		m_owner = owner;
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

	std::string GLShaderVariation::Precompile(const std::string& sourceCode)
	{
		String precompilieCode;
		String shaderCode = sourceCode;
		uint   verStart = shaderCode.Find("#",0);
		uint   verEnd = 0;

		//////////////提取版本声明///////////////////////
		if (verStart != String::NPOS)
		{
			verEnd = verStart + 9;
			while (verEnd < shaderCode.Length())
			{
				if (IsDigital((unsigned)shaderCode[verEnd]))
					++verEnd;
				else
					break;
			}
			String versionDefine = shaderCode.GetSubString(verStart, verEnd - verStart);
			precompilieCode += versionDefine;
			precompilieCode += +"\n";
		}
		if (!verEnd && GLGraphicDriver::GetGL3Support())
			precompilieCode += "#version 300\n";

		/////声明最大骨骼数///////////
		precompilieCode += "#define MAXBONES ";
		precompilieCode += String(GLGraphicDriver::GetMaxBones());
		precompilieCode += "\n";

		//////////加上预定义/////////
		std::vector<String> vec = String::Split(m_defines.c_str(), ' ');
		for (int i = 0; i < vec.size(); ++i)
		{
			String defineString = "#define " + vec[i].ReplaceSubString("=", " ");
			defineString += " \n";
			precompilieCode += defineString;
		}

		if (GLGraphicDriver::GetGL3Support())
			precompilieCode += "#define GL3\n";

		//如果版本定义找到，不要再重复插入
		if (verEnd > 0)
			shaderCode += (shaderCode.c_str() + verEnd);
		else
			shaderCode += shaderCode;

		return precompilieCode.c_str();
	}

}