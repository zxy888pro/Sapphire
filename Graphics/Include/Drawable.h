#pragma once


#include "Sapphire.h"
#include  "GraphicsDefs.h"
#include "Component.h"


namespace Sapphire
{

	//����Ⱦ���,����������ڵ����ʾ��Ⱦ
	class SAPPHIRE_CLASS Drawable : public Component
	{
		SAPPHIRE_OBJECT(Drawable, Component)
	public:

		Drawable(Core* pCore);
		virtual ~Drawable();



	};

}


