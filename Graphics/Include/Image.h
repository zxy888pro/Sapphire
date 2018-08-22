#ifndef __IMAGE__H__
#define __IMAGE__H__

#include "Graphics.h"
#include "singleton.h"
#include "handle.h"
#include <cassert>
#include <map>

namespace Sapphire
{
	enum ImageType
	{
		ImageType_Bmp,
		ImageType_Jpeg,
		ImageType_Png,
		ImageType_Tga,
		ImageType_RAW,
		ImageType_Unknown
	};

	

	class Image
	{
	public:

		Image()
		{
			m_imgType = ImageType::ImageType_Unknown;
			m_ullDataSize = 0;
			m_Name = "";
			m_pbData = NULL;
			m_nHeight = 0;
			m_nWidth = 0;
			m_nMipmapNum = 0;
			m_nChannels = 0;
		};

		friend class ImageMgr;

		typedef std::vector <Image*> MipmapArray;

		virtual ~Image();

		const std::string& getName() const { return m_Name; }

	    void setName(std::string val) { m_Name = val; }

		int getHeight() const { return m_nHeight; }

		int getWidth() const { return m_nWidth; }

		//RAWDATA 统一为4通道RGBA  3通道RGB  2通道GA  1通道G
		virtual PRAWIMAGE getData() const
		{
			return m_pbData;
		}

		virtual const Image* getMipmap(uint nMipmap = 0) const
		{
			assert(nMipmap < m_nMipmapNum);
			return m_mipmaps[nMipmap];
		}

		virtual  uint getRowSize() const { return m_uRowSize; }

		virtual ulonglong getDataSize() const { return m_ullDataSize; }

		virtual bool Load(const std::string& imagePath);

		virtual void Unload(void);

		virtual int getChannels() const { return m_nChannels; } 

		virtual void SetRAWData(void* dataPtr, int width, int height, int nChannels);
		

	protected:

		std::string  m_Name;       
		int m_nWidth;
		int m_nHeight;
		int m_nChannels;
		ulonglong    m_ullDataSize;
		uint		 m_uRowSize;
		PRAWIMAGE	     m_pbData;
		ImageType    m_imgType;
		int          m_nMipmapNum;
		MipmapArray  m_mipmaps;

		void GenerateMipmaps();
 
		
	};

	//定义Image句柄
	typedef Handle <Image> HIMAGE;


	class ImageMgr : public Singleton<ImageMgr>
	{
	public:


		ImageMgr();
		virtual ~ImageMgr();

		typedef HandleMgr <Image, HIMAGE> HImageMgr;	

		HIMAGE   LoadImage(const char* name);

		HIMAGE   GetImage(const char* name);

		void     DeleteTexture(HIMAGE himg);

		const std::string& GetName(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getName());
		}
		uint GetWidth(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getWidth());
		}
		uint GetHeight(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getHeight());
		}
		uint GetNumChannels(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getChannels());
		}
		ulonglong GetImageSize(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getDataSize());
		}

		PRAWIMAGE  GetTexture(HIMAGE himg, uint mip = 0) const
		{
			return m_Images.Dereference(himg)->getMipmap(mip)->getData();
		}
		 

	private:

		struct istring_less
		{
			bool operator () (const std::string& l, const std::string& r) const
			{
				return (std::strcmp(l.c_str(), r.c_str()) < 0);
			}
		};

		typedef std::map <std::string, HIMAGE, istring_less > ImageMap;
		typedef std::pair <ImageMap::iterator, bool> ImageMapInsertRc;

		HImageMgr m_Images;
		ImageMap   m_imageMap;
	};
}

#endif