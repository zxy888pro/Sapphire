#include <GraphicsDefs.h>
#include "CubeTexture.h"
#include "GPUObject.h"
#include <ITextureMgr.h>
#include <Image.h>
#include "ImageMgr.h"
#include "GraphicException.h"


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
					//找到等于这个纹理绑定的纹理单元
					if (m_pGraphicDriver->getTextureMgr()->GetTexture((TextureUnit)i) == this)
						m_pGraphicDriver->getTextureMgr()->SetTexture(0, (TextureUnit)i);  //清除对应的纹理单元
				}
				glDeleteTextures(1, &m_uHwUID); //删除硬件资源
			}
			//初始化硬件资源ID
			m_uHwUID = 0;
		}
	}

	bool CubeTexture::Recreate()
	{
		Release();
		Create();
		return true;
	}

	void CubeTexture::Dispose()
	{

	}

	bool CubeTexture::IsDisposed()
	{
		return false;
	}

	size_t CubeTexture::GetSize()
	{
		//返回纹理对象大小和该类对象大小
		return m_uSize + sizeof(CubeTexture);
	}


	uint CubeTexture::GetDataSize() const
	{
		if (m_bIsCompress)
		{
			return m_uSize;
		}
		return m_uSize;
	}



	uint CubeTexture::GetRowSize() const
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

	void CubeTexture::Load(HIMAGE himg, CubeMapFace face)
	{
		if (himg.IsNull())
		{
			LogUtil::LogMsgLn("Texture2D Load Image Failed! handle is Null!");
			return;
		}
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
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
		if (m_uWidth != m_uHeight)
		{
			throw GraphicDriverException("CubeMap Width Is Not Equal Height!", GraphicDriverException::GDError_CubeMapSizeError);
		}

		uint nChannels = pImageMgr->GetNumChannels(himg);
		m_ePixelFormat = m_pGraphicDriver->GetPixelFormat(pImageMgr->GetImageType(himg));
		if (m_ePixelFormat == PixelFormat::PF_UNDEFINED)
		{
			throw GraphicDriverException("Error Unknown Pixelformat !", GraphicDriverException::GDError_UnknownPixelFormat);
		}
		m_uSize = width * height * nChannels;
		if (pImgData == NULL)
		{
			LogUtil::LogMsgLn("Create Texture Failed! RawData is Null");
			return;
		}


		//创建纹理对象
		//Create();
		//应该先读取所有面纹理后再统一创建
		SetData(face, m_mipLevel, 0, 0, width, height, pImgData);
	}

	void CubeTexture::Load(HIMAGE leftImg, HIMAGE topImg, HIMAGE rightImg, HIMAGE bottomImg, HIMAGE frontImg, HIMAGE backImg)
	{
		if (m_pGraphicDriver == NULL)
		{
			LogUtil::LogMsgLn("GraphicDevice Is NULL! Load CubeTexture Failed!");
			return;
		}

		if (m_pGraphicDriver->IsDeviceLost())
		{
			LogUtil::LogMsgLn("GraphicDevice Is Lost! Load CubeTexture Failed!");
			return;
		}
		//先释放之前的纹理对象
		Release();
		try
		{
			Load(leftImg, FACE_NEGATIVE_X);
			Load(rightImg, FACE_POSITIVE_X);
			Load(topImg, FACE_POSITIVE_Y);
			Load(bottomImg, FACE_NEGATIVE_Y);
			Load(frontImg, FACE_POSITIVE_Z);
			Load(backImg, FACE_NEGATIVE_Z);
		}
		catch (GraphicDriverException& e)
		{
			LogUtil::LogMsgLn(StringFormatA("Error Code: %d Load CubeTexture Failed!", e.GetErrorCode()));
			Release();
			return;
		}
		
		//创建纹理对象
		Create();
	}

	void CubeTexture::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
	}

	void CubeTexture::OnDeviceReset()
	{
		if (m_uHwUID)
		{
			Recreate();
		}
	}

	bool CubeTexture::Create()
	{
		//先释放旧的
		Release();

		if (m_pGraphicDriver == NULL || m_uWidth == 0 || m_uHeight == 0)
		{
			LogUtil::LogMsgLn("GraphicDriver is not initialized!");
			return false;
		}

		if (m_pGraphicDriver->IsDeviceLost())
		{
			LogUtil::LogMsgLn("Texture Creation While Device is Lost!");
			return true;
		}
		glGenTextures(1, &m_uHwUID);
		//绑定纹理,设置属性
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_CUBEMAP);

		glTexParameteri(m_glType, GL_TEXTURE_WRAP_S, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_U]));
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_T, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_V]));
		glTexParameteri(m_glType, GL_TEXTURE_WRAP_R, GraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_W]));

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
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_CUBEMAP);
		return true;
	}

}