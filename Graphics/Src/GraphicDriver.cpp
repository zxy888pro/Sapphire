#include <GraphicDriver.h>
#include <GraphicException.h>
#include "IGraphicDriver.h"
#include "GLGraphicDriver.h"

namespace Sapphire
{

	SAPPHIRE_API void createGraphicDriver()
	{
		Core* pCore = Core::GetSingletonPtr();
		if (pCore)
		{
			if (!pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER))
			{
				IGraphicDriver* pDriver = new GLGraphicDriver();
				pDriver->Init();
				pCore->RegisterSubSystem(pDriver, ESST_GRAPHICDRIVER);
			}
		}
		else
		{
			SAPPHIRE_LOGERROR("error : Sapphire Core is null!");
		}
	}

}

