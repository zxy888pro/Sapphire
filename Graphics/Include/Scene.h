#pragma once

#include "Sapphire.h"
#include "SceneNode.h"

namespace Sapphire
{

	//�����ڵ㣬���������ĸ�
	class SAPPHIRE_CLASS Scene :public Node
	{
		SAPPHIRE_OBJECT(Scene, Node)
	public:

		Scene(Core* pCore);
		virtual ~Scene();

	protected:



	};

}

