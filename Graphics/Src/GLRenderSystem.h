#pragma once
#include "Graphics.h"
#include "GraphicDriver.h"
#include "IRenderSystem.h"

namespace Sapphire
{

	//OpenGL的绘制渲染系统
	//管理和繪製3D物件
	class GLRenderSystem : public IRenderSystem
	{
	public:
		GLRenderSystem();
		GLRenderSystem(GraphicDriver* pDriver);
		virtual ~GLRenderSystem();

		virtual void Update(float dTime);
		virtual void Render();

		//将一个要渲染视口的渲染表面加入队列
		virtual void QueueRenderSurface(IRenderSurface* renderTarget);

		virtual bool getDrawShadow() const { return m_bDrawShadow; }
		virtual void setDrawShadow(bool val) { m_bDrawShadow = val; }

	private:

		GraphicDriver*  m_pGraphicDriver;
		bool  m_bDrawShadow;


	};

}