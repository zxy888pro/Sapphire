#pragma once

#include <SapphireDef.h>
#include "stdafx.h"

namespace Sapphire
{
	//��ȾĿ��
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void Initialize();

	protected:
		
		GLuint m_fbo;  //fbo����

	};
}