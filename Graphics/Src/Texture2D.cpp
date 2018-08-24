#include <GraphicDriver.h>
#include "Texture2D.h"
#include <ITextureMgr.h>
#include <mathHelper.h>


namespace Sapphire
{

	Texture2D::Texture2D() :
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
		m_glType(GL_TEXTURE_2D)
	{
		m_eType = ResoureType_Texture;
		for (int i = 0; i < MAX_COORDS; ++i)
			m_eAddressMode_[i] = ADDRESS_REPEAT; m_eAddressMode_;
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
	}

	Texture2D::Texture2D(uint width, uint height, uint depth, PixelFormat pf /*= PF_R8G8B8A8*/, uint NumMipmaps /*= 1*/, TextureUsage eUsage /*= TextureUsage::TEXTURE_STATIC*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_WRAP*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_WRAP*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/)
	{
		m_uWidth = width;
		m_uHeight = height;
		m_uDepth = depth;
		m_ePixelFormat = pf;
		m_uNumMipmaps = NumMipmaps;
		m_eAddressMode_[TextureCoordinate::COORD_U] = s;
		m_eAddressMode_[TextureCoordinate::COORD_V] = t;
		m_eFilterMode = filterMode;
		m_szName = "";
		m_bIsCompress = false;
		m_bIsDisposed = false;
		m_eUsage = eUsage;
		m_mipLevel = 0;
		m_uAnisotropyLevel = 8;
		m_glType = GL_TEXTURE_2D;
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
	}

	Texture2D::~Texture2D()
	{
		Release();
	}

	void Texture2D::Release()
	{
		if (m_uHwUID != 0 && m_pGraphicDriver != NULL)
		{
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
				{
					//释放当前纹理所占用的纹理单元
					if (m_pGraphicDriver->getTextureMgr()->GetTexture((TextureUnit)i) == this)
						m_pGraphicDriver->getTextureMgr()->SetTexture(0, (TextureUnit)i);
				}
				glDeleteTextures(1, &m_uHwUID);
			}
			//初始化硬件资源ID
			m_uHwUID = 0;
		}
			
	}

	bool Texture2D::Recreate()
	{
		return false;
	}

	void Texture2D::Dispose()
	{

	}

	void Texture2D::GPUObjectInit()
	{
		glGenTextures(1, &m_uHwUID);
		glBindTexture(GL_TEXTURE_2D, m_uHwUID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_U]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_V]));

		switch (m_eFilterMode)
		{
		case Sapphire::FILTER_NEAREST:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			}
			break;
		case Sapphire::FILTER_BILINEAR:
		    {	
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;
		case Sapphire::FILTER_TRILINEAR:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			}
			break;
		case Sapphire::FILTER_ANISOTROPIC:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				GLfloat maxAniLevel;    //查询允许的各向异性数量
				glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniLevel);
				m_uAnisotropyLevel = MIN(maxAniLevel, m_uAnisotropyLevel);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_uAnisotropyLevel);
			}
			break;
		case Sapphire::FILTER_DEFAULT:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
			break;
		default:
			{
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		    }
			break;
		}
		
	}

	/*void Texture2D::SetData(void* pData)
	{
		if (pData != NULL)
		{
			GLint format = GraphicDriver::GetSWTextureFormat(m_ePixelFormat);
			GLint internalFormat = GraphicDriver::GetHWTextureFormat(m_ePixelFormat);
			glTexImage2D(GL_TEXTURE_2D, m_mipLevel, internalFormat, m_uWidth, m_uHeight, 0, format, GL_UNSIGNED_BYTE, pData);
		}
	}*/

	size_t Texture2D::GetSize()
	{
		return m_uSize;
	}


	void Texture2D::Load(HIMAGE himg)
	{
		//先释放之前的纹理
		Release();

	}

	uint Texture2D::getUID() const
	{
		return m_uHwUID;
	}

}