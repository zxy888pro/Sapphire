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
#include <FileStream.h>
#include "CubeTexture.h"


namespace Sapphire
{

	TextureMgr::TextureMgr()
	{
		//��������ڴ�����
		SetMaximumMemory(1024 * 1024 * 512);
		//��Ԥ�����ڴ�
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
		RHANDLE handle = 0;
		InsertResource(&handle, pTexture);
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

	bool TextureMgr::VerifyHWUID(uint uHwuid)
	{
		GLboolean bRet = glIsTexture(uHwuid);
		return bRet;
	}

	Sapphire::ITexture* TextureMgr::CreateCubeTextureFromImage(HIMAGE himg, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/)
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	Sapphire::ITexture* TextureMgr::CreateCubeTextureFromFile(std::string filePath, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/, bool bDynamic /*= false*/)
	{
		FileStream fs(filePath.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ);
		if (fs.IsOpen())
		{
			uint width = 0;
			uint height = 0;
			uint channel = 0;
			std::string widthStr;
			if (fs.ReadLine(widthStr))
			{
				width = ToInt(widthStr);
			}
			std::string heightStr;
			if (fs.ReadLine(heightStr))
			{
				height = ToInt(heightStr);
			}
			uint nChannel = 0;
			std::string channelStr;
			if (fs.ReadLine(channelStr))
			{
				channel = ToInt(channelStr);
			}
			std::vector<std::string> imgFiles;
			std::string imgFile;
			for (int i = 0; i < 6; i++)
			{
				
				if (fs.ReadLine(imgFile))
				{
					imgFiles.push_back(imgFile);
				}
				else
				{
					break;
				}

			}

			fs.Release();
			if (imgFiles.size() == 6)
			{
				IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
				Core* pCore = Core::GetSingletonPtr();
				if (pImageMgr == NULL || pCore == NULL)
				{
					throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
				}
				
				HIMAGE  imgs[6];
				for (int i = 0; i < imgFiles.size(); ++i)
				{

					imgs[i] = pImageMgr->GetImage(filePath.c_str());
					if (imgs[i].IsNull())
					{
						LogUtil::LogMsgLn(StringFormatA("Load ImageFile Failed! Not found %s", filePath.c_str()));
						return NULL;
					}
					
				}
				CubeTexture* pTexture = new CubeTexture(width,channel);
				pTexture->Load(imgs[FACE_NEGATIVE_X], imgs[FACE_POSITIVE_Y], imgs[FACE_POSITIVE_X], imgs[FACE_NEGATIVE_Y], imgs[FACE_POSITIVE_Z], imgs[FACE_NEGATIVE_Z]);
				pTexture->setName(filePath);
				RHANDLE handle = 0;
				InsertResource(&handle, pTexture);  //����TextureManager����
				return pTexture;
			}
		}
		return NULL;
	}

}