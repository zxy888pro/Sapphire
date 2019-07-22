#include <GraphicsDefs.h>
#include "GLGraphicDriver.h"
#include "CubeTexture.h"
#include "GPUObject.h"
#include <ITextureMgr.h>
#include <Image.h>
#include "../ImageMgr.h"
#include <FileStream.h>
#include "GraphicException.h"
#include <stringHelper.h>
#include "GLRenderSurface.h"
#include <json/json.h>


namespace Sapphire
{

	 
	CubeTexture::CubeTexture(Core* pCore, IGraphicDriver* pDriver, const char* name) :
		BaseResource(pCore, name),
		GPUObject(pDriver),
		m_uWidth(0),
		m_uHeight(0),
		m_uDepth(0),
		m_bIsCompress(false),
		m_mipLevel(0),
		m_maxMipLevel(0),
		m_uNumMipmaps(1),
		m_requestLevel(0),
		m_bIsDisposed(true),
		m_uAnisotropyLevel(8),
		m_ePixelFormat(PixelFormat::PF_R8G8B8A8),
		m_eFilterMode(TextureFilterMode::FILTER_NEAREST),
		m_eUsage(TextureUsage::TEXTURE_STATIC),
		m_szName(""),
		m_channelNum(0),
		m_glTexTarget(GL_TEXTURE_CUBE_MAP)
	{
		 
		m_eType = ResoureType_Texture;
		for (int i = 0; i < MAX_COORDS; ++i)
			m_eAddressMode_[i] = ADDRESS_REPEAT; m_eAddressMode_;
		for (int i = 0; i < MAX_TEXTURE_QUALITY_LEVELS; ++i)
			m_skipMips[i] = (unsigned)(MAX_TEXTURE_QUALITY_LEVELS - 1 - i);
		m_pGraphicDriver  = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(m_pGraphicDriver);

	}

	CubeTexture::CubeTexture(Core* pCore, IGraphicDriver* pDriver, const char* name, uint size, uint depth, PixelFormat pf /*= PF_R8G8B8A8*/, uint NumMipmaps /*= 1*/, int glTargerType /*= GL_TEXTURE_2D*/, TextureUsage eUsage /*= TextureUsage::TEXTURE_STATIC*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode r /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/) :
		BaseResource(pCore, name),
		GPUObject(pDriver)
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
		m_bIsDisposed = true;
		m_eUsage = eUsage;
		m_mipLevel = 0;
		m_requestLevel = 0;
		m_maxMipLevel = 0;
		m_uAnisotropyLevel = 8;
		m_glTexTarget = GL_TEXTURE_CUBE_MAP;
		m_channelNum = 0;
		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(pDriver);
		m_assert(m_pGraphicDriver);
		for (int i = 0; i < MAX_TEXTURE_QUALITY_LEVELS; ++i)
			m_skipMips[i] = (unsigned)(MAX_TEXTURE_QUALITY_LEVELS - 1 - i);

	}

	CubeTexture::~CubeTexture()
	{
		Dispose();
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
		for (unsigned i = 0; i < MAX_CUBEMAP_FACES; ++i)
		{
			if (m_renderSurfaces[i])
				m_renderSurfaces[i]->Release();
		}
	}

	bool CubeTexture::Recreate()
	{
		Dispose();
		if (m_szName != "")
		{
			ReRequest();  //重新请求纹理
		}
		return true;
	}

	void CubeTexture::ReRequest()
	{
		Dispose();
		m_uWidth = 0;
		m_uHeight = 0;
		if (m_szName != "")
		{
			FileStream fs(m_szName.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ);
			if (fs.IsOpen())
			{
				//暂时写在这里加载
				IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
				Core* pCore = Core::GetSingletonPtr();
				if (pImageMgr == NULL || pCore == NULL)
				{
					throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
				}
				std::string jsonStr = fs.ReadString(MAX_JSON_LENGTH);
				fs.Release();
				Json::CharReaderBuilder builder;
				Json::CharReader* reader = builder.newCharReader();
				JSONCPP_STRING errs;
				Json::Value rootNode;
				HIMAGE  imgs[6];
				if (reader->parse(jsonStr.c_str(), jsonStr.c_str() + strlen(jsonStr.c_str()), &rootNode, &errs))
				{
					int width = rootNode["size"].asInt();
					int height = width;
					if (width == 0 || height == 0)
					{
						throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
					}
					m_uWidth = width;
					m_uHeight = height;
					m_channelNum = rootNode["channel"].asInt();
					if (m_channelNum < 3 || m_channelNum > 4)
					{
						throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
					}
					//取得图像格式
					std::string imageType = rootNode["imageType"].asString();
					//取得图像格式对应的纹理格式
					m_ePixelFormat = m_pGraphicDriver->GetPixelFormat(imageType);
					if (m_ePixelFormat == PF_UNDEFINED)
					{
						throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
					}
					std::string faceNames[6];
					faceNames[FACE_NEGATIVE_X] = rootNode["-x"].asString();
					faceNames[FACE_POSITIVE_Y] = rootNode["+y"].asString();
					faceNames[FACE_POSITIVE_X] = rootNode["+x"].asString();
					faceNames[FACE_NEGATIVE_Y] = rootNode["-y"].asString();
					faceNames[FACE_POSITIVE_Z] = rootNode["+z"].asString();
					faceNames[FACE_NEGATIVE_Z] = rootNode["-z"].asString();
					for (int i = 0; i < 6; ++i)
					{
						imgs[i] = pImageMgr->GetImage(faceNames[i].c_str());
						if (imgs[i].IsNull())
						{
							LogUtil::LogMsgLn(StringFormatA("Load ImageFile Failed! Not found %s", faceNames[i].c_str()));
							throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
						}
					}
					//直接重新加载6个面
					Load(imgs[FACE_NEGATIVE_X], imgs[FACE_POSITIVE_Y], imgs[FACE_POSITIVE_X], imgs[FACE_NEGATIVE_Y], imgs[FACE_POSITIVE_Z], imgs[FACE_NEGATIVE_Z]);
					//计算大小
					m_uSize = width * height * m_channelNum * 6;
				}
				else
				{
					throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
				}
			} 

		}
	}

	void CubeTexture::Dispose()
	{
		Release();
		m_bIsDisposed = true;
	}

	bool CubeTexture::IsDisposed()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bIsDisposed = true;
		}
		return m_bIsDisposed;
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
			SAPPHIRE_LOGERROR("Texture2D Load Image Failed! handle is Null!");
			return;
		}
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return;
		}
		if (!pImageMgr->IsCompressd(himg))
		{
			//获取当前纹理质量
			int quality = m_pGraphicDriver->getTextureQuality();
			PRAWIMAGE levelData = pImageMgr->GetImageRaw(himg);
			uint levelWidth = pImageMgr->GetWidth(himg);
			uint levelHeight = pImageMgr->GetHeight(himg);
			//6个子纹理大小和纹理必须一致
			if (levelWidth != levelHeight && levelHeight != m_uHeight && levelWidth != m_uWidth)
			{
				throw GraphicDriverException("CubeMap Width Is Not Equal Height!", GraphicDriverException::GDError_CubeMapSizeError);
			}
			int channels = pImageMgr->GetNumChannels(himg);  //获取通道数
			PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(pImageMgr->GetImageType(himg));
			m_uWidth = levelWidth;
			m_uHeight = levelHeight;
			int nextLv = 1;
			unsigned memoryUse = sizeof(CubeTexture);
			//根据画质设置，跳过指定mip等级, 质量越高，跳过的mipmap越少
			for (unsigned i = 0; i < m_skipMips[quality]; ++i)
			{
				levelData = pImageMgr->GetImageRaw(himg, i);
				levelWidth = pImageMgr->GetWidth(himg, i);
				levelHeight = pImageMgr->GetHeight(himg, i);
				++nextLv;
			}
			//face为第一个面是创建纹理完成初始化，之后不再创建
			if (face == FACE_POSITIVE_X)
			{
				//纹理质量设置，会改变纹理0的大小
				SetSize(levelWidth, levelHeight, ePixelFormat);
			}
			for (unsigned i = 0; i < m_uNumMipmaps; ++i)
			{
				//mip0從跳過的mipmap lv開始
				SetData(face,i, 0, 0, levelWidth, levelHeight, levelData);
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



			//应先创建纹理对象后才能设置数据
			//应该先读取所有面纹理后再统一创建
			//SetData(face, m_mipLevel, 0, 0, levelWidth, levelHeight, levelData);
		}
		else
		{
			//压缩纹理格式dds/s3tc/etc.....
			return;
		}
		
 
		
	}

	void CubeTexture::Load(HIMAGE leftImg, HIMAGE topImg, HIMAGE rightImg, HIMAGE bottomImg, HIMAGE frontImg, HIMAGE backImg)
	{
		if (m_pGraphicDriver == NULL)
		{
			SAPPHIRE_LOGERROR("GraphicDevice Is NULL! Load CubeTexture Failed!");
			return;
		}

		if (m_pGraphicDriver->IsDeviceLost())
		{
			SAPPHIRE_LOGERROR("GraphicDevice Is Lost! Load CubeTexture Failed!");
			return;
		}
		
		//先释放之前的纹理对象
		Dispose();
		//创建纹理对象
		//Create();
		try
		{
			Load(rightImg, FACE_POSITIVE_X);
			Load(leftImg, FACE_NEGATIVE_X);
			Load(topImg, FACE_POSITIVE_Y);
			Load(bottomImg, FACE_NEGATIVE_Y);
			Load(frontImg, FACE_POSITIVE_Z);
			Load(backImg, FACE_NEGATIVE_Z);
		}
		catch (GraphicDriverException& e)
		{
			LogUtil::LogMsgLn(StringFormatA("Error Code: %d Load CubeTexture Failed!", e.GetErrorCode()));
			//发生异常丢弃纹理资源
			Dispose();
			return;
		}
	}

	void CubeTexture::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
		for (unsigned i = 0; i < MAX_CUBEMAP_FACES; ++i)
		{
			if (m_renderSurfaces[i])
				m_renderSurfaces[i]->OnDeviceLost();
		}
	}

	void CubeTexture::OnDeviceReset()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bDataLost = true; //数据已丢失
		}
		Recreate();  //重新创建
		m_bDataPending = false;
	}

	bool CubeTexture::Create()
	{
		//先释放旧的
		//Dispose();

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

		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_S, GLGraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_U]));
		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_T, GLGraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_V]));
		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_R, GLGraphicDriver::GetHWTextureWarpParam(m_eAddressMode_[TextureCoordinate::COORD_W]));

		switch (m_eFilterMode)
		{
		case Sapphire::FILTER_NEAREST:
		{
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}
		break;
		case Sapphire::FILTER_BILINEAR:
		{
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		case Sapphire::FILTER_TRILINEAR:
		{
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		break;
		case Sapphire::FILTER_ANISOTROPIC:
		{
			if (m_pGraphicDriver->GetAnisotropySupport())
			{
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				m_uAnisotropyLevel = MIN(m_pGraphicDriver->GetMaxAnisotropyLevels(), m_uAnisotropyLevel);
				glTexParameterf(m_glTexTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_uAnisotropyLevel);
			}
		}
		break;
		case Sapphire::FILTER_DEFAULT:
		{
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		default:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		break;
		}
#ifndef GL_ES_VERSION_2_0
		glTexParameteri(m_uHwUID, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_uHwUID, GL_TEXTURE_MAX_LEVEL, m_uNumMipmaps-1);
#endif
		//解除绑定
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_CUBEMAP);

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

	bool CubeTexture::SetSize(int width, int height, PixelFormat eformat, TextureUsage usage /*= TEXTURE_STATIC*/)
	{
		//先删除所有的旧renderSurface
		for (unsigned i = 0; i < MAX_CUBEMAP_FACES; ++i)
		{
			m_renderSurfaces[i].Reset();
			m_faceMemoryUse[i] = 0;
		}

		if (usage == TEXTURE_RENDERTARGET)
		{
			for (unsigned i = 0; i < MAX_CUBEMAP_FACES; ++i)
			{
				m_renderSurfaces[i] = new GLRenderSurface(m_pCore,this);
				m_renderSurfaces[i]->SetTarget(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
			}
			m_eFilterMode = FILTER_NEAREST;
			m_requestLevel = 1;
		}

 

		m_uWidth = width;
		m_uHeight = height;
		m_ePixelFormat = eformat;
		m_eUsage = usage;
		Create();
		//SetData(CubeMapFace::FACE_NEGATIVE_X, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		//SetData(CubeMapFace::FACE_POSITIVE_Y, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		//SetData(CubeMapFace::FACE_POSITIVE_X, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		//SetData(CubeMapFace::FACE_NEGATIVE_Y, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		//SetData(CubeMapFace::FACE_POSITIVE_Z, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		//SetData(CubeMapFace::FACE_NEGATIVE_Z, m_mipLevel, 0, 0, width, height, NULL); //空纹理
		return true;
	}

	bool CubeTexture::SetData(CubeMapFace face, uint level, int x, int y, int width, int height, const void* pData)
	{
		if (!m_pGraphicDriver)
		{
			SAPPHIRE_LOGERROR("Error GraphicDriver is Null!");
			return false;
		}
		if (m_pGraphicDriver->IsDeviceLost())
		{
			SAPPHIRE_LOGWARNING("Texture data assignment while device is lost");
			m_bDataPending = true;
			return true;
		}
		ITextureMgr* pTexMgr = m_pGraphicDriver->getTextureMgr();
		if (!pTexMgr)
		{
			LogUtil::LogMsgLn("Error TextureMgr is Null!");
			return false;
		}
		if (!pTexMgr->VerifyHWUID(m_uHwUID))
		{
			SAPPHIRE_LOGERROR("Error HwUID is invalid!");
			return false;
		}
		int format = GLGraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		int internalFormat = GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat);

		//获取level级下mipmap宽高
		int levelWidth = getLevelWidth(level);
		int levelHeight = getLevelHeight(level);

		//检查是否越界
		if (x < 0 || x + width > levelWidth || y < 0 || y + height > levelHeight || width <= 0 || height <= 0)
		{
			SAPPHIRE_LOGERROR("Illegal dimensions for setting data");
			return false;
		}

		if (IsCompressed())  //紋理是否壓縮
		{
			x &= ~3;
			y &= ~3;
		}

		//绑定纹理对象，默认Diffuse
		m_pGraphicDriver->BindTexture(this, TU_DIFFUSE);
		//是否全范围
		bool wholeLevel = x == 0 && y == 0 && width == levelWidth && height == levelHeight;

		uint dataType = GLGraphicDriver::GetHWTextureDataType(m_ePixelFormat);
		if (!IsCompressed())
		{
			if (wholeLevel)
				glTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, internalFormat, width, height, 0, format, dataType, pData);
			else
				glTexSubImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, x, y, width, height, internalFormat, dataType, pData); //更新局部
		}
		else
		{
			if (wholeLevel)
				glCompressedTexImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, internalFormat, width, height, 0, GetSize(), pData);
			else
				glCompressedTexSubImage2D((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, x, y, width, height, internalFormat, dataType, pData);  //更新局部
		}
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);

		return true;
	}

	bool CubeTexture::GetData(CubeMapFace face, unsigned level, void* dest) const
	{
		if (m_pGraphicDriver->IsDeviceLost())
		{
			SAPPHIRE_LOGWARNING("Getting texture data while device is lost");
			return false;
		}
		//注意这个函数不执行边界检查，可能会溢出
		if (!m_bIsCompress)
			glGetTexImage((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat), GLGraphicDriver::GetHWTextureDataType(m_ePixelFormat), dest);
		else
			glGetCompressedTexImage((GLenum)(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face), level, dest);

		//解除绑定
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_CUBEMAP);
	}

	uint CubeTexture::getLevelWidth(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uWidth >> level, 1);
	}

	uint CubeTexture::getLevelHeight(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uWidth >> level, 1);
	}

	Sapphire::ITexture* CubeTexture::getBackupTexture() const
	{
		return m_backupTex;
	}

	void CubeTexture::setBackupTexture(ITexture* tex)
	{
		m_backupTex = dynamic_cast<CubeTexture*>(tex);
	}

	uint CubeTexture::getUID() const
	{
		return m_uHwUID;
	}

	void CubeTexture::RenderSurfaceUpdate()
	{
		for (unsigned i = 0; i < MAX_CUBEMAP_FACES; ++i)
		{
			if (m_renderSurfaces[i] && m_renderSurfaces[i]->GetUpdateMode() == SURFACE_UPDATEALWAYS)
				m_renderSurfaces[i]->QueueUpdate();
		}
	}

	void CubeTexture::SetSRGB(bool enable)
	{

	}

	bool CubeTexture::GetSRGB() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint CubeTexture::GetHWFormat() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void CubeTexture::UpdateParameters()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool CubeTexture::GetParametersDirty() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	uint CubeTexture::getGPUHandle() const
	{
		return m_uHwUID;
	}

}