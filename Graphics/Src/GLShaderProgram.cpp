#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "GLShaderProgram.h"



namespace Sapphire
{

	GLShaderProgram::GLShaderProgram(Core* pCore):
		BaseObject(pCore),
		m_pDriver(NULL)
	{
		m_pDriver = (GLGraphicDriver*)(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
	}

	GLShaderProgram::~GLShaderProgram()
	{
		Release();
	}

	void GLShaderProgram::Release()
	{
		if (m_uHwUID)
		{
			if (!m_pDriver)
				return;

			if (!m_pDriver->IsDeviceLost())
			{
				if (m_pDriver->GetShaderProgram() == this)
					m_pDriver->SetShaders(0, 0);

				glDeleteProgram(m_uHwUID);
			}

			m_uHwUID = 0;
			m_linkOutMsg.clear();  //清除错误输出
			m_shaderParamters.clear();   //清除shader参数

			//清理缓冲区
			for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
				m_bUseTextureUnits[i] = false;
			for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
				m_constantBuffers[i].Reset();
		}
	}

	void GLShaderProgram::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();

		if (m_pDriver && m_pDriver->GetShaderProgram() == this)
			m_pDriver->SetShaders(0, 0);  //重置shader为空

		m_linkOutMsg.clear();
	}

	bool GLShaderProgram::Link()
	{
		Release();//释放之前的
		if (!m_vertexShader || !m_pixelShader || !m_vertexShader->GetGPUHandle() || !m_pixelShader->GetGPUHandle())
			return false;

		m_uHwUID = glCreateProgram();
		if (!m_uHwUID)
		{
			m_linkOutMsg = "Could not create shader program";
			return false;
		}
		// 在所有的shader绑定相同的vertex attribute locations,公用
		// 注意这和在VertexBuffer中顺序不同，需要重新映射（除了cube texture）
		// 为了更好兼容GLES2设备，要配合前面8个坐标
		glBindAttribLocation(m_uHwUID, 0, "iPos");
		glBindAttribLocation(m_uHwUID, 1, "iNormal");
		glBindAttribLocation(m_uHwUID, 2, "iColor");
		glBindAttribLocation(m_uHwUID, 3, "iTexCoord");
		glBindAttribLocation(m_uHwUID, 4, "iTexCoord2");
		glBindAttribLocation(m_uHwUID, 5, "iTangent");
		glBindAttribLocation(m_uHwUID, 6, "iBlendWeights");
		glBindAttribLocation(m_uHwUID, 7, "iBlendIndices");
		glBindAttribLocation(m_uHwUID, 8, "iCubeTexCoord");
		glBindAttribLocation(m_uHwUID, 9, "iCubeTexCoord2");
#if !defined(GL_ES_VERSION_2_0) || defined(__EMSCRIPTEN__)
		glBindAttribLocation(m_uHwUID, 10, "iInstanceMatrix1");
		glBindAttribLocation(m_uHwUID, 11, "iInstanceMatrix2");
		glBindAttribLocation(m_uHwUID, 12, "iInstanceMatrix3");
#endif
		glAttachShader(m_uHwUID, m_vertexShader->GetGPUHandle());
		glAttachShader(m_uHwUID, m_pixelShader->GetGPUHandle());
		glLinkProgram(m_uHwUID);

		return true;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetVertexShader() const
	{
		return m_vertexShader;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetPixelShader() const
	{
		return m_pixelShader;
	}

	Sapphire::IShaderVariation* GLShaderProgram::GetGeometryShader() const
	{
		return m_geometryShader;
	}

	bool GLShaderProgram::HasParameter(std::string param) const
	{
		if(m_shaderParamters.find(param) != m_shaderParamters.end())
			return true;
		return false;
	}

	const Sapphire::ShaderParameter* GLShaderProgram::GetParameter(std::string param) const
	{
		std::unordered_map<std::string, ShaderParameter*>::const_iterator it  = m_shaderParamters.find(param);
		if (it != m_shaderParamters.end())
			return it->second;
		return NULL;
	}

	bool GLShaderProgram::NeedParameterUpdate(ShaderParameterGroup group, const void* source)
	{
		//如果全局帧号被修改，无效化所有的shader参数组
		if (g_uFrameNumber != m_uFrameNumber)
		{
			for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
				m_parameterSources[i] = (const void*)M_MAX_UNSIGNED;
			m_uFrameNumber = g_uFrameNumber;
		}

#ifndef GL_ES_VERSION_2_0
		bool useBuffer = m_constantBuffers[group].Get() || m_constantBuffers[group + MAX_SHADER_PARAMETER_GROUPS].Get();
		bool useIndividual = !m_constantBuffers[group].Get() || !m_constantBuffers[group + MAX_SHADER_PARAMETER_GROUPS].Get();
		bool needUpdate = false;  //参数已变，需要更新

		//同样的就不用更新了
		if (useBuffer && g_parameterSources[group] != source)
		{
			g_parameterSources[group] = source; //更新全局参数源
			needUpdate = true; 
		}

		if (useIndividual && m_parameterSources[group] != source)
		{
			m_parameterSources[group] = source;
			needUpdate = true;
		}

		return needUpdate;
#else
		if (m_parameterSources[group] != source)
		{
			m_parameterSources[group] = source;
			return true;
		}
		else
			return false;
#endif
	}

	void GLShaderProgram::ClearParameterSource(ShaderParameterGroup group)
	{
		 
#ifndef GL_ES_VERSION_2_0
		bool useBuffer = m_constantBuffers[group].Get() || m_constantBuffers[group + MAX_SHADER_PARAMETER_GROUPS].Get();
		bool useIndividual = !m_constantBuffers[group].Get() || !m_constantBuffers[group + MAX_SHADER_PARAMETER_GROUPS].Get();

		if (useBuffer)
			g_parameterSources[group] = (const void*)M_MAX_UNSIGNED;
		if (useIndividual)
			m_parameterSources[group] = (const void*)M_MAX_UNSIGNED;
#else
		m_parameterSources[group] = (const void*)M_MAX_UNSIGNED;
#endif
	}

	void GLShaderProgram::ClearParameterSources()
	{
		++g_uFrameNumber;
		if (!g_uFrameNumber)
			++g_uFrameNumber;

#ifndef GL_ES_VERSION_2_0
		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			g_parameterSources[i] = (const void*)M_MAX_UNSIGNED;
#endif
	}

	void GLShaderProgram::ClearGlobalParameterSource(ShaderParameterGroup group)
	{
		g_parameterSources[group] = (const void*)M_MAX_UNSIGNED;
	}

	ulong GLShaderProgram::g_uFrameNumber;

}

