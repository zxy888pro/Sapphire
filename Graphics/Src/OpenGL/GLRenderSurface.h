#pragma once

#include "Graphics.h"
#include "Viewport.h"
#include "IRenderSurface.h"

namespace Sapphire
{
	struct ITexture;
	class  GLGraphicDriver;

	class GLRenderSurface : public IRenderSurface
	{
		SAPPHIRE_OBJECT(GLRenderSurface, BaseObject)

	public:
		GLRenderSurface(Core* pCore);
		GLRenderSurface(Core* pCore,ITexture* pTexture);

		virtual ~GLRenderSurface();

		virtual void SetViewportNum(uint num); //�����ӿ���

		virtual void SetViewport(uint index, IViewport* viewport); //�����ӿ�
		 
		virtual void SetUpdateMode(RenderSurfaceUpdateMode mode);  //���ø���ģʽ
		//����Ҫ���ӵ���ɫ��ȾĿ��
		virtual void SetLinkedRenderTarget(IRenderSurface* rt);
		//����Ҫ���ӵ���Ⱥ�ģ����ȾĿ��
		virtual void SetLinkedDepthStencil(IRenderSurface* depthStencil); 

		/// �Ⱥ��ӿڵ��ֶ����� 
		virtual void QueueUpdate();

		virtual bool IsQueueUpdate() const { return m_bUpdateQueue; }

		virtual void ResetUpdateQueued();

		virtual int GetWidth() const;

		virtual int GetHeight() const;

		virtual TextureUsage  GetUseage() const;

		virtual RenderSurfaceUpdateMode GetUpdateMode() const;

		virtual int GetViewportNum() const;

		virtual IViewport* GetViewport(int index) const;
		
		virtual bool  WasUpdated() const;  //��������־����Ⱦ���{��

		virtual void Release();

		virtual bool CreateRenderBuffer(uint width, uint height, uint format);   //����һ����Ⱦ�������

		virtual void OnDeviceLost();

		virtual ITexture* GetParentTexture() const;

		virtual uint GetRenderBuffer() const;

		virtual int GetMultiSample() const;  //���ض��ز����ļ���

		virtual bool GetAutoResolve() const;   //�����Ƿ���ز���

		virtual IRenderSurface* GetLinkedRenderTarget() const;

		virtual IRenderSurface* GetLinkedDepthStencil() const;

		void SetTarget(unsigned target) { m_gpuTarget = target; }

		unsigned GetTarget() const { return m_gpuTarget; }

	private:

		ITexture* m_parentTex;   //������

		uint   m_gpuTarget;       //GPU��ȾĿ�����
		uint   m_gpuRenderBuffer;    //GPU��Ⱦ�������

		std::vector<SharedPtr<IViewport>> m_viewports;  //ҕ��
		RenderSurfaceUpdateMode m_eUpdateMode;  //���ҕ�ڵĸ���ģʽ

		WeakPtr<IRenderSurface> m_linkedColorRenderTarget;  //���ӵ�����ɫ��ȾĿ��
		WeakPtr<IRenderSurface> m_linkedDepthRenderTarget;  //���ӵ��������ȾĿ��
		GLGraphicDriver*  m_pGLDriver;
		bool m_bUpdateQueue;  //�Ⱥ���±�־

		bool m_resolveDirty;   


	};

}