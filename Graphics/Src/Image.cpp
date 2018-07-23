#include "Image.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "path.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#include "mathHelper.h"

Sapphire::ImageMgr::ImageMgr()
{

}

Sapphire::ImageMgr::~ImageMgr()
{
	ImageMap::iterator i, begin = m_imageMap.begin(), end = m_imageMap.end();
	for (i = begin; i != end; ++i)
	{
		m_Images.Dereference(i->second)->Unload();
	}
}

Sapphire::HIMAGE Sapphire::ImageMgr::LoadImage(const char* name)
{
	ImageMapInsertRc rc = m_imageMap.insert(std::make_pair(name, HIMAGE()));
	if (rc.second)
	{
		
		Image* pImg = m_Images.Acquire(rc.first->second);
		if (!pImg->Load(rc.first->first))
		{
			DeleteTexture(rc.first->second);
			rc.first->second = HIMAGE();
		}
	}
	return (rc.first->second);
}

void Sapphire::ImageMgr::DeleteTexture(HIMAGE himg)
{
	Image* pImg = m_Images.Dereference(himg);
	if (pImg != 0)
	{
		// 通过索引从Map中删除
		m_imageMap.erase(m_imageMap.find(pImg->getName()));

		// 从数据库中删除
		pImg->Unload();
		m_Images.Release(himg);
	}
}

Sapphire::Image::~Image()
{

}

bool Sapphire::Image::Load(const std::string& imagePath)
{
	Path fpath = imagePath;
	if (!FileIsExistA(fpath.c_str()))
	{
		LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
		return false;
	}
	m_pbData = stbi_load(fpath.c_str(), &m_nWidth, &m_nHeight, &m_nChannels, 0);
	if (m_pbData == NULL)
	{
		LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
		return false;
	}
	m_ullDataSize =  GetFileSizeA(fpath.c_str());
	m_Name = fpath.getFileName();
	std::string ext = fpath.getFileExt();
	if (ext == "jpg")
	{
		m_imgType = ImageType::ImageType_Jpeg;
	}
	else if (ext == "png")
	{
		m_imgType = ImageType::ImageType_Png;
	}
	else if (ext == "bmp")
	{
		m_imgType = ImageType::ImageType_Bmp;
	}
	else if (ext == "tga")
	{
		m_imgType = ImageType::ImageType_Tga;
	}

	return true;
}

void Sapphire::Image::Unload(void)
{
	m_imgType == ImageType::ImageType_Unknown;
	if (m_pbData)
	{
		stbi_image_free(m_pbData);
	}
	for (int i = 0; i < m_mipmaps.size(); ++i)
	{
		m_mipmaps[i]->Unload();
	}
	LogUtil::LogMsgLn(StringFormatA(" 卸载图片 :%s ", m_Name.c_str()));
	m_nWidth = 0;
	m_nHeight = 0;
	m_Name = "";
	m_ullDataSize = 0;
	m_nChannels = 0;
	m_pbData = NULL;
	

}

void Sapphire::Image::SetRAWData(void* dataPtr, int width, int height, int nChannels)
{
	if (dataPtr == NULL)
	{
		LogUtil::LogMsgLn("SetRAWData Error! dataPtr is NULL");
	}
	m_pbData = (byte*)dataPtr;
	m_nWidth = width;
	m_nHeight = height;
	m_imgType = ImageType::ImageType_RAW;
	m_nChannels = nChannels;
	m_mipmaps.clear();
	m_nMipmapNum = 0;
	m_Name = "";
	m_ullDataSize = 0;
}

void Sapphire::Image::GenerateMipmaps()
{

	//创建缩放图
	int subWidth = m_nWidth;
	int subHeight = m_nHeight;

	int nMipmap = 0;
	

	while (subHeight > 1 && subWidth>1)
	{
		++nMipmap;
		int parentWidth = subWidth;
		int parentHeight = subHeight;
		subWidth /= 2;
		subHeight /= 2;

		Image* imgPtr = new Image();
		int totalSize = subHeight*subWidth*m_nChannels;
		byte* pbDate = new byte[totalSize];
		int ret = stbir_resize_uint8(m_pbData, parentWidth, parentHeight, 0, pbDate, subWidth, subHeight, 0, m_nChannels);
		imgPtr->SetRAWData(pbDate, subWidth, subHeight, m_nChannels);
		m_mipmaps.push_back(imgPtr);
	}
	m_nMipmapNum = nMipmap;

}
