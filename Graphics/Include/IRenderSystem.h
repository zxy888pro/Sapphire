#pragma once
#include <Graphics.h>



namespace Sapphire
{
	class  IRenderSurface;

	//渲染系统的對外高层接口，负责3D物件绘制
	class SAPPHIRE_CLASS IRenderSystem : public SubSystem
	{
		SAPPHIRE_SUBSYSTEM(ESST_RENDERSYSTEM)
	public:

		virtual void Update(float dTime) = 0;
		virtual void Render() = 0;
		virtual void QueueRenderSurface(IRenderSurface* renderTarget) = 0;
		virtual bool getDrawShadow() const = 0;
		virtual void setDrawShadow(bool val) = 0;

	};
}