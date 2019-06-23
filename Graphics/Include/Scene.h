#pragma once

#include "Sapphire.h"
#include "SceneNode.h"

namespace Sapphire
{

	//场景节点，整个场景的根
	class SAPPHIRE_CLASS Scene :public Node
	{
		SAPPHIRE_OBJECT(Scene, Node)
	public:

		Scene(Core* pCore);
		virtual ~Scene();

	protected:



	};

}

