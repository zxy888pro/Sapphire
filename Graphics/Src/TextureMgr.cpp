#include <Graphics.h>
#include <GraphicDriver.h>
#include "TextureMgr.h"
#include "Texture2D.h"
#include <Core.h>
#include <GraphicException.h>
#include <stringHelper.h>
#include <Image.h>
#include "ImageMgr.h"
#include <BaseResource.h>
#include <logUtil.h>


namespace Sapphire
{

	TextureMgr::TextureMgr()
	{
		//设置最大内存上限
		SetMaximumMemory(1024 * 1024 * 512);
		//先预分配内存
		ReserveMemory(1024 * 1024 * 64);
		
		m_nCurActiveTexUnit = 0;
		for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_textures[i] = NULL;
			m_textureTypes[i] = NULL;
		}
		m_pGraphicDriver = GraphicDriver::GetSingletonPtr();


	}

	TextureMgr::~TextureMgr()
	{
		Destroy();
	}

	Sapphire::ITexture* TextureMgr::CreateTexture2DFromFile(std::string filePath, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_WRAP*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_WRAP*/, bool bDynamic /*= false*/)
	{
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		Core* pCore = Core::GetSingletonPtr();
		if (pImageMgr == NULL || pCore == NULL)
		{
			throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
		}
		HIMAGE himg = pImageMgr->GetImage(filePath.c_str());
		if (himg.IsNull())
		{
			LogUtil::LogMsgLn(StringFormatA("Create Texture Failed! Not found %s", filePath.c_str()));
			return NULL;
		}
		Texture2D* pTexture = new Texture2D();
		pTexture->Load(himg);
		pTexture->setName(filePath);
		RHANDLE handle = 0;
		InsertResource(&handle, pTexture);
		return pTexture;

	}

	Sapphire::ITexture* TextureMgr::CreateTexture2DFromImage(HIMAGE himg, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/)
	{
		return NULL;
	}

	Sapphire::ITexture* TextureMgr::CreateTexture2DFromRAW(byte* pData, uint width, uint height, PixelFormat pFormat /*= PF_R8G8B8A8*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/)
	{
		return NULL;
	}

	Sapphire::ITexture* TextureMgr::CreateEmptyTexture2D(uint width, uint height, PixelFormat pFormat /*= PF_R8G8B8A8*/, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/)
	{
		Texture2D* pTexture = new Texture2D(width, height, pFormat);
		pTexture->Create();
		pTexture->SetData(pTexture->getMipLevel(),0,0, width, height, NULL);
		return pTexture;
	}

	Sapphire::ITexture* TextureMgr::RequestTexture(RHANDLE handle)
	{
		Texture2D* pTexture = (Texture2D*) GetResource(handle);

		return pTexture;
	}

	void TextureMgr::DiscardTexture(ITexture* pTexture)
	{
		DestroyResource((BaseResource*)pTexture);
	}

	void TextureMgr::SetTexture(ITexture* pTexture, TextureUnit index)
	{
		if (index > TextureUnit::MAX_TEXTURE_UNITS)
		{
			throw GraphicDriverException("Index Exceeding the maximum limit ", GraphicDriverException::GDError_TextureOperationError);
		}
		if (m_textures[index] != pTexture)
		{
			//如果当前激活的纹理单元不是这个
			if (m_nCurActiveTexUnit != index)
			{
				//那么激活这个纹理单元
				glActiveTexture(GL_TEXTURE0 + index);
				m_nCurActiveTexUnit = index;
			}
			//绑定纹理
			if (pTexture)
			{
				uint glType = pTexture->getTextureType();
				//如果该纹理单元的纹理类型和该纹理不一致，那么重新绑定到该纹理
				if (m_textureTypes[index] && m_textureTypes[index] != glType)
				{
					glBindTexture(m_textureTypes[index], 0);
				}
				glBindTexture(glType, pTexture->getUID());
				m_textureTypes[index] = glType;
			}
			else if (m_textureTypes[index])
			{
				glBindTexture(m_textureTypes[index], 0);
				m_textureTypes[index] = 0;
			}

			m_textures[index] = pTexture;
		}
		else
		{
			if (pTexture)
			{
				//如果当前激活的纹理单元不是这个
				if (m_nCurActiveTexUnit != index)
				{
					//那么激活这个纹理单元
					glActiveTexture(GL_TEXTURE0 + index);
					m_nCurActiveTexUnit = index;
				}
				glBindTexture(m_textureTypes[index], 0);

			}
		}

		
	}

	Sapphire::ITexture* TextureMgr::GetTexture(TextureUnit index)
	{
		if (index > TextureUnit::MAX_TEXTURE_UNITS)
		{
			throw GraphicDriverException("Index Exceeding the maximum limit ", GraphicDriverException::GDError_TextureOperationError);
		}
		return m_textures[index];

	}

	bool TextureMgr::VerifyHWUID(uint uHwuid)
	{
		GLboolean bRet = glIsTexture(uHwuid);
		return bRet;
	}

}