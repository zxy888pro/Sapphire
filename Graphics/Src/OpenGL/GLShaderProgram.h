#pragma once
#include "Graphics.h"
#include "IShaderProgram.h"
#include "GPUObject.h"
#include "GLShaderVariation.h"
#include "ConstantBuffer.h"


namespace Sapphire
{
	

	///shader参数的定义
	struct ShaderParameter
	{
		 
		ShaderParameter() :
			m_bufferPtr(0)
		{
		}

		/// uniform在constant buffer中的位置偏移值
		int m_location;
		/// 类型
		unsigned m_type;
		/// constantBuffer 指针.
		ConstantBuffer* m_bufferPtr;
	};

	class GLShaderProgram : public IShaderProgram, public GPUObject, public BaseObject
	{

	public:

		GLShaderProgram(Core* pCore,IGraphicDriver* pDriver, GLShaderVariation* vertexShader, GLShaderVariation* pixelShader);
		virtual ~GLShaderProgram();

		virtual void Release() override;
		 


		virtual void OnDeviceLost() override;
	 
		//链接
		virtual bool Link();

		virtual IShaderVariation* GetVertexShader() const;
		virtual IShaderVariation* GetPixelShader() const;
		virtual IShaderVariation* GetGeometryShader() const;

		//是否用到纹理单元unit
		virtual bool HasTextureUnit(TextureUnit unit) const { return m_bUseTextureUnits[unit]; }

		virtual bool HasParameter(std::string param) const;

		virtual const ShaderParameter* GetParameter(std::string param) const;
		virtual const std::string& GetLinkerOutput() const { return m_linkOutMsg; }

		const SharedPtr<ConstantBuffer>* GetConstantBuffers() const { return &m_constantBuffers[0]; }

		/// 检查shader参数组是否需要更新，注意不能在shader执行的时候检查
		bool NeedParameterUpdate(ShaderParameterGroup group, const void* source);
		/// 清理当前参数源
		void ClearParameterSource(ShaderParameterGroup group);

		/// 通过增加全局参数源帧号清理所有shader的参数源
		static void ClearParameterSources();
		///当constant buffer改变，清理所有参数源
		static void ClearGlobalParameterSource(ShaderParameterGroup group);

	protected:

		GLGraphicDriver*  m_pDriver;


	private:

		WeakPtr<GLShaderVariation>   m_vertexShader;
		WeakPtr<GLShaderVariation>   m_pixelShader;
		WeakPtr<GLShaderVariation>   m_geometryShader;

		std::unordered_map<std::string, ShaderParameter*>   m_shaderParamters;

		//记录用到的纹理单元
		bool                        m_bUseTextureUnits[MAX_TEXTURE_UNITS];
		//constant Buffer集合， VertexShader和PixelShader各占一段，所以是两倍
		SharedPtr<ConstantBuffer>    m_constantBuffers[MAX_SHADER_PARAMETER_GROUPS * 2];
		/// 参数源    单独uniform使用
		const void* m_parameterSources[MAX_SHADER_PARAMETER_GROUPS];
		//全局参数源  使用constant buffer
		static const void* g_parameterSources[MAX_SHADER_PARAMETER_GROUPS];
		//shader帧号
		ulong   m_uFrameNumber;
		//全局帧号
		static ulong  g_uFrameNumber;

		//链接错误
		std::string m_linkOutMsg;

		


	};

	


}