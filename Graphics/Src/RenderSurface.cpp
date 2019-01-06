#include "RenderSurface.h"
#include "ITexture.h"
#include "GraphicDriver.h"

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
		m_viewports.resize(num);
	}

	void RenderSurface::SetViewport(uint index, Viewport* viewport)
	{
		if (index >= m_viewports.size())
			m_viewports.resize(index + 1);

		m_viewports[index] = viewport;
	}

	void RenderSurface::SetUpdateMode(RenderSurfaceUpdateMode mode)
	{
		m_eUpdateMode = mode;
	}

	void RenderSurface::SetLinkedRenderTarget(RenderSurface* rt)
	{
		if (rt != this)
		{
			m_linkedColorRenderTarget = rt;
		}
	}

	void RenderSurface::SetLinkedDepthStencil(RenderSurface* depthStencil)
	{
		if (depthStencil != this)
		{
			m_linkedDepthRenderTarget = depthStencil;
		}

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
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		if (pGd == NULL)
			return;

		if (!pGd->IsDeviceLost())
		{
			for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (pGd->GetRenderTarget(i) == this)
					pGd->ResetRenderTarget(i);
			}

			if (pGd->GetDepthStencil() == this)
				pGd->ResetDepthStencil();

			// 清理非活动的FBO
			pGd->CleanupRenderSurface(this);

			if (m_gpuRenderBuffer)
				glDeleteRenderbuffersEXT(1, &m_gpuRenderBuffer);   //释放硬件资源
		}

		m_gpuRenderBuffer = 0;
	}

	bool RenderSurface::CreateRenderBuffer(uint width, uint height, uint format)
	{
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		if (pGd == NULL)
			return false;
		Release();
		 
		glGenRenderbuffersEXT(1, &m_gpuRenderBuffer);  //创建渲染缓冲对象
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_gpuRenderBuffer);
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, format, width, height);  //设置存储格式
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

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
		return m_gpuRenderBuffer;
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