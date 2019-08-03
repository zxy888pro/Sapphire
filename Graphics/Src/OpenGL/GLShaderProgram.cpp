#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "GLShaderProgram.h"
#include "stringHelper.h"



namespace Sapphire
{
	const char* shaderParameterGroups[] = {
		"frame",
		"camera",
		"zone",
		"light",
		"material",
		"object",
		"custom"
	};
	const void* GLShaderProgram::g_parameterSources[MAX_SHADER_PARAMETER_GROUPS];


	GLShaderProgram::GLShaderProgram(Core* pCore, IGraphicDriver* pDriver, GLShaderVariation* vertexShader, GLShaderVariation* pixelShader) :
		BaseObject(pCore),
		GPUObject(pDriver),
		m_uFrameNumber(0),
		m_vertexShader(vertexShader),
		m_pixelShader(pixelShader),
		m_pDriver(NULL)
	{
		m_pDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(m_pDriver);
		for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
			m_bUseTextureUnits[i] = false;
		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			m_parameterSources[i] = (const void*)M_MAX_UNSIGNED;
	}

	GLShaderProgram::GLShaderProgram(Core* pCore, IGraphicDriver* pDriver, GLShaderVariation* vertexShader, GLShaderVariation* pixelShader, GLShaderVariation* geometryShader, GLShaderVariation* computeShader):
		BaseObject(pCore),
		GPUObject(pDriver),
		m_uFrameNumber(0),
		m_vertexShader(vertexShader),
		m_pixelShader(pixelShader),
		m_pDriver(NULL),
		m_computeShader(computeShader),
		m_geometryShader(geometryShader)
	{
		m_pDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(m_pDriver);
		for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
			m_bUseTextureUnits[i] = false;
		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS; ++i)
			m_parameterSources[i] = (const void*)M_MAX_UNSIGNED;
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

		/////////////////////////////////////////////检查链接状态///////////////////////////////
		int linked, length;
		glGetProgramiv(m_uHwUID, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			//链接错误
			glGetProgramiv(m_uHwUID, GL_INFO_LOG_LENGTH, &length);
			int outLength;
			m_linkOutMsg.resize(length);
			glGetProgramInfoLog(m_uHwUID, length, &outLength, &m_linkOutMsg[0]);
			glDeleteProgram(m_uHwUID);
			m_uHwUID = 0;
		}
		else
			m_linkOutMsg.clear();

		if (!m_uHwUID)
			return false;




		const int MAX_PARAMETER_NAME_LENGTH = 256;
		char uniformName[MAX_PARAMETER_NAME_LENGTH];  //保存uniform常量名的缓冲区
		int uniformCount;

		glUseProgram(m_uHwUID);
		glGetProgramiv(m_uHwUID, GL_ACTIVE_UNIFORMS, &uniformCount);

		/////////////////////////////// 检查 constant buffers//////////////////////////////////
#ifndef GL_ES_VERSION_2_0
		//OpenGL 3.0以上使用UBO
		std::unordered_map<unsigned, unsigned> blockToBinding;
		if (GLGraphicDriver::GetGL3Support())
		{
			int numUniformBlocks = 0;

			glGetProgramiv(m_uHwUID, GL_ACTIVE_UNIFORM_BLOCKS, &numUniformBlocks); //活动的uniform常量数量
			for (int i = 0; i < numUniformBlocks; ++i)
			{
				int nameLength;
				glGetActiveUniformBlockName(m_uHwUID, (GLuint)i, MAX_PARAMETER_NAME_LENGTH, &nameLength, uniformName);  //获取活动的uniformBlack名字

				std::string name = uniformName;

				unsigned blockIndex = glGetUniformBlockIndex(m_uHwUID, name.c_str());
				unsigned group = M_MAX_UNSIGNED;

				//确认用到的buffer名
				for (unsigned j = 0; j < MAX_SHADER_PARAMETER_GROUPS; ++j)
				{
					if (StringFindNoCase(name, shaderParameterGroups[j]) != std::string::npos)
					{
						group = j;
						break;
					}
				}

				// 如果没有这个名字,搜索这个名字中的数字，用它做group index
				if (group == M_MAX_UNSIGNED)
				{
					for (unsigned j = 1; j < name.length(); ++j)
					{
						if (name[j] >= '0' && name[j] <= '5')
						{
							group = name[j] - '0';
							break;
						}
					}
				}

				if (group >= MAX_SHADER_PARAMETER_GROUPS)
				{
					SAPPHIRE_LOGWARNING(StringFormat("Skipping unrecognized uniform block %s in shader program  %s  %s", name.c_str(), m_vertexShader->GetName().c_str(), m_pixelShader->GetName().c_str()));
					continue;
				}


				// 查找constant buffer 数据块大小
				int dataSize;
				glGetActiveUniformBlockiv(m_uHwUID, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &dataSize);
				if (!dataSize)
					continue;

				unsigned bindingIndex = group; //用重新分配的组号做绑定索引
				//Vertex Shader和Pixel Shader分别占用的不同的独自的绑点索引,Pixel Shader的索引从Vertex Shader的尾部也就是MAX_SHADER_PARAMETER_GROUPS开始
				if (StringFindNoCase(name, "PS") != std::string::npos)
					bindingIndex += MAX_SHADER_PARAMETER_GROUPS;

				glUniformBlockBinding(m_uHwUID, blockIndex, bindingIndex); //绑定UBO对象到绑点bindingIndex
				blockToBinding[blockIndex] = bindingIndex;

				m_constantBuffers[bindingIndex] = m_pDriver->GetOrCreateConstantBuffer(bindingIndex, (unsigned)dataSize); //获取constant buffer， 没有则创建
			}
		}


#else


#endif

		// 挨个检查shader 参数和纹理单元
		for (int i = 0; i < uniformCount; ++i)
		{
			unsigned type;
			int count;

			//取得参数名
			glGetActiveUniform(m_uHwUID, (GLuint)i, MAX_PARAMETER_NAME_LENGTH, 0, &count, &type, uniformName);
			int location = glGetUniformLocation(m_uHwUID, uniformName);

			//检查包含此名字的数组索引，并且剔除它
			std::string name(uniformName);
			unsigned index = name.find('[');
			if (index != std::string::npos)
			{
				//如果不是第一个，跳过
				if (name.find("[0]", index) == std::string::npos)
					continue;

				name = name.substr(0, index);
			}

			if (name[0] == 'c')
			{
				//保存uniform常量
				std::string paramName = name.substr(1);
				ShaderParameter* newParam = new ShaderParameter();
				newParam->m_type = type;
				newParam->m_location = location;

#ifndef GL_ES_VERSION_2_0
				// 如果支持Opengl3， uniform应该放到constant buffer中
				if (newParam->m_location < 0 && GLGraphicDriver::GetGL3Support())
				{
					int blockIndex, blockOffset;
					glGetActiveUniformsiv(m_uHwUID, 1, (const GLuint*)&i, GL_UNIFORM_BLOCK_INDEX, &blockIndex);  //取得uniform的块索引
					glGetActiveUniformsiv(m_uHwUID, 1, (const GLuint*)&i, GL_UNIFORM_OFFSET, &blockOffset); //取得uniform的块偏移值
					if (blockIndex >= 0)
					{
						newParam->m_location = blockOffset;
						newParam->m_bufferPtr = m_constantBuffers[blockToBinding[blockIndex]];
					}
				}
#endif
				if (newParam->m_location >= 0)
					m_shaderParamters[paramName] = newParam;  //加到参数表中
			}
			return true;
		}
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
				m_parameterSources[i] = (const void*)M_MAX_UNSIGNED;  //重置所有参数组
			m_uFrameNumber = g_uFrameNumber;
		}

#ifndef GL_ES_VERSION_2_0
		bool useBuffer = m_constantBuffers[group].Get() || m_constantBuffers[group + MAX_SHADER_PARAMETER_GROUPS].Get();  //使用UBO
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

