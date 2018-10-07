#include <GraphicsDefs.h>
#include "CubeTexture.h"
#include "GPUObject.h"
#include <ITextureMgr.h>
#include <Image.h>

namespace Sapphire
{

	 
	CubeTexture::CubeTexture():
		GPUObject(),
		m_uWidth(0),
		m_uHeight(0),
		m_uDepth(0),
		m_bIsCompress(false),
		m_mipLevel(0),
		m_uNumMipmaps(1),
		m_bIsDisposed(false),
		m_uAnisotropyLevel(8),
		m_ePixelFormat(PixelFormat::PF_R8G8B8A8),
		m_eFilterMode(TextureFilterMode::FILTER_NEAREST),
		m_eUsage(TextureUsage::TEXTURE_STATIC),
		m_szName(""),
		m_glType(GL_TEXTURE_CUBE_MAP)
	{
		m_eType = ResoureType_Texture;
		for (int i = 0; i < MAX_COORDS; ++i)
			m_eAddressMode_[i] = ADDRESS_REPEAT; m_eAddressMode_;
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
	}

	CubeTexture::CubeTexture(uint size, uint depth, PixelFormat pf /*= PF_R8G8B8A8*/, uint NumMipmaps /*= 1*/, int glTargerType /*= GL_TEXTURE_2D*/, TextureUsage eUsage /*= TextureUsage::TEXTURE_STATIC*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode r /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/)
	{
		m_uWidth = size;
		m_uHeight = size;
		m_uDepth = depth;
		m_ePixelFormat = pf;
		m_uNumMipmaps = NumMipmaps;
		m_eAddressMode_[TextureCoordinate::COORD_U] = s;
		m_eAddressMode_[TextureCoordinate::COORD_V] = t;
		m_eAddressMode_[TextureCoordinate::COORD_W] = r;
		m_eFilterMode = filterMode;
		m_szName = "";
		m_bIsCompress = false;
		m_bIsDisposed = false;
		m_eUsage = eUsage;
		m_mipLevel = 0;
		m_uAnisotropyLevel = 8;
		m_glType = glTargerType;
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
	}

	CubeTexture::~CubeTexture()
	{
		Release();
	}

	void CubeTexture::Release()
	{
		if (m_uHwUID != 0 && m_pGraphicDriver != NULL)
		{
			//释放占用纹理对象
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
				{
					if (m_pGraphicDriver->getTextureMgr()->GetTexture((TextureUnit)i) == this)
						m_pGraphicDriver->getTextureMgr()->SetTexture(0, (TextureUnit)i);
				}
				glDeleteTextures(1, &m_uHwUID);
			}
			//初始化硬件资源ID
			m_uHwUID = 0;
		}
	}

}