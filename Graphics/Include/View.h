#pragma once
#include "Sapphire.h"

namespace Sapphire
{

	class SAPPHIRE_CLASS View :public BaseObject
	{
		SAPPHIRE_OBJECT(View, BaseObject)

	public:
		View(Core* pCore);
		virtual ~View();



	};


}