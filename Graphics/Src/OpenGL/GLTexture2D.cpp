#include <mathHelper.h>

#include "GLGraphicDriver.h"
#include "GraphicException.h"
#include "GLTexture2D.h"
#include "GLRenderSurface.h"


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
		
	}

	bool GLTexture2D::Recreate()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::Dispose()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLTexture2D::IsDisposed()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	size_t GLTexture2D::GetSize()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::ReRequest()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::OnDeviceLost()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::OnDeviceReset()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLTexture2D::Create()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::RenderSurfaceUpdate()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLTexture2D::Load(const char* resName)
	{
		m_resName = resName;
		return Load();
	}

	bool GLTexture2D::Load()
	{
		if (m_pGraphicDriver == NULL)
			return;

		if (m_pGraphicDriver->IsDeviceLost())
		{
			return;
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

	bool GLTexture2D::SetData(SharedPtr<ImageRes> img, bool useAlpha)
	{
		PRAWIMAGE pImgData = img->getData();
		if (pImgData == NULL)
		{
			SAPPHIRE_LOGERROR("Create Texture Failed! RawData is Null");
			return;
		}
		//创建纹理对象
		//Create();
		bool useAlpha = true;
		//默認alpha==false，只有一個通道的話默認為明度
		//获取当前纹理质量
		int quality = m_pGraphicDriver->getTextureQuality();
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		if (!pImageMgr)
		{
			SAPPHIRE_LOGERROR("ImageMgr is not initialized!");
			return false;
		}
		unsigned memoryUse = sizeof(GLTexture2D);
		//先绑定纹理对象
		m_pGraphicDriver->BindTexture(this, TextureUnit::TU_DIFFUSE);

		if (!img->IsCompressd())
		{
			int channels = img->getChannels();  //获取通道数
			//mip等级0，最大分辨率
			PRAWIMAGE levelData = img->getData();
			int levelWidth = img->getWidth;
			int levelHeight = img->getHeight();

			PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(img->getImageType);
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
			switch (channels)
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
			m_glTexFormat = format;
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
		//接触绑定
		m_pGraphicDriver->BindTexture(0, TextureUnit::TU_DIFFUSE);
		return true;
	}

	bool GLTexture2D::SetData(uint level, int x, int y, int width, int height, const void* data)
	{

	}

	void GLTexture2D::OnLoadStart()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::OnLoadEnd()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	void GLTexture2D::OnLoadError()
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

}