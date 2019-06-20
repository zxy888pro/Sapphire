#pragma once


#include "Sapphire.h"
#include  "GraphicsDefs.h"
#include "Component.h"


namespace Sapphire
{

	//可渲染组件,这个组件负责节点的显示渲染
	class SAPPHIRE_CLASS Drawable : public Component
	{
		SAPPHIRE_OBJECT(Drawable, Component)
	public:

		Drawable(Core* pCore);
		virtual ~Drawable();



	};

}


