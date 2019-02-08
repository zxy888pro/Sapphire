#include <GraphicDriver.h>
#include "Texture2D.h"
#include <ITextureMgr.h>
#include "ImageMgr.h"
#include <mathHelper.h>
#include "GraphicException.h"
#include "RenderSurface.h"

namespace Sapphire
{

	Texture2D::Texture2D() :
		GPUObject(),
		m_uWidth(0),
		m_uHeight(0),
		m_uDepth(0),
		m_bIsCompress(false),
		m_mipLevel(0),
		m_maxMipLevel(0),
		m_uNumMipmaps(1),
		m_bIsDisposed(true),
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
		for (int i = 0; i < MAX_TEXTURE_QUALITY_LEVELS; ++i)
			m_skipMips[i] = (unsigned)(MAX_TEXTURE_QUALITY_LEVELS - 1 - i);
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
		m_bIsDisposed = true;
		m_eUsage = eUsage;
		m_mipLevel = 0;
		m_maxMipLevel = 0;
		m_uAnisotropyLevel = 8;
		m_glType = glTargerType;
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();
		for (int i = 0; i < MAX_TEXTURE_QUALITY_LEVELS; ++i)
			m_skipMips[i] = (unsigned)(MAX_TEXTURE_QUALITY_LEVELS - 1 - i);  //质量越高,跳过的mip越小
	}

	Texture2D::~Texture2D()
	{
		Dispose();
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
		//释放renderSurface
		if (m_renderSurface)
			m_renderSurface->Release();

	}

	bool Texture2D::Recreate()
	{
		Dispose();
		if (m_szName != "")
		{
			ReRequest();  //重新请求纹理
		}
		return true;
	}

	void Texture2D::Dispose()
	{
		Release();
		m_bIsDisposed =true;
	}


	bool Texture2D::IsDisposed()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bIsDisposed = true;
		}
		return m_bIsDisposed;
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
		Dispose();
		if (himg.IsNull())
		{
			SAPPHIRE_LOGERROR("Texture2D Load Imsage Failed! handle is Null!");
			return;
		}
		IImageMgr* pImageMgr =  m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return;
		}
		PRAWIMAGE pImgData = pImageMgr->GetImageRaw(himg);
		uint width = pImageMgr->GetWidth(himg);
		uint height = pImageMgr->GetHeight(himg);
		m_uWidth = width;
		m_uHeight = height;
		//读取Mipmaps数
		m_uNumMipmaps = pImageMgr->GetNumMipmaps(himg);
		//通道数相当于占用字节数
		uint nChannels = pImageMgr->GetNumChannels(himg);
		m_ePixelFormat = m_pGraphicDriver->GetPixelFormat(pImageMgr->GetImageType(himg));
		if (m_ePixelFormat == PixelFormat::PF_UNDEFINED)
		{
			throw GraphicDriverException("Error Unknown Pixelformat !", GraphicDriverException::GDError_UnknownPixelFormat);
		}
		m_uSize = width * height * nChannels;
		if (pImgData == NULL)
		{
			SAPPHIRE_LOGERROR("Create Texture Failed! RawData is Null");
			return;
		}
		//创建纹理对象
		//Create();
		//默認alpha==false，只有一個通道的話默認為明度
		SetData(himg, false); //SetSize時創建紋理對象
		//SetData(m_mipLevel, 0, 0, width, height, pImgData);

	}

	void Texture2D::ReRequest()
	{
		//重新去請求資源
		if (IsDisposed())
		{
			//请求纹理
			IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
			Core* pCore = Core::GetSingletonPtr();
			if (pImageMgr == NULL || pCore == NULL)
			{
				throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
			}
			HIMAGE himg = pImageMgr->GetImage(m_szName.c_str());
			if (himg.IsNull())
			{
				LogUtil::LogMsgLn(StringFormatA("Request Texture Failed! Not found %s", m_szName.c_str()));
				return;
			}
			Load(himg);
		}

	}

	void Texture2D::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
		if (m_renderSurface)
			m_renderSurface->OnDeviceLost();
	}

	void Texture2D::OnDeviceReset()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bDataLost = true; //数据已丢失
		}
		Recreate();  //重新创建
		m_bDataPending = false;
	}

	bool Texture2D::Create()
	{
		//先释放旧的
		//Dispose();

		if (m_pGraphicDriver == NULL || m_uWidth == 0 || m_uHeight == 0)
		{
			SAPPHIRE_LOGERROR("GraphicDriver is not initialized!");
			return false;
		}
			
		if (m_pGraphicDriver->IsDeviceLost())
		{
			SAPPHIRE_LOGERROR("Texture Creation While Device is Lost!");
			return true;
		}
#ifndef GL_ES_VERSION_2_0
		// 如果不支持深度纹理或者这是一个一个打包的深度模板纹理的话，创建一个renderBuffer替代原本texture
		if (GraphicDriver::GetHWTextureFormat(m_ePixelFormat) == m_pGraphicDriver->GetHWDepthStencilFormat())
#else
		if (GraphicDriver::GetHWTextureFormat(m_ePixelFormat) == GL_DEPTH_COMPONENT16 || GraphicDriver::GetHWTextureFormat(m_ePixelFormat) == GL_DEPTH_COMPONENT24_OES || GraphicDriver::GetHWTextureFormat(m_ePixelFormat) == GL_DEPTH24_STENCIL8_OES ||
			(GraphicDriver::GetHWTextureFormat(m_ePixelFormat) == GL_DEPTH_COMPONENT && !m_pGraphicDriver->GetHWShadowMapFormat()))
#endif
		{	
			if (m_renderSurface)
			{
				m_renderSurface->CreateRenderBuffer(m_uWidth, m_uHeight, m_ePixelFormat);
				return true;
			}
			else
				return false;
		}


		glGenTextures(1, &m_uHwUID);
		//glBindTexture(GL_TEXTURE_2D, m_uHwUID);
		//Diffuse作为默认单元
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE);
		/*ITextureMgr* pTexMgr = m_pGraphicDriver->getTextureMgr();
		int b = pTexMgr->VerifyHWUID(m_uHwUID);*/

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
		
////設置mipmap
//#ifndef GL_ES_VERSION_2_0
//		glTexParameteri(m_glType, GL_TEXTURE_BASE_LEVEL, 0);
//		glTexParameteri(m_glType, GL_TEXTURE_MAX_LEVEL, m_uNumMipmaps - 1);
//#endif
		//解除绑定
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);

		//calculate mipmap level
		/*m_mipLevel = 0;
		if (!m_mipLevel)
		{
		unsigned maxSize = (unsigned)MAX(m_uWidth, m_uHeight);
		while (maxSize)
		{
		maxSize >>= 1;
		++m_mipLevel;
		}
		}*/
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
		//大小改变重新创建空纹理
		Dispose();
		Create();
		//SetData(m_mipLevel, 0, 0, width, height, NULL); //空纹理
		return true;
	}

	bool Texture2D::SetData(uint level, int x, int y, int width, int height, const void* pData)
	{
		if (!m_pGraphicDriver)
		{
			SAPPHIRE_LOGERROR("Error GraphicDriver is Null!");
			return false;
		}
		ITextureMgr* pTexMgr = m_pGraphicDriver->getTextureMgr();
		if (!pTexMgr)
		{
			LogUtil::LogMsgLn("Error TextureMgr is Null!");
			return false;
		}
		//绑定纹理对象，默认Diffuse
		m_pGraphicDriver->BindTexture(this, TU_DIFFUSE);
		if (!pTexMgr->VerifyHWUID(m_uHwUID))
		{
			SAPPHIRE_LOGERROR("Error HwUID is invalid!");
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
			SAPPHIRE_LOGERROR("Illegal dimensions for setting data");
			return false;
		}

		
		//是否全范围
		bool wholeLevel = x == 0 && y == 0 && width == levelWidth && height == levelHeight;

		uint dataType = GraphicDriver::GetHWTextureDataType(m_ePixelFormat);
		if (!m_bIsCompress)
		{
			//是更新整个纹理数据区
			if (wholeLevel)
				glTexImage2D(m_glType, level, internalFormat, width, height, 0, format, dataType, pData);
			else
				//更新部分
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


	bool Texture2D::SetData(HIMAGE himg, bool useAlpha)
	{
		//设置图像数据
		if (himg.IsNull())
		{
			SAPPHIRE_LOGERROR("Null image, can not set data");
			return false;
		}
		//获取当前纹理质量
		int quality = m_pGraphicDriver->getTextureQuality();
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return false;
		}
		unsigned memoryUse = sizeof(Texture2D);
		//先绑定纹理对象
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE); 

		if (!pImageMgr->IsCompressd(himg))
		{
			//mip等级0，最大分辨率
			PRAWIMAGE levelData = pImageMgr->GetImageRaw(himg);
			int levelWidth = pImageMgr->GetWidth(himg);
			int levelHeight = pImageMgr->GetHeight(himg);
			int channels = pImageMgr->GetNumChannels(himg);  //获取通道数
			PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(pImageMgr->GetImageType(himg));
			int format = 0;
			int nextLv = 1;
			//根据画质设置，跳过指定mip等级, 质量越高，跳过的mipmap越少
			for (unsigned i = 0; i < m_skipMips[quality]; ++i)
			{
				levelData = pImageMgr->GetImageRaw(himg, i);
				levelWidth = pImageMgr->GetWidth(himg, i);
				levelHeight = pImageMgr->GetHeight(himg, i);
				++nextLv;
			}

			//通过通道转换位对应的格式
			/*switch (channels)
			{
			case 1:
			format = useAlpha ? GraphicDriver::GetHWAlphaFormat() : GraphicDriver::GetHWLuminanceFormat();
			break;

			case 2:
			format = GraphicDriver::GetHWLuminanceAlphaFormat();
			break;

			case 3:
			format = GraphicDriver::GetHWRGBFormat();
			break;

			case 4:
			format = GraphicDriver::GetHWRGBAFormat();
			break;

			default:
			{
			SAPPHIRE_LOGERROR("HWFormat format illegal !");
			assert(false);
			}
			break;
			}*/
			//m_glType = format;
			//纹理质量设置，会改变纹理0的大小
			SetSize(levelWidth, levelHeight, ePixelFormat);
			for (unsigned i = 0; i < m_uNumMipmaps; ++i)
			{
				//mip0從跳過的mipmap lv開始
				SetData(i, 0, 0, levelWidth, levelHeight, levelData);
				memoryUse += levelWidth * levelHeight * channels;

				//if (i < m_uNumMipmaps - 1)
				if (nextLv < m_uNumMipmaps)
				{
					levelData = pImageMgr->GetImageRaw(himg, nextLv);
					levelWidth = pImageMgr->GetWidth(himg, nextLv);
					levelHeight = pImageMgr->GetHeight(himg, nextLv);
					++nextLv;
				}
				else
				{
					break;
				}
			}
			//設置mipmap
#ifndef GL_ES_VERSION_2_0
			glTexParameteri(m_glType, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_glType, GL_TEXTURE_MAX_LEVEL, nextLv - 1);
#endif
		}
		else
		{
			//压缩纹理格式dds/s3tc/etc.....
			return false;
		}
		//接触绑定
		m_pGraphicDriver->BindTexture(0, TextureUnit::TU_DIFFUSE);
		
	}

	uint Texture2D::getLevelWidth(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uWidth >> level, 1);
	}

	uint Texture2D::getLevelHeight(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uHeight >> level, 1);
	}

	Sapphire::ITexture* Texture2D::getBackupTexture() const
	{
		return m_backupTex;
	}

	void Texture2D::setBackupTexture(ITexture* tex)
	{
		m_backupTex = dynamic_cast<Texture2D*>(tex);
	}

	uint Texture2D::getUID() const
	{
		return m_uHwUID;
	}

	void Texture2D::RenderSurfaceUpdate()
	{
		if (m_renderSurface && m_renderSurface->GetUpdateMode() == SURFACE_UPDATEALWAYS)
			m_renderSurface->QueueUpdate();
	}

}