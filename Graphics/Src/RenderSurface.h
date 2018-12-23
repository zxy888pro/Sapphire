#pragma once

#include "Graphics.h"
#include "Viewport.h"

namespace Sapphire
{
	struct ITexture;

	class RenderSurface
	{
	public:
		RenderSurface();

		virtual ~RenderSurface();

		virtual void SetViewportNum(uint num);

		virtual void SetViewport(uint index, Viewport* viewport);

		virtual void SetUpdateMode(RenderSurfaceUpdateMode mode);

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

	private:

		ITexture* m_parentTex;
		std::vector<SharedPtr<Viewport>> m_viewports;  //ҕ��
		RenderSurfaceUpdateMode m_eUpdateMode;  //���ҕ�ڵĸ���ģʽ

		bool m_bUpdateQueue;  //������И˜�


	};

}