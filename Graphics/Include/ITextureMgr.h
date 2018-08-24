#pragma once
#include <Graphics.h>
#include <ITexture.h>
#include <Image.h>

namespace Sapphire
{
	struct ITextureMgr
	{
		  
		virtual ITexture* CreateTexture2DFromFile(std::string filePath, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT, bool bDynamic = false) = 0;

		virtual ITexture* CreateTexture2DFromImage(HIMAGE himg, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) = 0;

		virtual ITexture* CreateTexture2DFromRAW(byte* pData, uint width, uint height, PixelFormat pFormat = PF_R8G8B8A8, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) = 0;

		virtual ITexture* CreateEmptyTexture2D(uint width, uint height, PixelFormat pFormat = PF_R8G8B8A8, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) = 0;

		virtual ITexture*   RequestTexture(RHANDLE handle) = 0;

		virtual void        DiscardTexture(ITexture* pTexture) = 0;

		virtual void SetTexture(ITexture* pTexture, TextureUnit index) = 0;

		virtual ITexture* GetTexture(TextureUnit index) = 0;
	};
}