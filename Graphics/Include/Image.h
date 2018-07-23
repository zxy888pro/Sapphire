#ifndef __IMAGE__H__
#define __IMAGE__H__

#include "SapphireGraphics.h"
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

		virtual byte* getData() const
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
		byte*	     m_pbData;
		ImageType    m_imgType;
		int          m_nMipmapNum;
		MipmapArray  m_mipmaps;

		void GenerateMipmaps();

		

		 
		
	};

	//¶¨ÒåImage¾ä±ú
	typedef Handle <Image> HIMAGE;

	class SAPPHIRE_GRAPHICS_CLASS ImageMgr
	{
	public:

		ImageMgr();
		virtual ~ImageMgr();

		typedef HandleMgr <Image, HIMAGE> HImageMgr;	

		HIMAGE   LoadImage(const char* name);

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
		ulonglong GetImageSize(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getDataSize());
		}


		/*OsHandle GetTexture(HIMAGE himg, unsigned int mip = 0) const
		{
		return (m_Textures.Dereference(himg)->GetOsHandle(mip));
		}*/

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