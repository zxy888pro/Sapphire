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
			m_bCompressd = false;
			m_subImgs.clear();
			m_mipmaps.clear();
		};
		virtual ~Image();

		friend class ImageMgr;

		typedef std::vector <PRAWIMAGE> MipmapArray;


		const std::string& getName() const { return m_Name; }

		bool IsCompressd() const { return m_bCompressd; }

	    void setName(std::string val) { m_Name = val; }

		int getHeight(uint nMipmap = 0) const { 
			if (nMipmap == 0)
				return m_nHeight;
			else
			{
				assert(m_subImgs.size()<m_nMipmapNum);
				m_subImgs[nMipmap - 1]->getHeight();
			}
				
		}

		int getWidth(uint nMipmap = 0) const { 

			if (nMipmap == 0)
				return m_nWidth;
			else
			{
				assert(m_subImgs.size()<m_nMipmapNum);
				m_subImgs[nMipmap - 1]->getWidth();
			}
				
		}

		ImageType getImageType() const { return m_imgType; }

		//RAWDATA 统一为4通道RGBA  3通道RGB  2通道GA  1通道G
		virtual PRAWIMAGE getData() const
		{
			return m_pbData;
		}

		virtual const PRAWIMAGE getMipmap(uint nMipmap = 0) const
		{
			assert(nMipmap < m_nMipmapNum);
			return m_mipmaps[nMipmap];
		}
		virtual uint getMipmapNum() const
		{
			return m_nMipmapNum;
		}

		virtual  uint getRowSize() const { return m_uRowSize; }

		virtual ulonglong getDataSize() const { return m_ullDataSize; }

		virtual bool Load(const std::string& imagePath, bool bCreateMipmap=false);

		virtual void Unload(void);

		virtual int getChannels() const { return m_nChannels; } 

		virtual void SetRAWData(void* dataPtr, int width, int height, int nChannels);
		

	private:

		std::string  m_Name;       
		int m_nWidth;
		int m_nHeight;
		int m_nChannels;
		ulonglong    m_ullDataSize;
		uint		 m_uRowSize;
		PRAWIMAGE	     m_pbData;
		ImageType    m_imgType;
		int          m_nMipmapNum;
		bool         m_bCompressd;
		MipmapArray  m_mipmaps;
		//mipmap 0 就是本身，不放在subImg里了
		std::vector<Image*> m_subImgs;
		void GenerateMipmaps();
 
		
	};


	//定义Image句柄
	typedef Handle <Image> HIMAGE;


	 
}

#endif