#include "Image.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#include "mathHelper.h"


Sapphire::Image::~Image()
{

}

bool Sapphire::Image::Load(const std::string& imagePath)
{
	Path fpath = imagePath;
	if (!FileIsExistA(fpath.c_str()))
	{
		LogUtil::LogMsgLn(StringFormatA("����ʧ�ܣ� %s �����ڣ�", fpath.c_str()));
		return false;
	}
	m_pbData = stbi_load(fpath.c_str(), &m_nWidth, &m_nHeight, &m_nChannels, 0);
	if (m_pbData == NULL)
	{
		LogUtil::LogMsgLn(StringFormatA("����ʧ�ܣ� %s �����ڣ�", fpath.c_str()));
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
	m_mipmaps.push_back(this);
	++m_nMipmapNum;
	return true;
}

void Sapphire::Image::Unload(void)
{
	m_imgType == ImageType::ImageType_Unknown;
	if (m_pbData)
	{
		stbi_image_free(m_pbData);
	}
	for (int i = 1; i < m_mipmaps.size(); ++i)
	{
		m_mipmaps[i]->Unload();
	}
	LogUtil::LogMsgLn(StringFormatA(" ж��ͼƬ :%s ", m_Name.c_str()));
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
	m_pbData = (PRAWIMAGE)dataPtr;
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

	//��������ͼ
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
		++m_nMipmapNum;
	}

}
