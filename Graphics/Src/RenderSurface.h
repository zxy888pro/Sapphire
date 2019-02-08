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
		RenderSurface(ITexture* pTexture);

		virtual ~RenderSurface();

		virtual void SetViewportNum(uint num);

		virtual void SetViewport(uint index, Viewport* viewport);

		virtual void SetUpdateMode(RenderSurfaceUpdateMode mode);
		//����Ҫ���ӵ���ɫ��ȾĿ��
		virtual void SetLinkedRenderTarget(RenderSurface* rt);
		//����Ҫ���ӵ���Ⱥ�ģ����ȾĿ��
		virtual void SetLinkedDepthStencil(RenderSurface* depthStencil); 

		/// �Ⱥ��ӿڵ��ֶ����� 
		virtual void QueueUpdate();

		virtual bool IsQueueUpdate() const { return m_bUpdateQueue; }

		virtual void ResetUpdateQueued();

		virtual int GetWidth() const;

		virtual int GetHeight() const;

		virtual TextureUsage  GetUseage() const;

		virtual RenderSurfaceUpdateMode GetUpdateMode() const;

		virtual int GetViewportNum() const;

		virtual Viewport* GetViewport(int index) const;
		
		virtual bool  WasUpdated() const;  //�������־����Ⱦ���{��

		virtual void Release();

		virtual bool CreateRenderBuffer(uint width, uint height, uint format);

		virtual void OnDeviceLost();

		virtual ITexture* GetParentTexture() const;

		virtual uint GetRenderBuffer() const;

		virtual int GetMultiSample() const;  //���ض��ز����ļ���

		virtual bool GetAutoResolve() const;   //�����Ƿ���ز���

		virtual RenderSurface* GetLinkedRenderTarget() const;

		virtual RenderSurface* GetLinkedDepthStencil() const;

		void SetTarget(unsigned target) { m_gpuTarget = target; }

		unsigned GetTarget() const { return m_gpuTarget; }

	private:

		ITexture* m_parentTex;   //������

		uint   m_gpuTarget;       //GPU��ȾĿ�����
		uint   m_gpuRenderBuffer;    //GPU��Ⱦ�������

		std::vector<SharedPtr<Viewport>> m_viewports;  //ҕ��
		RenderSurfaceUpdateMode m_eUpdateMode;  //���ҕ�ڵĸ���ģʽ

		WeakPtr<RenderSurface> m_linkedColorRenderTarget;  //���ӵ�����ɫ��ȾĿ��
		WeakPtr<RenderSurface> m_linkedDepthRenderTarget;  //���ӵ��������ȾĿ��

		bool m_bUpdateQueue;  //�Ⱥ���±�־

		bool m_resolveDirty;   


	};

}