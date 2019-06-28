#include "Component.h"


namespace Sapphire
{

	Component::Component(Core* pCore) :BaseObject(pCore)
	{
		m_uID = pCore->GenUID();
	}

	Component::~Component()
	{

	}

	void Component::Remove()
	{

	}

	Sapphire::WeakPtr<Sapphire::Node> Component::GetNode() const
	{

	}

	bool Component::SetActive(bool val)
	{

	}

	void Component::SetNode(WeakPtr<Node> node)
	{

	}

}