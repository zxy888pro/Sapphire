#pragma once
#include "Graphics.h"
#include "GraphicDriver.h"
#include "IRenderSystem.h"

namespace Sapphire
{
	class GLGraphicDriver;
	//OpenGL的绘制渲染系统
	//管理和繪製3D物件
	class SAPPHIRE_CLASS GLRenderSystem : public IRenderSystem
	{
		SAPPHIRE_OBJECT(GLRenderSystem, IRenderSystem)

	public:
		GLRenderSystem(Core* pCore);
		GLRenderSystem(Core* pCore,GLGraphicDriver* pDriver);
		virtual ~GLRenderSystem();

		virtual void Update(float dTime);
		virtual void Render();

		//将一个要渲染视口的渲染表面加入队列
		virtual void QueueRenderSurface(IRenderSurface* renderTarget);

		virtual bool getDrawShadow() const { return m_bDrawShadow; }
		virtual void setDrawShadow(bool val) { m_bDrawShadow = val; }


		virtual void Initialize() override;
	private:

		GLGraphicDriver*  m_pGraphicDriver;
		bool  m_bDrawShadow;


	};

}