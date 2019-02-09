#pragma once
#include "Graphics.h"
#include "GraphicDriver.h"
#include "IRenderSystem.h"

namespace Sapphire
{

	//OpenGL�Ļ�����Ⱦϵͳ
	//������L�u3D���
	class GLRenderSystem : public IRenderSystem
	{
	public:
		GLRenderSystem();
		GLRenderSystem(GraphicDriver* pDriver);
		virtual ~GLRenderSystem();

		virtual void Update(float dTime);
		virtual void Render();

		//��һ��Ҫ��Ⱦ�ӿڵ���Ⱦ����������
		virtual void QueueRenderSurface(IRenderSurface* renderTarget);

		virtual bool getDrawShadow() const { return m_bDrawShadow; }
		virtual void setDrawShadow(bool val) { m_bDrawShadow = val; }

	private:

		GraphicDriver*  m_pGraphicDriver;
		bool  m_bDrawShadow;


	};

}