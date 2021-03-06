#pragma once
#include <Graphics.h>
#include <Image.h>

namespace Sapphire
{
	struct IImageMgr
	{
		virtual HIMAGE GetImage(const char* name) = 0;

		virtual void   DeleteTexture(HIMAGE himg) = 0;

		virtual const std::string& GetName(HIMAGE himg) const = 0;

		virtual uint GetWidth(HIMAGE himg, uint nMipmap = 0) const = 0;

		virtual uint GetHeight(HIMAGE himg, uint nMipmap = 0) const = 0;

		virtual uint GetNumChannels(HIMAGE himg) const = 0;

		virtual ulonglong GetImageSize(HIMAGE himg) const = 0;

		virtual PRAWIMAGE  GetImageRaw(HIMAGE himg, uint mip = 0) const = 0;

		virtual ImageType  GetImageType(HIMAGE himg) const = 0;

		virtual uint GetNumMipmaps(HIMAGE himg) const = 0;

		virtual bool IsCompressd(HIMAGE himg) const = 0;

	};

	 

}

