#include "stdafx.h"
#include "RenderState.h"


namespace Sapphire
{

	RenderState::RenderState()
	{
		LoadCurRenderState();
	}

	RenderState::~RenderState()
	{

	}

	void RenderState::LoadCurRenderState()
	{
		glGetBooleanv(GL_DEPTH_TEST, &bDepthTest);
		glGetBooleanv(GL_CULL_FACE, &bCullFace);
		glGetBooleanv(GL_BLEND, &bAlphaBlend);
	}

	void RenderState::SetState()
	{
		if (bDepthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		if (bCullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
		if (bAlphaBlend)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

}