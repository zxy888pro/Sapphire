#pragma once

#include "Sapphire.h"
#include "Component.h"

namespace Sapphire
{

	//����λ��/��ת/���ŵȼ��α仯
	class SAPPHIRE_CLASS TransformComponent : public Component
	{
		SAPPHIRE_OBJECT(TransformComponent, Component)
	public:
		TransformComponent(Core* pCore);
		virtual ~TransformComponent();

	protected:


	};

}

