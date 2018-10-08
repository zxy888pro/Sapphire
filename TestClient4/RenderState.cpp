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
		glGetBooleanv(GL_ALPHA_TEST, &bAlphaTest);
		glGetBooleanv(GL_CULL_FACE, &bCullFace);
		glGetBooleanv(GL_BLEND, &bAlphaBlend);
		glGetBooleanv(GL_STENCIL_TEST, &bStencilTest);
		glGetIntegerv(GL_STENCIL_FUNC, &nStencilFunc);
		glGetIntegerv(GL_STENCIL_REF, &nStencilRef);
		glGetIntegerv(GL_STENCIL_VALUE_MASK, &nStencilMask);
		glGetIntegerv(GL_STENCIL_FAIL, &nStencilFailFunc);
		glGetIntegerv(GL_STENCIL_PASS_DEPTH_FAIL, &nStencilPassDepthFailFunc);
		glGetIntegerv(GL_STENCIL_PASS_DEPTH_PASS, &nStencilPassDepthPassFunc);
		glGetIntegerv(GL_BLEND_SRC, &nBlendSrcFactor);
		glGetIntegerv(GL_BLEND_DST, &nBlendDstFactor);
		glGetIntegerv(GL_BLEND_EQUATION, &nBlendEquation);

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
		if (bAlphaTest)
		{
			glEnable(GL_ALPHA_TEST);
		}
		else
		{
			glDisable(GL_ALPHA_TEST);
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
		if (bStencilTest)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);

		}
		glStencilOp(nStencilFailFunc, nStencilPassDepthFailFunc, nStencilPassDepthPassFunc);
		glStencilFunc(nStencilFunc, nStencilRef, nStencilMask);
		glBlendFunc(nBlendSrcFactor, nBlendDstFactor);
		glBlendEquation(nBlendEquation);

	}

}