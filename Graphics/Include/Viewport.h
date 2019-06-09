#pragma once

#include "Sapphire.h"
#include "Graphics.h"
#include "IVewport.h"
#include "Math/Rect.h"


namespace Sapphire
{

	class View;

	//�ӿ���renderSurface��backBuffer����
	class SAPPHIRE_CLASS Viewport : public BaseObject
	{
		SAPPHIRE_OBJECT(Viewport, BaseObject)

	public:

		Viewport(Core* pCore);
		virtual ~Viewport();

		




	private:

		//�ӿ�����
		IntRect m_viewportRect;

		//�ڲ���Ⱦ�ṹ
		SharedPtr<View> m_pView;

	};
}