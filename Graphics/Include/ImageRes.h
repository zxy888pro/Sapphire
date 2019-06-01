#ifndef __IMAGE_RES__
#define  __IMAGE_RES__
#include "Graphics.h"
#include <cassert>
#include <map>

namespace Sapphire
{

	class SAPPHIRE_CLASS ImageRes : public BaseResource
	{
		SAPPHIRE_OBJECT(ImageRes, BaseResource)
	public:

		ImageRes(Core* pCore, const char* name = "") :BaseResource(pCore, name)
		{
			m_eType = ResourceType_Image;
			m_eState = ResourceState_Unload;
			m_imgType = ImageType::ImageType_Unknown;
			m_ullDataSize = 0;
			m_Name = "";
			m_pbData = NULL;
			m_nHeight = 0;
			m_nWidth = 0;
			m_nMipmapNum = 0;
			m_nChannels = 0;
			m_bCompressd = false;
			SetCreateMipmap(false);
			m_subImgs.clear();
			m_mipmaps.clear();
		};

		ImageRes(Core* pCore, bool bCreateMipmap, const char* name = "") :ImageRes(pCore, name)
		{
			SetCreateMipmap(bCreateMipmap);
		};
		virtual ~ImageRes();


		typedef std::vector <PRAWIMAGE> MipmapArray;


		const std::string& getName() const { return m_Name; }

		bool IsCompressd() const { return m_bCompressd; }

		void setName(std::string val) { m_Name = val; }

		int getHeight(uint nMipmap = 0) const {
			if (nMipmap == 0)
				return m_nHeight;
			else
			{
				assert(m_subImgs.size() < m_nMipmapNum);
				m_subImgs[nMipmap - 1]->getHeight();
			}

		}

		int getWidth(uint nMipmap = 0) const {

			if (nMipmap == 0)
				return m_nWidth;
			else
			{
				assert(m_subImgs.size() < m_nMipmapNum);
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
		bool IsCreateMipmap() const { return m_bCreateMipmap; }

		void SetCreateMipmap(bool val) { m_bCreateMipmap = val; }

		virtual  uint getRowSize() const { return m_uRowSize; }

		virtual ulonglong getDataSize() const { return m_ullDataSize; }


		virtual bool Load(bool bCreateMipmap);

		virtual bool Load() override;

		virtual bool Load(const char* resPath) override;

		virtual int getChannels() const { return m_nChannels; }

		virtual void SetRAWData(void* dataPtr, int width, int height, int nChannels);



		virtual void Clear() override;


		virtual bool Create() override;


		virtual void Destroy() override;


		virtual bool Recreate() override;


		virtual void Dispose() override;


		virtual void OnLoadStart() override;


		virtual void OnLoadEnd() override;


		virtual void OnLoadError() override;


		virtual size_t GetSize() override;


		virtual bool IsDisposed() override;

		
	private:

		std::string  m_Name;
		int m_nWidth;
		int m_nHeight;
		int m_nChannels;
		bool m_bCreateMipmap;
		ulonglong    m_ullDataSize;
		uint		 m_uRowSize;
		PRAWIMAGE	     m_pbData;
		ImageType    m_imgType;
		int          m_nMipmapNum;
		bool         m_bCompressd;
		MipmapArray  m_mipmaps;
		//mipmap 0 就是本身，不放在subImg里了
		std::vector<ImageRes*> m_subImgs;
		void GenerateMipmaps();


	};

}


#endif