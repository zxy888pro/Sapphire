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

		GLShaderVariation(ShaderType type, Core* pCore, IGraphicDriver* pDriver);
		GLShaderVariation(ShaderType type, Core* pCore, IGraphicDriver* pDriver, GLShader* owner);

		virtual ~GLShaderVariation();
		//����ű�
		virtual bool Create();

		virtual void SetName(const std::string&  name);

		virtual void SetDefines(const std::string& defines);

		virtual IShader* GetOwner() const;

		void    SetOwner(GLShader* owner);

		ShaderType GetShaderType() const;

		virtual const std::string& GetName() const;

		virtual const std::string& GetDefines() const;

		const std::string& GetCompilerOutput() const;

		virtual const std::string GetFullName() const;


		virtual void Release() override;


		virtual void OnDeviceLost() override;



	private:

		std::string Precompile(const std::string& sourceCode);

		WeakPtr<GLShader> m_owner;

		ShaderType   m_eType;

		std::string  m_name;
		std::string  m_defines;  //�궨����ַ���
		std::string  m_compilerOutput;
		GLGraphicDriver* m_pGraphicDriver;

	};

}