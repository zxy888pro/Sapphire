#include "Viewport.h"
#include "View.h"


namespace Sapphire
{

	Viewport::Viewport(Core* pCore)
	{

	}

	Viewport::~Viewport()
	{

	}

	Sapphire::Ray Viewport::GetScreenRay(int x, int y) const
	{
		return Ray();
	}

}