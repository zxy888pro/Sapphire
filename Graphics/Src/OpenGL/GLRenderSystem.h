#pragma once
#include "Graphics.h"
#include "GraphicDriver.h"
#include "IRenderSystem.h"

namespace Sapphire
{
	class GLGraphicDriver;
	//OpenGL�Ļ�����Ⱦϵͳ
	//������L�u3D���
	class SAPPHIRE_CLASS GLRenderSystem : public IRenderSystem
	{
		SAPPHIRE_OBJECT(GLRenderSystem, IRenderSystem)

	public:
		GLRenderSystem(Core* pCore);
		GLRenderSystem(Core* pCore,GLGraphicDriver* pDriver);
		virtual ~GLRenderSystem();

		virtual void Update(float dTime);
		virtual void Render();

		//��һ��Ҫ��Ⱦ�ӿڵ���Ⱦ����������
		virtual void QueueRenderSurface(IRenderSurface* renderTarget);

		virtual bool getDrawShadow() const { return m_bDrawShadow; }
		virtual void setDrawShadow(bool val) { m_bDrawShadow = val; }


		virtual void Initialize() override;
	private:

		GLGraphicDriver*  m_pGraphicDriver;
		bool  m_bDrawShadow;


	};

}