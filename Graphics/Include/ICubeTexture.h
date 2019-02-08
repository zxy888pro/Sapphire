#pragma once
#include <GraphicsDefs.h>
#include <ITexture.h>

namespace Sapphire
{
	class ICubeTexture : public ITexture
	{
	public:
		ICubeTexture(){};
		virtual ~ICubeTexture(){};




		virtual uint getWidth() const override
		{
			return 0;
		}


		virtual uint getHeight() const override
		{
			return 0;
		}


		virtual uint getLevelWidth(uint level) const override
		{
			return 0;
		}


		virtual uint getLevelHeight(uint level) const override
		{
			return 0;
		}


		virtual uint getDepth() const override
		{
			return 0;
		}


		virtual void setDepth(uint val) override
		{
			return;
		}


		virtual uint getNumMipmaps() const override
		{
			return 0;
		}


		virtual void setNumMipmaps(uint val) override
		{
			return;
		}


		virtual uint getMipLevel() const override
		{
			return 0;
		}


		virtual void setMipLevel(uint val) override
		{
			return;
		}


		virtual bool getIsCompress() const override
		{
			return 0;
		}


		virtual void setIsCompress(bool val) override
		{
			return;
		}


		virtual PixelFormat getPixelFormat() const override
		{
			return PixelFormat::PF_A8R8G8B8;
		}


		virtual void setPixelFormat(PixelFormat val) override
		{
			return;
		}


		virtual Sapphire::TextureFilterMode getFilterMode() const override
		{
			return TextureFilterMode::FILTER_BILINEAR;
		}


		virtual void setFilterMode(Sapphire::TextureFilterMode val) override
		{
			return;
		}


		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood) override
		{
			return TextureAddressMode::ADDRESS_REPEAT;
		}


		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode) override
		{
			return;
		}


		virtual uint GetDataSize() const override
		{
			return 0;
		}


		virtual uint GetRowSize() const override
		{
			return 0;
		}


		virtual std::string getName() const override
		{
			return 0;
		}


		virtual void setName(std::string val) override
		{
			return;
		}


		virtual uint getAnisotropyLevel() const override
		{
			return 0;
		}


		virtual void setAnisotropyLevel(uint val) override
		{
			return;
		}


		virtual Sapphire::TextureUsage getUsage() const override
		{
			return TextureUsage::TEXTURE_STATIC;
		}


		virtual void setUsage(Sapphire::TextureUsage val) override
		{
			return;
		}


		virtual int getTextureType() const override
		{
			return 0;
		}


		virtual uint getUID() const override
		{
			return 0;
		}


		virtual ITexture* getBackupTexture() const override
		{
			return 0;
		}


		virtual void setBackupTexture(ITexture* tex) override
		{
			return;
		}

		virtual void RenderSurfaceUpdate() override
		{
			return;
		}

		virtual uint getRequestMipLevel() const override
		{
			return 0;
		}
		virtual void setRequestMipLevel(uint level) override
		{
			return;
		}

		virtual bool IsCompressed() const
		{
			return false;
		}

	};
}