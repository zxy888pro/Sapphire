#pragma once

#include "Graphics.h"
#include "GPUObject.h"

namespace Sapphire
{
	class Shader;
	class ShaderProgram;

	class GLShaderVariation : public RefCounter, public GPUObject
	{
	public:

		GLShaderVariation();

		virtual ~GLShaderVariation();

		bool Create();

		void SetName(const std::string&  name);

		void SetDefines(const std::string& defines);

		Shader* GetOwner() const;

		const std::string& GetName() const;

		const std::string& GetDefines() const;

		const std::string& GetCompilerOutput() const;

		std::string GetFullName() const;

		virtual void Release() override;


		virtual void OnDeviceLost() override;



	private:

		WeakPtr<Shader> m_owner;

		ShaderType   m_eType;

		std::string  m_name;
		std::string  m_defines;
		std::string  m_compilerOutput;

	};

}