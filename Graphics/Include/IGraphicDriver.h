#pragma once

#include "Graphics.h"


namespace Sapphire
{
	struct  ITextureMgr;
	struct  IImageMgr;
	struct  IDisplayContext;
	class   GPUObject;
	

	//OpenGL 与硬件层访问驱动类。  
	//无关图形API
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

		/// 渲染帧的开始，如果设备有效可以渲染，返回true
		virtual bool BeginFrame() = 0;
		/// 渲染帧结束，交换双缓冲区
		virtual void EndFrame() = 0;

		virtual void PrepareDraw() = 0;
		virtual void* GetMainWindow() = 0;
		virtual ITextureMgr* getTextureMgr() const = 0;
		
		virtual IImageMgr* getImageMgr() const = 0;
		virtual GraphicDriverType getDriverType() const = 0;

		virtual bool    IsInitialized() = 0;   //是否初始化完成

		virtual void SetViewport(const IntRect& rect) = 0; //设置视口

		virtual bool  IsDeviceLost() = 0;  //设备是否丢失

		virtual bool SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable) = 0;

		virtual void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0) = 0;

		///  设置剔除模式
		virtual void SetCullMode(CullMode mode) = 0;
		///  设置填充模式
		virtual void SetFillMode(FillMode mode) = 0;

		virtual IDisplayContext*   GetDisplayContext() const = 0;

		/// 添加一个跟踪的GPUObject对象，由GPUObject调用
		virtual void AddGPUObject(GPUObject*  gpuObj) = 0;
		/// 移除一个GPUObject对象，由GPUObject调用
		virtual void RemoveGPUObject(GPUObject* gpuObj) = 0;

	protected:

		 

	};
}