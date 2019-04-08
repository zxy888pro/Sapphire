#pragma once
#include "Graphics.h"
#include "ShaderScript.h"

namespace Sapphire
{

	class GLGraphicDriver;
	class GLShader;
	

	//��ȡ�͹���shader�ű�
	class ShaderMgr :public ResourceMgr
	{
	public:
		ShaderMgr();
		virtual ~ShaderMgr();

		//ͨ��shader���ã�����shader
		virtual GLShader* CreateShader(std::string cfgPath);

		virtual void StoreScriptDependency(std::string name,  ShaderScript* pScripts);

		static std::string GetFileName(const std::string Name, ShaderType type);



	protected:

		GLGraphicDriver*  m_pGraphicDriver;


	};

}