#include <GraphicDriver.h>
#include "Texture2D.h"
#include <ITextureMgr.h>
#include "ImageMgr.h"
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

	Texture2D::Texture2D(uint width, uint height, uint depth, PixelFormat pf /*= PF_R8G8B8A8*/, uint NumMipmaps /*= 1*/,int glTargerType /*= GL_TEXTURE_2D*/, TextureUsage eUsage /*= TextureUsage::TEXTURE_STATIC*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_WRAP*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_WRAP*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/)
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
		m_glType = glTargerType;
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

	bool Texture2D::Recreate()
	{
		Release();
		Create();
		return true;
	}

	void Texture2D::Dispose()
	{

	}


	size_t Texture2D::GetSize()
	{
		//返回纹理对象大小和该类对象大小
		return m_uSize + sizeof(Texture2D);
	}

	size_t Texture2D::GetDataSize() const
	{
		if (m_bIsCompress)
		{
			return m_uSize;
		}
		return m_uSize;
	}


	uint Texture2D::GetRowSize() const
	{
		switch (m_ePixelFormat)
		{
		case Sapphire::PF_L8:
			return m_uWidth;
		case Sapphire::PF_L16:
			return m_uWidth * 2;
		case Sapphire::PF_A8:
			return m_uWidth;
		case Sapphire::PF_A4L4:
			return m_uWidth;
		case Sapphire::PF_R8G8B8A8:
			return m_uWidth * 4;
		case Sapphire::PF_A8R8G8B8:
			return m_uWidth * 4;
		case Sapphire::PF_A8B8G8R8:
			return m_uWidth * 4;
		case Sapphire::PF_R8G8B8:
			return m_uWidth * 3;
		case Sapphire::PF_B8G8R8:
			return m_uWidth * 3;
		case Sapphire::PF_R8:
			return m_uWidth;
		default:
			return 0;
			break;
		}
	}

	void Texture2D::Load(HIMAGE himg)
	{
		if (m_pGraphicDriver == NULL)
			return;

		if (m_pGraphicDriver->IsDeviceLost())
		{
			return;
		}
		//先释放之前的纹理对象
		Release();
		if (himg.IsNull())
		{
			LogUtil::LogMsgLn("Texture2D Load Image Failed! handle is Null!");
			return;
		}
		IImageMgr* pImageMgr =  m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			LogUtil::LogMsgLn("ImageMgr is not initialized!");
			return;
		}
		PRAWIMAGE pImgData = pImageMgr->GetTexture(himg);
		uint width = pImageMgr->GetWidth(himg);
		uint height = pImageMgr->GetHeight(himg);
		m_uWidth = width;
		m_uHeight = height;
		//通道数相当于占用字节数
		uint nChannels = pImageMgr->GetNumChannels(himg);
		m_uSize = width * height * nChannels;
		if (pImgData == NULL)
		{
			LogUtil::LogMsgLn("Create Texture Failed! RawData is Null");
			return;
		}
		//创建纹理对象
		Create();
		SetData(m_mipLevel, 0, 0, width, height, pImgData);

	}

	void Texture2D::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
	}

	void Texture2D::OnDeviceReset()
	{
		if (m_uHwUID)
		{
			Recreate();
		}
	}

	bool Texture2D::Create()
	{
		//先释放旧的
		Release();

		if (m_pGraphicDriver == NULL || m_uWidth == 0 || m_uHeight == 0)
			return false;

		if (m_pGraphicDriver->IsDeviceLost())
		{
			LogUtil::LogMsgLn("Texture Creation While Device is Lost!");
			return true;
		}

		glGenTextures(1, &m_uHwUID);
		//glBindTexture(GL_TEXTURE_2D, m_uHwUID);
		//Diffuse作为默认单元
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE);

		glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_U]));
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_V]));

		switch (m_eFilterMode)
		{
		case Sapphire::FILTER_NEAREST:
		{
			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
		case Sapphire::FILTER_BILINEAR:
		{
			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		case Sapphire::FILTER_TRILINEAR:
		{
			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		break;
		case Sapphire::FILTER_ANISOTROPIC:
		{
			if (m_pGraphicDriver->GetAnisotropySupport())
			{
				glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				m_uAnisotropyLevel = MIN(m_pGraphicDriver->GetMaxAnisotropyLevels(), m_uAnisotropyLevel);
				glTexParameterf(m_glType, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_uAnisotropyLevel);
			}
		}
		break;
		case Sapphire::FILTER_DEFAULT:
		{
			glTexParameteri(m_glType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_glType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		default:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		}

		//解除绑定
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);
		return true;
	}

	bool Texture2D::SetSize(int width, int height, PixelFormat eformat, TextureUsage eUsage /*= TEXTURE_STATIC*/)
	{

		if (eUsage == TEXTURE_RENDERTARGET)
		{

		}

		m_eUsage = eUsage;
		m_uWidth = width;
		m_uHeight = height;
		m_ePixelFormat = eformat;
		//大小改变重新创建
		Create();
		return true;
	}

	bool Texture2D::SetData(uint level, int x, int y, int width, int height, const void* pData)
	{
		if (!m_pGraphicDriver)
		{
			LogUtil::LogMsgLn("Error GraphicDriver is Null!");
			return false;
		}
		ITextureMgr* pTexMgr = m_pGraphicDriver->getTextureMgr();
		if (!pTexMgr)
		{
			LogUtil::LogMsgLn("Error TextureMgr is Null!");
			return false;
		}
		if (!pTexMgr->VerifyHWUID(m_uHwUID))
		{
			LogUtil::LogMsgLn("Error HwUID is invalid!");
			return false;
		}
		int format = GraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		int internalFormat = GraphicDriver::GetHWTextureFormat(m_ePixelFormat);

		//获取level级下mipmap宽高
		int levelWidth = getLevelWidth(level);
		int levelHeight = getLevelHeight(level);

		//检查是否越界
		if (x < 0 || x + width > levelWidth || y < 0 || y + height > levelHeight || width <= 0 || height <= 0)
		{
			LogUtil::LogMsgLn("Illegal dimensions for setting data");
			return false;
		}

		//绑定纹理对象，默认Diffuse
		m_pGraphicDriver->BindTexture(this, TU_DIFFUSE);
		//是否全范围
		bool wholeLevel = x == 0 && y == 0 && width == levelWidth && height == levelHeight;

		uint dataType = GraphicDriver::GetHWTextureDataType(m_ePixelFormat);
		if (!m_bIsCompress)
		{
			if (wholeLevel)
				glTexImage2D(m_glType, level, internalFormat, width, height, 0, format, dataType, pData);
			else
				glTexSubImage2D(m_glType, level, x, y, width, height, internalFormat, dataType, pData);
		}
		else
		{
			if (wholeLevel)
				glCompressedTexImage2D(m_glType, level, internalFormat, width, height, 0, GetSize(), pData);
			else
				glCompressedTexSubImage2D(m_glType, level, x, y, width, height, internalFormat, dataType, pData);
		}
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);
		return true;
	}


	uint Texture2D::getLevelWidth(uint level) const
	{
		if (level > m_mipLevel)
			return 0;
		return MAX(m_uWidth >> level, 1);
	}

	uint Texture2D::getLevelHeight(uint level) const
	{
		if (level > m_mipLevel)
			return 0;
		return MAX(m_uHeight >> level, 1);
	}

	uint Texture2D::getUID() const
	{
		return m_uHwUID;
	}

}