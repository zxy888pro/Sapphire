#pragma once

#include "Graphics.h"
#include "GPUObject.h"
#include "IShaderVariation.h"

namespace Sapphire
{
	class GLShader;
	class IShader;
	class ShaderProgram;
	class GLGraphicDriver;

	//GPU�ϵ�vs/ps/gs shader����
	class GLShaderVariation :public BaseObject,public IShaderVariation, public GPUObject
	{
		SAPPHIRE_OBJECT(GLShaderVariation, BaseObject)

	public:

		GLShaderVariation(ShaderType type, Core* pCore);

		virtual ~GLShaderVariation();
		//����ű�
		bool Create();

		void SetName(const std::string&  name);

		void SetDefines(const std::string& defines);

		IShader* GetOwner() const;

		ShaderType GetShaderType() const;

		const std::string& GetName() const;

		const std::string& GetDefines() const;

		const std::string& GetCompilerOutput() const;

		std::string GetFullName() const;



		virtual void Release() override;


		virtual void OnDeviceLost() override;



	private:

		WeakPtr<GLShader> m_owner;

		ShaderType   m_eType;

		std::string  m_name;
		std::string  m_defines;  //�궨����ַ���
		std::string  m_compilerOutput;
		GLGraphicDriver* m_pGraphicDriver;

	};

}