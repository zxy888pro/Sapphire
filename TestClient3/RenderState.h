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
	};
}