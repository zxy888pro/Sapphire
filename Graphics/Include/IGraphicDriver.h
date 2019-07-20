#pragma once

#include "Graphics.h"


namespace Sapphire
{
	struct  ITextureMgr;
	struct  IImageMgr;
	struct  IDisplayContext;
	class   GPUObject;
	

	//OpenGL ��Ӳ������������ࡣ  
	//�޹�ͼ��API
	class SAPPHIRE_CLASS IGraphicDriver : public SubSystem
	{
		SAPPHIRE_OBJECT(IGraphicDriver, SubSystem)

	public:

		IGraphicDriver(Core* pCore):
			SubSystem(pCore)
		{
			 
		}
		virtual ~IGraphicDriver(){}

		virtual void Init() = 0;
		virtual void Release() = 0;

		/// ��Ⱦ֡�Ŀ�ʼ������豸��Ч������Ⱦ������true
		virtual bool BeginFrame() = 0;
		/// ��Ⱦ֡����������˫������
		virtual void EndFrame() = 0;

		virtual void PrepareDraw() = 0;
		virtual void* GetMainWindow() = 0;
		virtual ITextureMgr* getTextureMgr() const = 0;
		
		virtual IImageMgr* getImageMgr() const = 0;
		virtual GraphicDriverType getDriverType() const = 0;

		virtual bool    IsInitialized() = 0;   //�Ƿ��ʼ�����

		virtual void SetViewport(const IntRect& rect) = 0; //�����ӿ�

		virtual bool  IsDeviceLost() = 0;  //�豸�Ƿ�ʧ

		virtual bool SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable) = 0;

		virtual void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0) = 0;

		///  �����޳�ģʽ
		virtual void SetCullMode(CullMode mode) = 0;
		///  �������ģʽ
		virtual void SetFillMode(FillMode mode) = 0;

		virtual IDisplayContext*   GetDisplayContext() const = 0;

		/// ���һ�����ٵ�GPUObject������GPUObject����
		virtual void AddGPUObject(GPUObject*  gpuObj) = 0;
		/// �Ƴ�һ��GPUObject������GPUObject����
		virtual void RemoveGPUObject(GPUObject* gpuObj) = 0;

	protected:

		 

	};
}