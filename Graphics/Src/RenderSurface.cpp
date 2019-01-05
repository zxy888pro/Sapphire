#include "RenderSurface.h"
#include "ITexture.h"

namespace Sapphire
{

	RenderSurface::RenderSurface()
	{

	}

	RenderSurface::~RenderSurface()
	{

	}

	void RenderSurface::SetViewportNum(uint num)
	{

	}

	void RenderSurface::SetViewport(uint index, Viewport* viewport)
	{

	}

	void RenderSurface::SetUpdateMode(RenderSurfaceUpdateMode mode)
	{

	}

	void RenderSurface::SetLinkedRenderTarget(RenderSurface* rt)
	{

	}

	void RenderSurface::SetLinkedDepthStencil(RenderSurface* depthStencil)
	{

	}

	void RenderSurface::QueueUpdate()
	{
		m_bUpdateQueue = true;
	}

	void RenderSurface::ResetUpdateQueued()
	{
		m_bUpdateQueue = false;
	}

	int RenderSurface::GetWidth() const
	{
		return m_parentTex->getWidth();
	}

	int RenderSurface::GetHeight() const
	{
		return m_parentTex->getHeight();
	}

	Sapphire::TextureUsage RenderSurface::GetUseage() const
	{
		return m_parentTex->getUsage();
	}

	Sapphire::RenderSurfaceUpdateMode RenderSurface::GetUpdateMode() const
	{
		return m_eUpdateMode;
	}

	int RenderSurface::GetViewportNum() const
	{
		return m_viewports.size();
	}

	Sapphire::Viewport* RenderSurface::GetViewport(int index) const
	{
		return index < m_viewports.size() ? m_viewports[index] : (Viewport*)NULL;
	}

	bool RenderSurface::WasUpdated() const
	{
		return false;
	}

	void RenderSurface::Release()
	{

	}

	bool RenderSurface::CreateRenderBuffer(uint width, uint height, uint format)
	{
		return true;
	}

	void RenderSurface::OnDeviceLost()
	{

	}

	Sapphire::ITexture* RenderSurface::GetParentTexture() const
	{
		return m_parentTex;
	}

	uint RenderSurface::GetRenderBuffer() const
	{
		return 0;
	}

	int RenderSurface::GetMultiSample() const
	{
		return false;
	}

	bool RenderSurface::GetAutoResolve() const
	{
		return false;
	}

	Sapphire::RenderSurface* RenderSurface::GetLinkedRenderTarget() const
	{
		return m_linkedColorRenderTarget;
	}

	Sapphire::RenderSurface* RenderSurface::GetLinkedDepthStencil() const
	{
		return m_linkedDepthRenderTarget;
	}

}