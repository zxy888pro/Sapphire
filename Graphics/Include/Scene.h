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

		/// 节点已添加，管理场景指针并且添加到id map
		void NodeAdded(SharedPtr<Node> node);
		 

	protected:



	};

}

