#pragma once
#include "Graphics.h"
#include "IShaderProgram.h"
#include "GPUObject.h"
#include "GLShaderVariation.h"
#include "ConstantBuffer.h"


namespace Sapphire
{
	

	///shader�����Ķ���
	struct ShaderParameter
	{
		 
		ShaderParameter() :
			m_bufferPtr(0)
		{
		}

		/// uniform��constant buffer�е�λ��ƫ��ֵ
		int m_location;
		/// ����
		unsigned m_type;
		/// constantBuffer ָ��.
		ConstantBuffer* m_bufferPtr;
	};

	class GLShaderProgram : public IShaderProgram, public GPUObject, public RefCounter
	{

	public:

		GLShaderProgram();
		virtual ~GLShaderProgram();

		virtual void Release() override;
		 


		virtual void OnDeviceLost() override;
	 

		virtual bool Link();
		virtual IShaderVariation* GetVertexShader() const;
		virtual IShaderVariation* GetPixelShader() const;
		virtual IShaderVariation* GetGeometryShader() const;

		virtual bool HasTextureUnit(TextureUnit unit) const { return m_bUseTextureUnits[unit]; }
		virtual bool HasParameter(std::string param) const;

		virtual const ShaderParameter* GetParameter(std::string param) const;
		virtual const std::string& GetLinkerOutput() const { return m_linkOutMsg; }

		const SharedPtr<ConstantBuffer>* GetConstantBuffers() const { return &m_constantBuffers[0]; }

		/// ���shader�������Ƿ���Ҫ���£�ע�ⲻ����shaderִ�е�ʱ����
		bool NeedParameterUpdate(ShaderParameterGroup group, const void* source);
		/// ����ǰ����Դ
		void ClearParameterSource(ShaderParameterGroup group);

		/// ͨ������ȫ�ֲ���Դ֡����������shader�Ĳ���Դ
		static void ClearParameterSources();
		///��constant buffer�ı䣬�������в���Դ
		static void ClearGlobalParameterSource(ShaderParameterGroup group);


	private:

		WeakPtr<GLShaderVariation>   m_vertexShader;
		WeakPtr<GLShaderVariation>   m_pixelShader;
		WeakPtr<GLShaderVariation>   m_geometryShader;

		std::unordered_map<std::string, GLShaderProgram>   m_shaderParamters;

		//��¼�õ�������Ԫ
		bool                        m_bUseTextureUnits[MAX_TEXTURE_UNITS];
		//constant Buffer
		SharedPtr<ConstantBuffer>    m_constantBuffers[MAX_SHADER_PARAMETER_GROUPS * 2];
		/// ����Դ
		const void* m_parameterSources[MAX_SHADER_PARAMETER_GROUPS];

		//shader֡��
		ulong   m_uFrameNumber;
		//ȫ��֡��
		static ulong  g_uFrameNumber;

		//���Ӵ���
		std::string m_linkOutMsg;




	};

	


}