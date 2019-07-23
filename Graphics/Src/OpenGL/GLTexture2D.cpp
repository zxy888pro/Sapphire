#include <mathHelper.h>
#include "ResourceCache.h"
#include "GLGraphicDriver.h"
#include "GraphicException.h"
#include "GLTexture2D.h"
#include "GLRenderSurface.h"
#include "TextureMgr.h"


namespace Sapphire
{

	GLTexture2D::GLTexture2D(Core* pCore, IGraphicDriver* pDriver, const char* resName /*= ""*/) :
		GLTexture(pCore, pDriver, resName)
	{
		m_ePixelFormat = PF_R8G8B8A8;
		m_glTexTarget = GL_TEXTURE_2D;
	}


	GLTexture2D::~GLTexture2D()
	{
		Dispose();
	}

	void GLTexture2D::Release()
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
				glDeleteTextures(1, &m_uHwUID); //删除纹理对象
			}
			//初始化硬件资源ID
			m_uHwUID = 0;
		}
		//释放renderSurface
		if (m_renderSurface)
			m_renderSurface->Release();
	}

	bool GLTexture2D::Recreate()
	{
		Dispose();
		if (m_szName != "")
		{
			ReRequest();  //重新请求纹理
		}
		return true;
	}

	void GLTexture2D::Dispose()
	{
		Release();
		m_bIsDisposed = true;
	}

	bool GLTexture2D::IsDisposed()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bIsDisposed = true;
		}
		return m_bIsDisposed;
	}

	size_t GLTexture2D::GetSize()
	{
		return m_uSize;
	}

	void GLTexture2D::ReRequest()
	{
		if (IsDisposed())
		{
			Load();
		}
	}

	void GLTexture2D::OnDeviceLost()
	{
		GPUObject::OnDeviceLost();
		if (m_renderSurface)
			m_renderSurface->OnDeviceLost();
	}

	void GLTexture2D::OnDeviceReset()
	{
		if (glIsTexture(m_uHwUID) == false)
		{
			m_bDataLost = true; //数据已丢失
		}
		Recreate();  //重新创建
		m_bDataPending = false;
	}

	bool GLTexture2D::Create()
	{
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
		GLGraphicDriver* pGLDriver = dynamic_cast<GLGraphicDriver*>(m_pGraphicDriver);
#ifndef GL_ES_VERSION_2_0
		// 如果不支持深度纹理或者这是一个一个打包的深度模板纹理的话，创建一个renderBuffer替代原本texture
		if (GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat) == pGLDriver->GetHWDepthStencilFormat())
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

		//设置纹理到纹理单元0好对其进行更新
		pGLDriver->getTextureMgr()->SetTextureForUpdate(this);
		int format = GLGraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		//是否使用SRGB空间
		format = GetSRGB() ? GetSRGBFormat(format) : format;
		int internalFormat = GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat);
		int dataType = GLGraphicDriver::GetHWTextureDataType(m_ePixelFormat);

		bool ret = true;
		if (!IsCompressed())
		{
			glGetError();
			glTexImage2D(m_glTexTarget, 0, internalFormat, m_uWidth, m_uHeight, 0, format, dataType, 0);
			if (glGetError())
			{
				SAPPHIRE_LOGERROR("Failed to create texture");
				ret = false;
			}
		}

		//设置mipmap
		m_uNumMipmaps = m_requestLevel;
		if (!m_uNumMipmaps)
		{
			unsigned maxSize = MAX((int)m_uWidth, (int)m_uHeight);
			while (maxSize)
			{
				maxSize >>= 1;
				++m_uNumMipmaps;
			}
		}

#ifndef GL_ES_VERSION_2_0
		glTexParameteri(m_glTexTarget, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(m_glTexTarget, GL_TEXTURE_MAX_LEVEL, m_uNumMipmaps - 1);
#endif
 
		//更新纹理参数
		UpdateParameters();
		//设置完纹理对象，取消绑定
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);

		return true;
	}

	void GLTexture2D::RenderSurfaceUpdate()
	{
		if (m_renderSurface && m_renderSurface->GetUpdateMode() == SURFACE_UPDATEALWAYS)
			m_renderSurface->QueueUpdate();
	}

	bool GLTexture2D::Load(const char* resName)
	{
		m_resName = resName;
		return Load();
	}

	bool GLTexture2D::Load()
	{
		if (m_pGraphicDriver == NULL)
			return false;

		if (m_pGraphicDriver->IsDeviceLost())
		{
			return false;
		}
		//先释放之前的纹理对象
		Dispose();
		Path xmlFile = m_resName;
		std::string fileName = xmlFile.getFileName();
		xmlFile = xmlFile.getParentDir();
		xmlFile.addTailSlash();
		xmlFile += fileName + ".xml";
		bool ret = false;

		m_parameters = SharedPtr<XMLFile>(new XMLFile(m_pCore, xmlFile.c_str()));
		ret = m_parameters->Load();
		SetParameters(m_parameters); //解析纹理配置参数
		m_imageRes = SharedPtr<ImageRes>(new ImageRes(m_pCore, m_resName.c_str()));
		ret = m_imageRes->Load();


		m_uWidth = m_imageRes->getWidth();
		m_uHeight = m_imageRes->getHeight();
		m_uNumMipmaps = m_imageRes->getMipmapNum();
		uint nChannels = m_imageRes->getChannels();
		m_ePixelFormat = m_pGraphicDriver->GetPixelFormat(m_imageRes->getImageType());
		if (m_ePixelFormat == PixelFormat::PF_UNDEFINED)
		{
			throw GraphicDriverException("Error Unknown Pixelformat !", GraphicDriverException::GDError_UnknownPixelFormat);
		}
		//m_uSize = m_uWidth * m_uHeight * nChannels;
		return SetData(m_imageRes);
	}

	bool GLTexture2D::SetSize(int width, int height, PixelFormat eformat, TextureUsage eUsage /*= TEXTURE_STATIC*/)
	{
		m_renderSurface.Reset(); //释放旧资源
	 
		m_eUsage = eUsage;
		m_uWidth = width;
		m_uHeight = height;
		m_ePixelFormat = eformat;
		

		if (eUsage >= TEXTURE_RENDERTARGET)
		{
			// 恢复默认设置
			m_renderSurface = new GLRenderSurface(m_pCore,this); //重新创建对应渲染表面
			m_eAddressMode_[COORD_U] = ADDRESS_CLAMP;
			m_eAddressMode_[COORD_V] = ADDRESS_CLAMP;
			m_eFilterMode = FILTER_NEAREST;
			m_requestLevel = 1;
		}
		//订阅一个事件渲染表面更新, 有更新时调用RenderSurfaceUpdate
		if (m_eUsage == TEXTURE_RENDERTARGET)
			SubscribeEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_RENDERSURFACEUPDATE);
		else
			UnSubscribeEvent(ET_GRAPHIC_EVENT,EVENT_GRAPHICS_RENDERSURFACEUPDATE);
		//大小和像素格式参数改变需重新创建空纹理
		Dispose();
		Create();
		//SetData(m_mipLevel, 0, 0, width, height, NULL); //空纹理
		return true;
	}

	bool GLTexture2D::SetData(SharedPtr<ImageRes> img, bool useAlpha)
	{
		PRAWIMAGE pImgData = img->getData();
		if (pImgData == NULL)
		{
			SAPPHIRE_LOGERROR("Create Texture Failed! RawData is Null");
			return false;
		}
		//创建纹理对象
		//Create();
		//默認alpha==false，只有一個通道的話默認為明度
		//获取当前纹理质量
		int quality = m_pGraphicDriver->getTextureQuality();
		/*IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return false;
		}*/
		//统计内存占用
		unsigned memoryUse = sizeof(GLTexture2D); 
		//先绑定纹理对象
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE);

		if (!img->IsCompressd())
		{
			int channels = img->getChannels();  //获取通道数
			//mip等级0，最大分辨率
			PRAWIMAGE levelData = img->getData();
			int levelWidth = img->getWidth();
			int levelHeight = img->getHeight();

			PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(img->getImageType());
			int format = 0;
			int nextLv = 1;
			//根据画质设置，跳过指定mip等级, 质量越高，跳过的mipmap越少
			//丢弃不必要的mip等级
			for (unsigned i = 0; i < m_skipMips[quality]; ++i)
			{
				levelData = img->getMipmap(i);
				levelWidth = img->getWidth(i);
				levelHeight = img->getHeight(i);
				++nextLv;
			}
			//通过通道转换位对应的格式
			/*switch (channels)
			{
			case 1:
			format = useAlpha ? GLGraphicDriver::GetHWAlphaFormat() : GLGraphicDriver::GetHWLuminanceFormat();
			break;

			case 2:
			format = GLGraphicDriver::GetHWLuminanceAlphaFormat();
			break;

			case 3:
			format = GLGraphicDriver::GetHWRGBFormat();
			break;

			case 4:
			format = GLGraphicDriver::GetHWRGBAFormat();
			break;

			default:
			{
			SAPPHIRE_LOGERROR("HWFormat format illegal !");
			assert(false);
			}
			break;
			}
			m_glTexFormat = format;*/
			//纹理质量设置，会改变纹理0的大小
			SetSize(levelWidth, levelHeight, m_ePixelFormat);
			for (unsigned i = 0; i < m_uNumMipmaps; ++i)
			{
				//mip0從跳過的mipmap lv開始,计算内存占用
				SetData(i, 0, 0, levelWidth, levelHeight, levelData);
				memoryUse += levelWidth * levelHeight * channels;

				//if (i < m_uNumMipmaps - 1)
				if (nextLv < m_uNumMipmaps)
				{
					levelData = img->getMipmap(nextLv);
					levelWidth = img->getWidth(nextLv);
					levelHeight = img->getHeight(nextLv);
					++nextLv;
				}
				else
				{
					break;
				}
			}
			//設置mipmap
#ifndef GL_ES_VERSION_2_0
			glTexParameteri(m_glTexTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAX_LEVEL, nextLv - 1);
#endif
		}
		else
		{
			//压缩纹理格式dds/s3tc/etc.....
			return false;
		}
		//计算内存使用大小
		m_uSize = memoryUse;
		//解除绑定
		m_pGraphicDriver->BindTexture(0, TextureUnit::TU_DIFFUSE);
		return true;
	}

	bool GLTexture2D::SetData(uint level, int x, int y, int width, int height, const void* pData)
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

		if (m_pGraphicDriver->IsDeviceLost())
		{
			SAPPHIRE_LOGWARNING("Texture data assignment while device is lost");
			m_bDataPending = true;
			return true;
		}

		if (IsCompressed())
		{
			x &= ~3;
			y &= ~3;
		}

		//绑定纹理对象，默认Diffuse
		m_pGraphicDriver->BindTexture(this, TU_DIFFUSE);
		if (!glIsTexture(m_uHwUID))
		{
			SAPPHIRE_LOGERROR("Error HwUID is invalid!");
			return false;
		}


		int format = GLGraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		//判断是否SRGB空间 
		format = GetSRGB() ? GetSRGBFormat(format) : format;
		int internalFormat = GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat);

		//设置一下opengl纹理格式属性
		m_glTexFormat = format;
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

		uint dataType = GLGraphicDriver::GetHWTextureDataType(m_ePixelFormat);
		if (!IsCompressed())
		{
			//是更新整个纹理数据区
			if (wholeLevel)
				glTexImage2D(m_glTexTarget, level, internalFormat, width, height, 0, format, dataType, pData);
			else
				//更新部分
				glTexSubImage2D(m_glTexTarget, level, x, y, width, height, internalFormat, dataType, pData);
		}
		else
		{
			if (wholeLevel)
				glCompressedTexImage2D(m_glTexTarget, level, internalFormat, width, height, 0, GetSize(), pData);
			else
				glCompressedTexSubImage2D(m_glTexTarget, level, x, y, width, height, internalFormat, dataType, pData);
		}
		m_pGraphicDriver->BindTexture(NULL, TextureUnit::TU_DIFFUSE);
		return true;
	}

	void GLTexture2D::OnLoadStart()
	{
		m_eState = ResourceState_Loading;
	}

	void GLTexture2D::OnLoadEnd()
	{
		m_eState = ResourceState_Loaded;
		ResourceCache* cache = dynamic_cast<ResourceCache*>(m_pCore->GetSubSystemWithType(ESST_RESOURCECACHE));
		if (cache)
		{
			cache->InsertResource(m_resName.c_str(), this);
		}
	}

	void GLTexture2D::OnLoadError()
	{
		m_eState = ResourceState_Unload;
	}

	void GLTexture2D::Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData /*= NULL*/)
	{
		if (eEventType == ET_GRAPHIC_EVENT)
		{
			switch (eEvent)
			{
			case EVENT_GRAPHICS_RENDERSURFACEUPDATE:
				RenderSurfaceUpdate();
				break;
			default:
				break;
			}
		}
		
	}

}