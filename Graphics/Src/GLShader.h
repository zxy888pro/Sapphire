#pragma once
#include "Graphics.h"
#include "IShader.h"
#include "GLShaderVariation.h"
#include "ShaderScript.h"

namespace Sapphire
{

	class GLGraphicDriver;

	//Shader�YԴ����Դ�a�Ͷ���shader
	class GLShader :public IShader, public BaseResource, public RefCounter
	{
	public:
		GLShader();
		virtual ~GLShader();

		IShaderVariation* GetVariation(ShaderType type, const std::string& defines);
		IShaderVariation* GetVariation(ShaderType type, const char* defines);

		const std::string&  GetSourceCode(ShaderType type) const;

		uint GetTimeStamp() const { return m_timeStamp; }

        //Resource���ֺ���
		virtual bool Create() override;
		virtual bool Recreate() override;
		virtual void Dispose() override;
		virtual size_t GetSize() override;
		virtual bool IsDisposed() override;
		virtual bool Load(HSHADERSCRIPT shaderPath);
		virtual const std::string& GetName() const
		{
			return m_name;
		}

	private:

		//����Դ�벢�����ļ�
		bool ProcessSource(std::string source);

		//��������岢������ظ�����
		std::string  NormalizeDefines(const std::string& defines);

		//���¼����ڴ�ռ��
		void RefreshMemoryUse();
		

		//�������shader��ԭ��
		std::string m_vsSource;
		std::string m_psSource;
		std::string m_gsSource;
		std::string m_csSource;
		//shader����
		std::string m_name;

		uint  m_timeStamp;
		uint  m_numVariation;
		

		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_vsVariation;
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_psVariation;
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_gsVariation;
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_csVariation;

		GLGraphicDriver* m_pGraphicDriver;
		bool  m_bIsDisposed;

	};
}