#pragma once

#include "Sapphire.h"
#include "Math/Vector3.h"


namespace Sapphire
{

	class SAPPHIRE_CLASS Sphere
	{
	public:
		Sphere();



		/// Sphere center.
		Vector3 center_;
		/// Sphere radius.
		float radius_;

	};

}