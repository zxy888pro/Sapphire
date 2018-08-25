#include <GraphicDriver.h>
#include <GraphicException.h>
#include "Texture2D.h"
#include "TextureMgr.h"
#include "ImageMgr.h"

namespace Sapphire
{

	GraphicDriver::GraphicDriver()
	{
		m_pTextureMgr = new TextureMgr();
		m_pImageMgr = new ImageMgr();
	}

	GraphicDriver::~GraphicDriver()
	{

	}

	void GraphicDriver::Init()
	{
		CheckFeature();
	}

	void GraphicDriver::Release()
	{
		safeDelete(m_pImageMgr);	
		safeDelete(m_pTextureMgr);
		
	}

 

	void GraphicDriver::BindTexture(ITexture* pTexture, TextureUnit unit)
	{
		if (m_pTextureMgr)
		{
			m_pTextureMgr->SetTexture(pTexture, unit);
		}
		else
		{
			throw GraphicDriverException("TextureManager is Null!", GraphicDriverException::GDError_NullPointerError);
		}
	}

	bool GraphicDriver::IsDeviceLost()
	{
		return false;
	}

	uint GraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //查询允许的各向异性数量
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniLevel);
		return (uint)maxAniLevel;
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


	int GraphicDriver::GetHWTextureFormat(PixelFormat eFormat)
	{
		switch (eFormat)
		{
		case Sapphire::PF_L8:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_L16:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_A8:
			return GL_ALPHA;
			break;
		case Sapphire::PF_A4L4:
			return GL_LUMINANCE_ALPHA;
			break;
		case Sapphire::PF_R8G8B8A8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8R8G8B8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8B8G8R8:
			return GL_RGBA;
			break;
		case Sapphire::PF_R8G8B8:
			return GL_RGB;
			break;
		case Sapphire::PF_B8G8R8:
			return GL_RGB;
			break;
		case Sapphire::PF_R8:
			return GL_LUMINANCE;
			break;
		default:
			return GL_RGBA;
			break;
		}
	}

	int GraphicDriver::GetSWTextureFormat(PixelFormat eFormat)
	{
		switch (eFormat)
		{
		case Sapphire::PF_L8:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_L16:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_A8:
			return GL_ALPHA;
			break;
		case Sapphire::PF_A4L4:
			return GL_LUMINANCE_ALPHA;
			break;
		case Sapphire::PF_R8G8B8A8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8R8G8B8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8B8G8R8:
			return GL_RGBA;
			break;
		case Sapphire::PF_R8G8B8:
			return GL_RGB;
			break;
		case Sapphire::PF_B8G8R8:
			return GL_RGB;
			break;
		case Sapphire::PF_R8:
			return GL_LUMINANCE;
			break;
		default:
			return GL_RGBA;
			break;
		}
	}


	uint GraphicDriver::GetHWTextureDataType(PixelFormat ePformat)
	{
		uint format = GetHWTextureFormat(ePformat);

#ifndef GL_ES_VERSION_2_0
			if (format == GL_DEPTH24_STENCIL8_EXT)
				return GL_UNSIGNED_INT_24_8_EXT;
			else if (format == GL_RG16 || format == GL_RGBA16)
				return GL_UNSIGNED_SHORT;
			else if (format == GL_RGBA32F_ARB || format == GL_RG32F || format == GL_R32F)
				return GL_FLOAT;
			else if (format == GL_RGBA16F_ARB || format == GL_RG16F || format == GL_R16F)
				return GL_HALF_FLOAT_ARB;
			else
				return GL_UNSIGNED_BYTE;
#else
			if (format == GL_DEPTH_COMPONENT || format == GL_DEPTH_COMPONENT24_OES)
				return GL_UNSIGNED_INT;
			else if (format == GL_DEPTH_COMPONENT16)
				return GL_UNSIGNED_SHORT;
			else
				return GL_UNSIGNED_BYTE;
#endif
	}

	void GraphicDriver::CheckFeature()
	{
		m_bAnisotropySupport = GLEW_EXT_texture_filter_anisotropic != 0;
		m_nMaxTextureUnits = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
	}

}