#pragma once
#include <Sapphire.h>
#include "stdafx.h"

namespace Sapphire
{
	class RenderState
	{
	public:
		RenderState();
		virtual ~RenderState();
	    
		void LoadCurRenderState();
		void SetState();

		GLboolean bDepthTest;
		GLboolean bCullFace;
		GLboolean bAlphaBlend;
		GLint     nStencilFunc;
		GLint     nStencilRef;
		GLint     nStencilMask;
		GLint     nStencilFailFunc;
		GLint     nStencilPassDepthFailFunc;
		GLint     nStencilPassDepthPassFunc;
		GLboolean     bStencilTest;
	};
}