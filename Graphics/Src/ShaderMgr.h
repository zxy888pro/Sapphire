#pragma once
#include "Graphics.h"
#include "ShaderScript.h"

namespace Sapphire
{

	class GLGraphicDriver;
	class GLShader;
	

	//读取和管理shader脚本
	class ShaderMgr :public ResourceMgr
	{
	public:
		ShaderMgr();
		virtual ~ShaderMgr();

		//通过shader配置，创建shader
		virtual GLShader* CreateShader(std::string cfgPath);

		virtual void StoreScriptDependency(std::string name,  ShaderScript* pScripts);

		static std::string GetFileName(const std::string Name, ShaderType type);



	protected:

		GLGraphicDriver*  m_pGraphicDriver;


	};

}