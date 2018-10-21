#include <Sapphire.h>
#include <GraphicsDefs.h>
#include <Image.h>
#include <IImageMgr.h>

namespace Sapphire
{
	//暂时未加入资源缓存管理功能
	class ImageMgr : public IImageMgr
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
		uint GetWidth(HIMAGE himg, uint nMipmap = 0) const
		{
			return (m_Images.Dereference(himg)->getWidth(nMipmap));
		}
		uint GetHeight(HIMAGE himg, uint nMipmap = 0) const
		{
			return (m_Images.Dereference(himg)->getHeight(nMipmap));
		}
		uint GetNumChannels(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getChannels());
		}
		ulonglong GetImageSize(HIMAGE himg) const
		{
			return (m_Images.Dereference(himg)->getDataSize());
		}
		uint GetNumMipmaps(HIMAGE himg) const
		{
			return  (m_Images.Dereference(himg)->getMipmapNum());
		}

		PRAWIMAGE  GetTexture(HIMAGE himg, uint mip = 0) const
		{
			Image*  pImg = (Image*) m_Images.Dereference(himg);
			if (pImg)
			{
				return pImg->getMipmap(mip);
			}
			return NULL;
		}

		bool IsCompressd(HIMAGE himg) const
		{
			return  (m_Images.Dereference(himg)->IsCompressd());;
		}

		virtual ImageType GetImageType(HIMAGE himg) const override;

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