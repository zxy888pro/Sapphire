#include "GLRenderSurface.h"
#include "ITexture.h"
#include "GLGraphicDriver.h"
#include "IRenderSystem.h"

namespace Sapphire
{

#ifdef GL_ES_VERSION_2_0
#define GL_RENDERBUFFER_EXT GL_RENDERBUFFER
#define glGenRenderbuffersEXT glGenRenderbuffers
#define glBindRenderbufferEXT glBindRenderbuffer
#define glRenderbufferStorageEXT glRenderbufferStorage
#define glDeleteRenderbuffersEXT glDeleteRenderbuffers
#endif

	GLRenderSurface::GLRenderSurface(Core* pCore):
		IRenderSurface(pCore),
		m_parentTex(NULL),
		m_gpuTarget(GL_TEXTURE_2D),
		m_bUpdateQueue(false),
		m_eUpdateMode(RenderSurfaceUpdateMode::SURFACE_UPDATEVISIBLE),
		m_gpuRenderBuffer(0)

	{
		m_pGLDriver = dynamic_cast<GLGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
	}

	GLRenderSurface::GLRenderSurface(Core* pCore,ITexture* parentTexture):
		IRenderSurface(pCore),
		m_parentTex(parentTexture),
		m_gpuTarget(GL_TEXTURE_2D),
		m_bUpdateQueue(false),
		m_eUpdateMode(RenderSurfaceUpdateMode::SURFACE_UPDATEVISIBLE),
		m_gpuRenderBuffer(0)
	{

	}

	GLRenderSurface::~GLRenderSurface()
	{
		Release();
	}

	void GLRenderSurface::SetViewportNum(uint num)
	{
		m_viewports.resize(num);
	}

	void GLRenderSurface::SetViewport(uint index, IViewport* viewport)
	{
		if (index >= m_viewports.size())
			m_viewports.resize(index + 1);

		m_viewports[index] = viewport;
	}

	void GLRenderSurface::SetUpdateMode(RenderSurfaceUpdateMode mode)
	{
		m_eUpdateMode = mode;
	}

	void GLRenderSurface::SetLinkedRenderTarget(IRenderSurface* rt)
	{
		if (rt != this)
		{
			m_linkedColorRenderTarget = rt; //颜色渲染缓冲区
		}
	}

	void GLRenderSurface::SetLinkedDepthStencil(IRenderSurface* depthStencil)
	{
		if (depthStencil != this)
		{
			m_linkedDepthRenderTarget = depthStencil;
		}

	}

	void GLRenderSurface::QueueUpdate()
	{
		if (!m_bUpdateQueue)
		{
			bool hasValidView = false;
			//驗證有可用的Viewport
			for (unsigned i = 0; i < m_viewports.size(); ++i)
			{
				if (m_viewports[i])
				{
					hasValidView = true;
					break;
				}
			}

			if (hasValidView)
			{
			//將這個RenderSurface加入渲染隊列
				IRenderSystem*  pRenderSys = dynamic_cast<IRenderSystem*>(m_pCore->GetSubSystemWithType(ESST_RENDERSYSTEM));
				if (pRenderSys)
					pRenderSys->QueueRenderSurface(this);

				m_bUpdateQueue = true;
			}
		}
	}

	void GLRenderSurface::ResetUpdateQueued()
	{
		m_bUpdateQueue = false;
	}

	int GLRenderSurface::GetWidth() const
	{
		return m_parentTex->getWidth();
	}

	int GLRenderSurface::GetHeight() const
	{
		return m_parentTex->getHeight();
	}

	Sapphire::TextureUsage GLRenderSurface::GetUseage() const
	{
		return m_parentTex->getUsage();
	}

	Sapphire::RenderSurfaceUpdateMode GLRenderSurface::GetUpdateMode() const
	{
		return m_eUpdateMode;
	}

	int GLRenderSurface::GetViewportNum() const
	{
		return m_viewports.size();
	}

	Sapphire::IViewport* GLRenderSurface::GetViewport(int index) const
	{
		return index < m_viewports.size() ? m_viewports[index] : (IViewport*)NULL;
	}

	bool GLRenderSurface::WasUpdated() const
	{
		return false;
	}

	void GLRenderSurface::Release()
	{
		if (m_pGLDriver == NULL)
			return;

		if (!m_pGLDriver->IsDeviceLost())
		{
			for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (m_pGLDriver->GetRenderTarget(i) == this)
					m_pGLDriver->ResetRenderTarget(i);
			}

			if (m_pGLDriver->GetDepthStencil() == this)
				m_pGLDriver->ResetDepthStencil();

			// 清理非活动的FBO
			m_pGLDriver->CleanupRenderSurface(this);

			if (m_gpuRenderBuffer)
				glDeleteRenderbuffersEXT(1, &m_gpuRenderBuffer);   //释放硬件资源
		}

		m_gpuRenderBuffer = 0;
	}

	bool GLRenderSurface::CreateRenderBuffer(uint width, uint height, uint format)
	{
		if (m_pGLDriver == NULL)
			return false;
		Release();
		 
		glGenRenderbuffersEXT(1, &m_gpuRenderBuffer);  //创建RBO渲染缓冲对象 
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_gpuRenderBuffer); //绑定渲染缓冲对象
		glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, format, width, height);  //设置存储格式
		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);

		return true;
	}

	void GLRenderSurface::OnDeviceLost()
	{
		//設備已經丟失掉了，釋放還在用的系統資源
		if (m_pGLDriver == NULL)
			return;

		if (!m_pGLDriver->IsDeviceLost())
		{
			for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (m_pGLDriver->GetRenderTarget(i) == this)
					m_pGLDriver->ResetRenderTarget(i);
			}

			if (m_pGLDriver->GetDepthStencil() == this)
				m_pGLDriver->ResetDepthStencil();

			// 清理非活动的FBO
			m_pGLDriver->CleanupRenderSurface(this);

		}

		m_gpuRenderBuffer = 0;
	}

	Sapphire::ITexture* GLRenderSurface::GetParentTexture() const
	{
		return m_parentTex;
	}

	uint GLRenderSurface::GetRenderBuffer() const
	{
		return m_gpuRenderBuffer;
	}

	int GLRenderSurface::GetMultiSample() const
	{
		return false;
	}

	bool GLRenderSurface::GetAutoResolve() const
	{
		return false;
	}

	Sapphire::IRenderSurface* GLRenderSurface::GetLinkedRenderTarget() const
	{
		return m_linkedColorRenderTarget;
	}

	Sapphire::IRenderSurface* GLRenderSurface::GetLinkedDepthStencil() const
	{
		return m_linkedDepthRenderTarget;
	}

}