#pragma once
#include <Graphics.h>

namespace Sapphire
{
	struct ITextureMgr;
	struct IImageMgr;
	struct ITexture;

	class GraphicDriver : public Singleton<GraphicDriver>
	{
	public:

		typedef std::unordered_map<std::string, ImageType>   ImageTypeNameMap;

		GraphicDriver();
		virtual ~GraphicDriver();

		void Init();

		void Release();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		//绑定一个纹理到指定的纹理单元
		//先激活对应的纹理单元
		//然后绑定纹理对象
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

		PixelFormat GetPixelFormat(ImageType eImgType);

		PixelFormat GetPixelFormat(std::string szImageType);


		int getTextureQuality() const { return m_nTextureQuality; }

		void setTextureQuality(int val) { m_nTextureQuality = val; }

	public:

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);

		static uint GetHWTextureDataType(PixelFormat format);
		//硬件Alpha格式
		static int GetHWAlphaFormat();
		//硬件明度格式
		static int GetHWLuminanceFormat();
		//硬件明度和alpha格式
		static int GetHWLuminanceAlphaFormat();
		//硬件RGB格式
		static int GetHWRGBFormat();
		//硬件RGBA格式
		static int GetHWRGBAFormat();
		
		
	private:

		void CheckFeature();

		ITextureMgr*   m_pTextureMgr;
		IImageMgr*     m_pImageMgr;

		int            m_nTextureQuality;
		bool		   m_bAnisotropySupport;
		uint           m_nMaxTextureUnits;
		ImageTypeNameMap   m_imagetypeNames;

	};

}
