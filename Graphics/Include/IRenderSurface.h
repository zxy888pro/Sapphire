#pragma once
#include "Graphics.h"
#include "GraphicsDefs.h"


namespace Sapphire
{
	struct ITexture;
	class IViewport;

	//���Ա����M����Ⱦ���ɫ�������/ģ�� ����
	class SAPPHIRE_CLASS  IRenderSurface : public RefCounter
	{
	public:
		virtual void SetViewportNum(uint num) = 0;

		virtual void SetViewport(uint index, IViewport* viewport) = 0;

		virtual void SetUpdateMode(RenderSurfaceUpdateMode mode) = 0;
		//����Ҫ���ӵ���ɫ��ȾĿ��
		virtual void SetLinkedRenderTarget(IRenderSurface* rt) = 0;
		//����Ҫ���ӵ���Ⱥ�ģ����ȾĿ��
		virtual void SetLinkedDepthStencil(IRenderSurface* depthStencil) = 0;

		/// �Ⱥ��ӿڵ��ֶ����� 
		virtual void QueueUpdate() = 0;

		virtual bool IsQueueUpdate() const = 0;

		virtual void ResetUpdateQueued() = 0;

		virtual int GetWidth() const = 0;

		virtual int GetHeight() const = 0;

		virtual TextureUsage  GetUseage() const = 0;

		virtual RenderSurfaceUpdateMode GetUpdateMode() const = 0;

		virtual int GetViewportNum() const = 0;

		virtual IViewport* GetViewport(int index) const = 0;

		virtual bool  WasUpdated() const = 0;  //�������־����Ⱦ���{��

		virtual void Release() = 0;

		virtual bool CreateRenderBuffer(uint width, uint height, uint format) = 0;

		virtual void OnDeviceLost() = 0;

		virtual ITexture* GetParentTexture() const = 0;

		virtual uint GetRenderBuffer() const = 0;

		virtual int GetMultiSample() const = 0;  //���ض��ز����ļ���

		virtual bool GetAutoResolve() const = 0;   //�����Ƿ���ز���

		virtual IRenderSurface* GetLinkedRenderTarget() const = 0;

		virtual IRenderSurface* GetLinkedDepthStencil() const = 0;

		virtual void SetTarget(unsigned target) = 0;

		virtual unsigned GetTarget() const = 0;
	};
}