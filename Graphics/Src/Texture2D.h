#pragma once

#include <Sapphire.h>
#include <Graphics.h>
#include "GPUObject.h"
#include <Image.h>
#include <GraphicsDefs.h>
#include <BaseResource.h>
#include <ITexture.h>

namespace Sapphire
{

	class Texture2D : public GPUObject, public BaseResource, public ITexture
	{
	public:

		friend class TextureMgr;

		Texture2D();
		Texture2D(uint width, uint height, uint depth, PixelFormat pf = PF_R8G8B8A8, uint NumMipmaps = 1, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR);
		virtual ~Texture2D();
		virtual void Release();
		virtual bool Recreate();
		virtual void Dispose();
		virtual size_t  GetSize();
		virtual bool  IsDisposed();



		virtual void GPUObjectInit() override;
		


		virtual void SetData(void* pData) override;

		
	public:

		virtual uint getWidth() const { return m_uWidth; }
		virtual void setWidth(uint val) { m_uWidth = val; }
		virtual uint getHeight() const { return m_uHeight; }
		virtual void setHeight(uint val) { m_uHeight = val; }
		virtual uint getDepth() const { return m_uDepth; }
		virtual void setDepth(uint val) { m_uDepth = val; }
		virtual uint getNumMipmaps() const { return m_uNumMipmaps; }
		virtual void setNumMipmaps(uint val) { m_uNumMipmaps = val; }
		virtual uint getMipLevel() const { return m_mipLevel; }
		virtual void setMipLevel(uint val) { m_mipLevel = val; }
		virtual bool getIsCompress() const { return m_bIsCompress; }
		virtual void setIsCompress(bool val) { m_bIsCompress = val; }
		virtual PixelFormat getPixelFormat() const { return m_ePixelFormat; }
		virtual void setPixelFormat(PixelFormat val) { m_ePixelFormat = val; }
		virtual Sapphire::TextureFilterMode getFilterMode() const { return m_eFilterMode; }
		virtual void setFilterMode(Sapphire::TextureFilterMode val) { m_eFilterMode = val; }
		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood){ return m_eAddressMode_[crood]; }
		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode){ m_eAddressMode_[crood] = mode; }
		virtual std::string getName() const { return m_szName; }
		virtual uint  getAnisotropyLevel() const { return m_uAnisotropyLevel; }
		virtual void setAnisotropyLevel(uint val) { m_uAnisotropyLevel = val; }

	private:

		uint m_uWidth;
		uint m_uHeight;
		uint m_uDepth;
		uint m_uNumMipmaps;
		uint m_mipLevel;
		uint m_uAnisotropyLevel;
		bool m_bIsCompress;
		bool m_bIsDisposed;
		HIMAGE m_hImg;
		PixelFormat m_ePixelFormat;
		TextureFilterMode  m_eFilterMode;
		TextureAddressMode m_eAddressMode_[MAX_COORDS];
		TextureUsage   m_eUsage;
		std::string m_szName;

	    
	};

}


