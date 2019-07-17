#include <GraphicDriver.h>
#include <GraphicException.h>
#include "IGraphicDriver.h"
#include "GLGraphicDriver.h"
#include "IRenderSystem.h"
#include "GLRenderSystem.h"

namespace Sapphire
{

	SAPPHIRE_API void createGraphicDriver()
	{
		Core* pCore = Core::GetSingletonPtr();
		if (pCore)
		{
			if (!pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER))
			{
				IGraphicDriver* pDriver = new GLGraphicDriver(pCore);
				pCore->RegisterSubSystem(pDriver, ESST_GRAPHICDRIVER);
				pDriver->Init();
			}
		}
		else
		{
			SAPPHIRE_LOGERROR("error : Sapphire Core is null!");
		}
	}

	SAPPHIRE_API void createGraphicDriver(Core* pCore)
	{
		if (pCore)
		{
			if (!pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER))
			{
				IGraphicDriver* pDriver = new GLGraphicDriver(pCore);
				pCore->RegisterSubSystem(pDriver, ESST_GRAPHICDRIVER);
				pDriver->Init();
			}
		}
		else
		{
			SAPPHIRE_LOGERROR("error : Sapphire Core is null!");
		}
	}

	 

}

