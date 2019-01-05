#pragma once

#include "Graphics.h"
#include "Viewport.h"

namespace Sapphire
{
	struct ITexture;

	class RenderSurface : public RefCounter
	{
	public:
		RenderSurface();

		virtual ~RenderSurface();

		virtual void SetViewportNum(uint num);

		virtual void SetViewport(uint index, Viewport* viewport);

		virtual void SetUpdateMode(RenderSurfaceUpdateMode mode);
		//设置要链接的颜色渲染目标
		virtual void SetLinkedRenderTarget(RenderSurface* rt);
		//设置要链接的深度和模板渲染目标
		virtual void SetLinkedDepthStencil(RenderSurface* depthStencil); 

		/// 等候视口的手动更新 
		virtual void QueueUpdate();

		virtual bool IsQueueUpdate() const { return m_bUpdateQueue; }

		virtual void ResetUpdateQueued();

		virtual int GetWidth() const;

		virtual int GetHeight() const;

		virtual TextureUsage  GetUseage() const;

		virtual RenderSurfaceUpdateMode GetUpdateMode() const;

		virtual int GetViewportNum() const;

		virtual Viewport* GetViewport(int index) const;
		
		virtual bool  WasUpdated() const;  //清理更新標志，渲染后調用

		virtual void Release();

		virtual bool CreateRenderBuffer(uint width, uint height, uint format);

		virtual void OnDeviceLost();

		virtual ITexture* GetParentTexture() const;

		virtual uint GetRenderBuffer() const;

		virtual int GetMultiSample() const;  //返回多重采样的级别

		virtual bool GetAutoResolve() const;   //返回是否多重采样

		virtual RenderSurface* GetLinkedRenderTarget() const;

		virtual RenderSurface* GetLinkedDepthStencil() const;

	private:

		ITexture* m_parentTex;   //父纹理

		std::vector<SharedPtr<Viewport>> m_viewports;  //視口
		RenderSurfaceUpdateMode m_eUpdateMode;  //對於視口的更新模式

		WeakPtr<RenderSurface> m_linkedColorRenderTarget;  //链接到的颜色渲染目标
		WeakPtr<RenderSurface> m_linkedDepthRenderTarget;  //链接到的深度渲染目标

		bool m_bUpdateQueue;  //等候更新标志

		bool m_resolveDirty;   


	};

}