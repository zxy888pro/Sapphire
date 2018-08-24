#include <Graphics.h>
#include "TextureMgr.h"
#include "Texture2D.h"
#include <Core.h>
#include <GraphicException.h>
#include <stringHelper.h>
#include <Image.h>
#include <BaseResource.h>
#include <logUtil.h>


namespace Sapphire
{

	TextureMgr::TextureMgr()
	{
		m_nCurActiveTexUnit = 0;
		for (int i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_textures[i] = NULL;
			m_textureTypes[i] = NULL;
		}
			


	}

	TextureMgr::~TextureMgr()
	{

	}

	Sapphire::ITexture* TextureMgr::CreateTexture2DFromFile(std::string filePath, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_WRAP*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_WRAP*/, bool bDynamic /*= false*/)
	{
		//ImageMgr* pImageMgr = ImageMgr::GetSingletonPtr();
		//Core* pCore = Core::GetSingletonPtr();
		//if (pImageMgr == NULL || pCore == NULL)
		//{
		//	throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
		//}
		//HIMAGE himg = pImageMgr->GetImage(filePath.c_str());
		//if (!himg.IsNull())
		//{
		//	LogUtil::LogMsgLn(StringFormatA("Create Texture Failed! Not found %s", filePath.c_str()));
		//	return NULL;
		//}
		//PRAWIMAGE pImgData = pImageMgr->GetTexture(himg);
		//uint width = pImageMgr->GetWidth(himg);
		//uint height = pImageMgr->GetHeight(himg);
		////ͨ�����൱��ռ���ֽ���
		//uint nChannels = pImageMgr->GetNumChannels(himg);
		//if (pImgData == NULL)
		//{
		//	LogUtil::LogMsgLn("Create Texture Failed! RawData is Null");
		//}
		//Texture2D*  pTexture = new Texture2D(width,height,nChannels);
		//pTexture->GPUObjectInit(pImgData);
		//pTexture->SetSize(pImageMgr->GetImageSize(himg));
		//return pTexture;
		return NULL;

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
		return NULL;
	}

	Sapphire::ITexture* TextureMgr::RequestTexture(RHANDLE handle)
	{
		return NULL;
	}

	void TextureMgr::DiscardTexture(ITexture* pTexture)
	{
		 
	}

	void TextureMgr::SetTexture(ITexture* pTexture, TextureUnit index)
	{
		if (index > TextureUnit::MAX_TEXTURE_UNITS)
		{
			throw GraphicDriverException("Index Exceeding the maximum limit ", GraphicDriverException::GDError_TextureOperationError);
		}
		if (m_textures[index] != pTexture)
		{
			//�����ǰ���������Ԫ�������
			if (m_nCurActiveTexUnit != index)
			{
				//��ô�����������Ԫ
				glActiveTexture(GL_TEXTURE0 + index);
				m_nCurActiveTexUnit = index;
			}
			//������
			if (pTexture)
			{
				uint glType = pTexture->getTextureType();
				//���������Ԫ���������ͺ͸�����һ�£���ô���°󶨵�������
				if (m_textureTypes[index] && m_textureTypes[index] != glType)
				{
					glBindTexture(m_textureTypes[index], 0);
					glBindTexture(glType, pTexture->getUID());
					m_textureTypes[index] = glType;
				}
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
				//�����ǰ���������Ԫ�������
				if (m_nCurActiveTexUnit != index)
				{
					//��ô�����������Ԫ
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

}