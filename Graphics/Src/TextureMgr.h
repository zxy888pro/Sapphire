#pragma once
#include <Sapphire.h>
#include <BaseResource.h>
#include <ITextureMgr.h>

namespace Sapphire
{
	class TextureMgr :public ITextureMgr
	{
	public:

		TextureMgr();

		virtual ~TextureMgr();


		virtual ITexture* CreateTexture2DFromFile(std::string filePath, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT,bool bDynamic = false) override;


		virtual ITexture* CreateTexture2DFromImage(HIMAGE himg, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) override;


		virtual ITexture* CreateTexture2DFromRAW(byte* pData, uint width, uint height, PixelFormat pFormat = PF_R8G8B8A8, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) override;


		virtual ITexture* CreateEmptyTexture2D(uint width, uint height, PixelFormat pFormat = PF_R8G8B8A8, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) override;


		virtual ITexture* RequestTexture(RHANDLE handle) override;


		virtual void DiscardTexture(ITexture* pTexture) override;

	protected:



	private:


	};
}