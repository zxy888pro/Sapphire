#pragma once
#include <Graphics.h>


namespace Sapphire
{

	struct ITexture
	{
		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;
		virtual uint getLevelWidth(uint level) const = 0;  //获取指定mipmap等级下的宽度
		virtual uint getLevelHeight(uint level) const = 0; ////获取指定mipmap等级下的高度
		virtual uint getDepth() const = 0;
		virtual void setDepth(uint val) = 0;
		virtual uint getNumMipmaps() const = 0;
		virtual void setNumMipmaps(uint val) = 0;
		virtual uint getMipLevel() const = 0;
		virtual void setMipLevel(uint val) = 0;
		virtual bool getIsCompress() const = 0;
		virtual void setIsCompress(bool val) = 0;
		virtual PixelFormat getPixelFormat() const = 0;
		virtual void setPixelFormat(PixelFormat val) = 0;
		virtual Sapphire::TextureFilterMode getFilterMode() const = 0;
		virtual void setFilterMode(Sapphire::TextureFilterMode val) = 0;
		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood) = 0;
		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode) = 0;
		virtual uint  GetDataSize() const = 0;
		virtual uint  GetRowSize() const = 0;
		virtual std::string getName() const = 0;
		virtual void setName(std::string val) = 0;
		virtual uint  getAnisotropyLevel() const = 0;
		virtual void setAnisotropyLevel(uint val) = 0;
		virtual Sapphire::TextureUsage getUsage() const = 0;
		virtual void setUsage(Sapphire::TextureUsage val) = 0;
		virtual int getTextureType() const = 0;
		virtual uint getUID() const = 0;
	
	};
}