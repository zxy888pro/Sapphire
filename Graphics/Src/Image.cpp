#include "Image.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#include "mathHelper.h"


Sapphire::Image::~Image()
{

}

bool Sapphire::Image::Load(const std::string& imagePath, bool bCreateMipmap)
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
		m_imgType = ImageType::ImageType_Bmp_R8G8B8;
	}
	else if (ext == "tga")
	{
		m_imgType = ImageType::ImageType_Tga;
	}
	m_mipmaps.push_back(m_pbData);
	++m_nMipmapNum;
	GenerateMipmaps();
	return true;
}

void Sapphire::Image::Unload(void)
{
	m_imgType == ImageType::ImageType_Unknown;
	if (m_pbData)
	{
		stbi_image_free(m_pbData);
	}
	//ж����ͼ����Դ
	for (int i = 0; i < m_subImgs.size(); ++i)
	{
		if (m_subImgs[i] != NULL)
		{
			m_subImgs[i]->Unload();
			safeDelete(m_subImgs[i]);
		}
	}
	m_subImgs.clear();
	//for (int i = 1; i < m_mipmaps.size(); ++i)
	//{

	//	safeDeleteArray(m_mipmaps[i]);
	//	//m_mipmaps[i]->Unload();
	//}
	m_mipmaps.clear();
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
	if (nChannels == 4)
		m_imgType = ImageType::ImageType_RAW_R8G8B8A8;
	else
	    m_imgType = ImageType::ImageType_RAW_R8G8B8;
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

	//int nMipmap = 0;
	

	while (subHeight > 1 && subWidth>1)
	{
		//++nMipmap;
		int parentWidth = subWidth;
		int parentHeight = subHeight;
		subWidth /= 2;
		subHeight /= 2;

		Image* imgPtr = new Image();
		int totalSize = subHeight*subWidth*m_nChannels;
		byte* pbDate = new byte[totalSize];
		int ret = stbir_resize_uint8(m_pbData, parentWidth, parentHeight, 0, pbDate, subWidth, subHeight, 0, m_nChannels); //���¸ı��С
		imgPtr->SetRAWData(pbDate, subWidth, subHeight, m_nChannels);
		m_mipmaps.push_back(pbDate);
		m_subImgs.push_back(imgPtr);
		++m_nMipmapNum;
	}

}
