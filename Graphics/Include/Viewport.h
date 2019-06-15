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

	//视口由renderSurface或backBuffer定义
	class SAPPHIRE_CLASS Viewport : public BaseObject
	{
		SAPPHIRE_OBJECT(Viewport, BaseObject)

	public:

		Viewport(Core* pCore);
		virtual ~Viewport();

		
		Ray GetScreenRay(int x, int y) const;



	private:

		//视口区域
		IntRect m_viewportRect;

		//内部渲染结构
		SharedPtr<View> m_pView;

	};
}