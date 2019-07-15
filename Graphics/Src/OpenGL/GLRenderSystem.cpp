#include "Graphics.h"
#include "GLRenderSystem.h"


namespace Sapphire
{

	GLRenderSystem::GLRenderSystem(Core* pCore):
		IRenderSystem(pCore)
	{
		m_pGraphicDriver = NULL;
		m_bDrawShadow = false;
	}

	GLRenderSystem::GLRenderSystem(Core* pCore,GLGraphicDriver* pDriver):
		IRenderSystem(pCore)
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

	void GLRenderSystem::Initialize()
	{
		
	}

}