#pragma once

#include "Sapphire.h"
#include "Component.h"

namespace Sapphire
{

	//所有位移/旋转/缩放等几何变化
	class SAPPHIRE_CLASS TransformComponent : public Component
	{
		SAPPHIRE_OBJECT(TransformComponent, Component)
	public:
		TransformComponent(Core* pCore);
		virtual ~TransformComponent();

	protected:


	};

}

