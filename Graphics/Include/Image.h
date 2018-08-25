#ifndef __IMAGE__H__
#define __IMAGE__H__

#include "Graphics.h"
#include <cassert>
#include <map>

namespace Sapphire
{
	
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


	 
}

#endif