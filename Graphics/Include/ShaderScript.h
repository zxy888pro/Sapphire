#pragma once
#include "Graphics.h"


namespace Sapphire
{
	class ShaderScript
	{
	public:
		ShaderScript()
		{
			m_type = UNKNOWN;
		}
		virtual ~ShaderScript()
		{

		}
		const std::string& GetName() const
		{
			return m_scriptContent;
		};

		const std::string& GetScriptContent() const
		{
			return m_scriptContent;
		}

		ShaderType GetShaderType() const
		{
			return m_type;
		}

		bool Load(const std::string& scriptsPath);

		bool UnLoad();
		 

		std::string m_name;
		std::string m_scriptContent;
		ShaderType  m_type;


	};

	typedef Handle<ShaderScript>  HSHADERSCRIPT;


}

