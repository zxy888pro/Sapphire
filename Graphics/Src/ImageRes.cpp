#include "ImageRes.h"
#include "ImageHelper.h"
#include "mathHelper.h"
#include "ResourceCache.h"
#include "ResourceLoader.h"


namespace Sapphire
{

	bool Sapphire::ImageRes::Load(bool bCreateMipmap)
	{
		Path fpath = m_resName;
		if (!FileIsExistA(fpath.c_str()))
		{
			LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
			return false;
		}
		m_pbData = LoadImageFile(fpath.c_str(), &m_nWidth, &m_nHeight, &m_nChannels);
		if (m_pbData == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("加载失败！ %s 不存在！", fpath.c_str()));
			return false;
		}
		m_ullDataSize = GetFileSizeA(fpath.c_str());
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
			if (m_nChannels == 3)
			{
				m_imgType = ImageType::ImageType_Tga24;
			}
			else
			{
				m_imgType = ImageType::ImageType_Tga32;
			}

		}
		m_mipmaps.push_back(m_pbData);
		++m_nMipmapNum;
		GenerateMipmaps();
		return true;
	}


	void Sapphire::ImageRes::SetRAWData(void* dataPtr, int width, int height, int nChannels)
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

	void Sapphire::ImageRes::GenerateMipmaps()
	{

		//创建缩放图
		int subWidth = m_nWidth;
		int subHeight = m_nHeight;

		//int nMipmap = 0;


		while (subHeight > 1 && subWidth > 1)
		{
			//++nMipmap;
			int parentWidth = subWidth;
			int parentHeight = subHeight;
			subWidth /= 2;
			subHeight /= 2;

			ImageRes* imgPtr = new ImageRes(m_pCore);
			int totalSize = subHeight*subWidth*m_nChannels;
			byte* pbDate = new byte[totalSize];
			int ret = ResizeImage(m_pbData, parentWidth, parentHeight, 0, pbDate, subWidth, subHeight, 0, m_nChannels); //重新改变大小
			imgPtr->SetRAWData(pbDate, subWidth, subHeight, m_nChannels);
			m_mipmaps.push_back(pbDate);
			m_subImgs.push_back(imgPtr);
			++m_nMipmapNum;
		}

	}

	void ImageRes::Clear()
	{
		Dispose();
	}

	bool ImageRes::Create()
	{
		if (IsDisposed())
		{
			Load(m_bCreateMipmap);
			return true;
		}
		return false;
	}

	void ImageRes::Destroy()
	{
		Dispose();
	}

	ImageRes::~ImageRes()
	{
		if (!IsDisposed())
		{
			Dispose();
		}
	}

	bool ImageRes::Load()
	{
		return Load(m_bCreateMipmap);
	}

	bool ImageRes::Load(const char* resPath)
	{
		m_resName = resPath;
		return Load(m_bCreateMipmap);
	}

	bool ImageRes::Recreate()
	{
		if (!IsDisposed())
		{
			Dispose();
		}
		ResourceLoader*  resourceLoader = dynamic_cast<ResourceLoader*>(m_pCore->GetSubSystemWithType(ESST_RESOURCELOADER));
		if (resourceLoader)
		{
			resourceLoader->LoadResource(this);
		}
		//return Load(m_bCreateMipmap);
		return true;
	}

	void ImageRes::Dispose()
	{
		m_imgType == ImageType::ImageType_Unknown;
		if (m_pbData)
		{
			FreeImage(m_pbData);
		}
		//卸载子图像资源
		for (int i = 0; i < m_subImgs.size(); ++i)
		{
			if (m_subImgs[i] != NULL)
			{
				m_subImgs[i]->Dispose();
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
		LogUtil::LogMsgLn(StringFormatA(" 卸载图片 :%s ", m_Name.c_str()));
		m_nWidth = 0;
		m_nHeight = 0;
		m_Name = "";
		m_ullDataSize = 0;
		m_nChannels = 0;
		m_pbData = NULL;
	}

	void ImageRes::OnLoadStart()
	{
		/*m_eState = ResourceState_Loading;
		ResourceCache* cache = dynamic_cast<ResourceCache*>(m_pCore->GetSubSystemWithType(ESST_RESOURCECACHE));
		if (cache)
		{
			cache->InsertResource(m_resName.c_str(), this);
		}*/
		BaseResource::OnLoadStart();
	}

	void ImageRes::OnLoadEnd()
	{
		//m_eState = ResourceState_Loaded;
		BaseResource::OnLoadEnd();
	}

	void ImageRes::OnLoadError()
	{
		//m_eState = ResourceState_Unload;
	}

	size_t ImageRes::GetSize()
	{
		return getDataSize();
	}

	bool ImageRes::IsDisposed()
	{
		return m_pbData == NULL;
	}

}