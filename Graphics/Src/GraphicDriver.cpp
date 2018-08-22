#include <GraphicDriver.h>
#include "Texture2D.h"
#include "TextureMgr.h"

namespace Sapphire
{

	GraphicDriver::GraphicDriver()
	{
		m_pTextureMgr = new TextureMgr();
	}

	GraphicDriver::~GraphicDriver()
	{

	}

	void GraphicDriver::Init()
	{

	}

	int GraphicDriver::GetHWTextureWarpParam(TextureAddressMode mode)
	{
		switch (mode)
		{
		case Sapphire::ADDRESS_REPEAT:
			return GL_REPEAT;
			break;
		case Sapphire::ADDRESS_MIRROR:
			return GL_MIRRORED_REPEAT;
			break;
		case Sapphire::ADDRESS_CLAMP:
			return GL_CLAMP;
			break;
		case Sapphire::ADDRESS_BORDER:
			return GL_CLAMP_TO_BORDER;
			break;
		default:
			return GL_REPEAT;
			break;
		}
	}


}