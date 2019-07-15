#pragma once

#include "Graphics.h"


namespace Sapphire
{
	struct  ITextureMgr;
	struct  IImageMgr;
	struct  IDisplayContext;
	

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

		virtual IDisplayContext*   GetDisplayContext() const = 0;

	protected:

		 

	};
}