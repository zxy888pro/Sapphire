#pragma once
#include "Sapphire.h"
#include "Node.h"

namespace Sapphire
{

	//场景节点，场景节点包括多个功能组件和子节点
	class SAPPHIRE_CLASS SceneNode : public Node
	{
		SAPPHIRE_OBJECT(SceneNode, Node);

	public:

		SceneNode();
		virtual ~SceneNode();


	protected:

		

	};
}