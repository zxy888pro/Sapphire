#include <Graphics.h>
#include "GLGraphicDriver.h"
#include "TextureMgr.h"
#include "Texture2D.h"
#include <Core.h>
#include <GraphicException.h>
#include <stringHelper.h>
#include <Image.h>
#include "../ImageMgr.h"
#include <BaseResource.h>
#include <logUtil.h>
#include <FileStream.h>
#include "CubeTexture.h"
#include "json/json.h"


namespace Sapphire
{

	TextureMgr::TextureMgr(Core* pCore) :ResourceContainer(pCore)
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
		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(m_pCore->GetSubSystemWithType(ESST_GRAPHICDRIVER));


	}

	TextureMgr::~TextureMgr()
	{
		Destroy();
	}

	Sapphire::ITexture* TextureMgr::CreateTexture2DFromFile(std::string filePath, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_WRAP*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_WRAP*/, bool bDynamic /*= false*/)
	{
		IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
		HIMAGE himg = pImageMgr->GetImage(filePath.c_str());
		if (himg.IsNull())
		{
			LogUtil::LogMsgLn(StringFormatA("Create Texture Failed! Not found %s", filePath.c_str()));
			return NULL;
		}
		Texture2D* pTexture = new Texture2D(m_pCore, m_pGraphicDriver, filePath.c_str());
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
		Texture2D* pTexture = new Texture2D(m_pCore , m_pGraphicDriver, "", width, height, pFormat);
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
				//获取OpenGL纹理类型
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

	Sapphire::ITexture* TextureMgr::CreateCubeTextureFromImage(HIMAGE himg, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/)
	{
		//通过切图来加载,还没想好怎么定义
		throw std::logic_error("The method or operation is not implemented.");
	}

	Sapphire::ITexture* TextureMgr::CreateCubeTextureFromFile(std::string filePath, TextureFilterMode filterMode /*= TextureFilterMode::FILTER_BILINEAR*/, TextureAddressMode s /*= TextureAddressMode::ADDRESS_REPEAT*/, TextureAddressMode t /*= TextureAddressMode::ADDRESS_REPEAT*/, bool bDynamic /*= false*/)
	{
		FileStream fs(filePath.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			//暂时写在这里加载
			IImageMgr* pImageMgr = m_pGraphicDriver->getImageMgr();
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
				int channel = rootNode["channel"].asInt();
				if (channel < 3 || channel > 4)
				{
					throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
				}
				//取得图像格式
				std::string imageType = rootNode["imageType"].asString();
				//取得图像格式对应的纹理格式
				PixelFormat ePixelFormat = m_pGraphicDriver->GetPixelFormat(imageType);
				if (ePixelFormat == PF_UNDEFINED)
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
				CubeTexture* pTexture = new CubeTexture(m_pCore,m_pGraphicDriver, filePath.c_str(),width, channel);
				pTexture->Load(imgs[FACE_NEGATIVE_X], imgs[FACE_POSITIVE_Y], imgs[FACE_POSITIVE_X], imgs[FACE_NEGATIVE_Y], imgs[FACE_POSITIVE_Z], imgs[FACE_NEGATIVE_Z]);
				pTexture->setName(filePath);
				pTexture->setPixelFormat(ePixelFormat);
				pTexture->setChannelNum(channel);
				RHANDLE handle = 0;
				InsertResource(&handle, pTexture);  //加入TextureManager管理
				return pTexture;
			}
			else
			{
				throw GraphicDriverException("Parse Cubmap File Failed!", GraphicDriverException::GDError_TextureFileParseError);
			}
		}
		return NULL;
	}

	void TextureMgr::SetTextureForUpdate(ITexture* texture)
	{
		if (m_nCurActiveTexUnit != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			m_nCurActiveTexUnit = 0;
		}
		//获取OpenGL纹理类型
		unsigned glType = texture->getTextureType();
		// 如果有必要取消旧的绑定纹理
		if (m_textureTypes[0] && m_textureTypes[0] != glType)
			glBindTexture(m_textureTypes[0], 0);
		glBindTexture(glType, texture->getUID());
		m_textureTypes[0] = glType;
		m_textures[0] = texture;
	}

}