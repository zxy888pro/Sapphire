#pragma once

#include <SapphireDef.h>
#include "stdafx.h"

namespace Sapphire
{
	//渲染目标
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void Initialize();

	protected:
		
		GLuint m_fbo;  //fbo对象

	};
}