#pragma once

#include "Sapphire.h"
#include "Graphics.h"
#include "IVewport.h"
#include "Math/Rect.h"
#include "Math/Ray.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"

namespace Sapphire
{

	class View;
	class Camera;
	class RenderPath;
	class Scene;

	//�ӿ���renderSurface��backBuffer����
	class SAPPHIRE_CLASS Viewport : public BaseObject
	{
		SAPPHIRE_OBJECT(Viewport, BaseObject)

	public:

		Viewport(Core* pCore);
		virtual ~Viewport();

		
		Ray GetScreenRay(int x, int y) const;



	private:

		//�ӿ�����
		IntRect m_viewportRect;

		//�ڲ���Ⱦ�ṹ
		SharedPtr<View> m_pView;

	};
}