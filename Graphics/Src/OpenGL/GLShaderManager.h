#pragma once

#include "Sapphire.h"
#include "Graphics.h"


namespace Sapphire
{
	struct IShaderVariation;
	class  ResourceCache;
	class  ResourceLoader;
	class  IGraphicDriver;


	class SAPPHIRE_CLASS GLShaderManager : public BaseObject
	{
		SAPPHIRE_OBJECT(GLShaderManager, BaseObject)

	public:

		GLShaderManager(Core* pcore, IGraphicDriver* pDriver);
		virtual ~GLShaderManager();

		//shader��춱���YԴ����Փ�ϑ�ԓȫ����ͬ�����d
		virtual IShaderVariation* GetShader(ShaderType type, const char* name, const char* defines, bool bAsync = false);
		
		 



	private:

		IGraphicDriver*			m_pGraphicDriver;
		 

	};
}

