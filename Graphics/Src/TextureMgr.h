#pragma once
#include <Sapphire.h>
#include <BaseResource.h>
#include <ITextureMgr.h>

namespace Sapphire
{
	//OpenGL TextureManager
	class TextureMgr :public ITextureMgr, ResourceMgr
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

		//������������Ԫ
		virtual void SetTexture(ITexture* pTexture, TextureUnit index) override;

		virtual ITexture* GetTexture(TextureUnit index) override;

		//����Ӳ���������ID�Ƿ��ѷ���
		virtual bool VerifyHWUID(uint uHwuid) override;


		virtual ITexture* CreateCubeTextureFromImage(HIMAGE himg, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT) override;

		//CubeTexture�ļ���ʱ����Ϊ��׺��Ϊ.cub��һ������6��ͼƬ���ı��ļ�
		virtual ITexture* CreateCubeTextureFromFile(std::string filePath, TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT, bool bDynamic = false) override;

	protected:



	private:

		//����ʹ�õ�����
		ITexture*      m_textures[MAX_TEXTURE_UNITS];
		uint           m_textureTypes[MAX_TEXTURE_UNITS];
		int            m_nCurActiveTexUnit;

		GraphicDriver* m_pGraphicDriver;

	};
}