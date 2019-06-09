#pragma once

#include "Sapphire.h"
#include "Graphics.h"
#include "IVewport.h"
#include "Math/Rect.h"


namespace Sapphire
{

	class View;

	//视口由renderSurface或backBuffer定义
	class SAPPHIRE_CLASS Viewport : public BaseObject
	{
		SAPPHIRE_OBJECT(Viewport, BaseObject)

	public:

		Viewport(Core* pCore);
		virtual ~Viewport();

		




	private:

		//视口区域
		IntRect m_viewportRect;

		//内部渲染结构
		SharedPtr<View> m_pView;

	};
}