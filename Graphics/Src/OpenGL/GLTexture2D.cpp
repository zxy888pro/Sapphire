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
			//�ͷ�ռ���������
			if (!m_pGraphicDriver->IsDeviceLost())
			{
				for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
				{
					if (m_pGraphicDriver->getTextureMgr()->GetTexture((TextureUnit)i) == this)
						m_pGraphicDriver->getTextureMgr()->SetTexture(0, (TextureUnit)i);
				}
				glDeleteTextures(1, &m_uHwUID); //ɾ���������
			}
			//��ʼ��Ӳ����ԴID
			m_uHwUID = 0;
		}
		//�ͷ�renderSurface
		if (m_renderSurface)
			m_renderSurface->Release();
	}

	bool GLTexture2D::Recreate()
	{
		Dispose();
		if (m_szName != "")
		{
			ReRequest();  //������������
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
			m_bDataLost = true; //�����Ѷ�ʧ
		}
		Recreate();  //���´���
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
		// �����֧����������������һ��һ����������ģ������Ļ�������һ��renderBuffer���ԭ��texture
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

		//������������Ԫ0�ö�����и���
		pGLDriver->getTextureMgr()->SetTextureForUpdate(this);
		int format = GLGraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		//�Ƿ�ʹ��SRGB�ռ�
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

		//����mipmap
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
 
		//�����������
		UpdateParameters();
		//�������������ȡ����
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
		//���ͷ�֮ǰ���������
		Dispose();
		Path xmlFile = m_resName;
		std::string fileName = xmlFile.getFileName();
		xmlFile = xmlFile.getParentDir();
		xmlFile.addTailSlash();
		xmlFile += fileName + ".xml";
		bool ret = false;

		m_parameters = SharedPtr<XMLFile>(new XMLFile(m_pCore, xmlFile.c_str()));
		ret = m_parameters->Load();
		SetParameters(m_parameters); //�����������ò���
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
		m_renderSurface.Reset(); //�ͷž���Դ
	 
		m_eUsage = eUsage;
		m_uWidth = width;
		m_uHeight = height;
		m_ePixelFormat = eformat;
		

		if (eUsage >= TEXTURE_RENDERTARGET)
		{
			// �ָ�Ĭ������
			m_renderSurface = new GLRenderSurface(m_pCore,this); //���´�����Ӧ��Ⱦ����
			m_eAddressMode_[COORD_U] = ADDRESS_CLAMP;
			m_eAddressMode_[COORD_V] = ADDRESS_CLAMP;
			m_eFilterMode = FILTER_NEAREST;
			m_requestLevel = 1;
		}
		//����һ���¼���Ⱦ�������, �и���ʱ����RenderSurfaceUpdate
		if (m_eUsage == TEXTURE_RENDERTARGET)
			SubscribeEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_RENDERSURFACEUPDATE);
		else
			UnSubscribeEvent(ET_GRAPHIC_EVENT,EVENT_GRAPHICS_RENDERSURFACEUPDATE);
		//��С�����ظ�ʽ�����ı������´���������
		Dispose();
		Create();
		//SetData(m_mipLevel, 0, 0, width, height, NULL); //������
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
		//�����������
		//Create();
		//Ĭ�Jalpha==false��ֻ��һ��ͨ����ԒĬ�J������
		//��ȡ��ǰ��������
		int quality = m_pGraphicDriver->getTextureQuality();
		/*IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return false;
		}*/
		//ͳ���ڴ�ռ��
		unsigned memoryUse = sizeof(GLTexture2D); 
		//�Ȱ��������
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE);

		if (!img->IsCompressd())
		{
			int channels = img->getChannels();  //��ȡͨ����
			//mip�ȼ�0�����ֱ���
			PRAWIMAGE levelData = img->getData();
			int levelWidth = img->getWidth();
			int levelHeight = img->getHeight();

			PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(img->getImageType());
			int format = 0;
			int nextLv = 1;
			//���ݻ������ã�����ָ��mip�ȼ�, ����Խ�ߣ�������mipmapԽ��
			//��������Ҫ��mip�ȼ�
			for (unsigned i = 0; i < m_skipMips[quality]; ++i)
			{
				levelData = img->getMipmap(i);
				levelWidth = img->getWidth(i);
				levelHeight = img->getHeight(i);
				++nextLv;
			}
			//ͨ��ͨ��ת��λ��Ӧ�ĸ�ʽ
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
			//�����������ã���ı�����0�Ĵ�С
			SetSize(levelWidth, levelHeight, m_ePixelFormat);
			for (unsigned i = 0; i < m_uNumMipmaps; ++i)
			{
				//mip0�����^��mipmap lv�_ʼ,�����ڴ�ռ��
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
			//�O��mipmap
#ifndef GL_ES_VERSION_2_0
			glTexParameteri(m_glTexTarget, GL_TEXTURE_BASE_LEVEL, 0);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAX_LEVEL, nextLv - 1);
#endif
		}
		else
		{
			//ѹ�������ʽdds/s3tc/etc.....
			return false;
		}
		//�����ڴ�ʹ�ô�С
		m_uSize = memoryUse;
		//�����
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

		//���������Ĭ��Diffuse
		m_pGraphicDriver->BindTexture(this, TU_DIFFUSE);
		if (!glIsTexture(m_uHwUID))
		{
			SAPPHIRE_LOGERROR("Error HwUID is invalid!");
			return false;
		}


		int format = GLGraphicDriver::GetSWTextureFormat(m_ePixelFormat);
		//�ж��Ƿ�SRGB�ռ� 
		format = GetSRGB() ? GetSRGBFormat(format) : format;
		int internalFormat = GLGraphicDriver::GetHWTextureFormat(m_ePixelFormat);

		//����һ��opengl�����ʽ����
		m_glTexFormat = format;
		//��ȡlevel����mipmap���
		int levelWidth = getLevelWidth(level);
		int levelHeight = getLevelHeight(level);

		//����Ƿ�Խ��
		if (x < 0 || x + width > levelWidth || y < 0 || y + height > levelHeight || width <= 0 || height <= 0)
		{
			SAPPHIRE_LOGERROR("Illegal dimensions for setting data");
			return false;
		}


		//�Ƿ�ȫ��Χ
		bool wholeLevel = x == 0 && y == 0 && width == levelWidth && height == levelHeight;

		uint dataType = GLGraphicDriver::GetHWTextureDataType(m_ePixelFormat);
		if (!IsCompressed())
		{
			//�Ǹ�����������������
			if (wholeLevel)
				glTexImage2D(m_glTexTarget, level, internalFormat, width, height, 0, format, dataType, pData);
			else
				//���²���
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