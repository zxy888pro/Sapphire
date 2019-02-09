#include "Graphics.h"
#include "GLRenderSystem.h"


namespace Sapphire
{

	GLRenderSystem::GLRenderSystem()
	{
		m_pGraphicDriver = NULL;
		m_bDrawShadow = false;
	}

	GLRenderSystem::GLRenderSystem(GraphicDriver* pDriver)
	{
		m_pGraphicDriver = pDriver;
		m_bDrawShadow = false;
	}

	GLRenderSystem::~GLRenderSystem()
	{

	}

	void GLRenderSystem::Update(float dTime)
	{

	}

	void GLRenderSystem::Render()
	{

	}

	void GLRenderSystem::QueueRenderSurface(IRenderSurface* renderTarget)
	{

	}

}