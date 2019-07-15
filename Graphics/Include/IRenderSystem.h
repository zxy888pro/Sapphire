#pragma once
#include <Graphics.h>



namespace Sapphire
{
	class  IRenderSurface;

	//��Ⱦϵͳ�Č���߲�ӿڣ�����3D�������
	class SAPPHIRE_CLASS IRenderSystem : public SubSystem
	{
		SAPPHIRE_OBJECT(IRenderSystem, SubSystem)
	public:
		IRenderSystem(Core* pCore) :SubSystem(pCore)
		{
			m_type = ESST_RENDERSYSTEM;
		}
		virtual ~IRenderSystem(){};
		virtual void Initialize() = 0;
		virtual void Update(float dTime) = 0;
		virtual void Render() = 0;
		virtual void QueueRenderSurface(IRenderSurface* renderTarget) = 0;
		virtual bool getDrawShadow() const = 0;
		virtual void setDrawShadow(bool val) = 0;

	};
}