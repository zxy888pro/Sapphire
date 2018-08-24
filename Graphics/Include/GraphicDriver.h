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

		GraphicDriver();
		virtual ~GraphicDriver();

		void Init();

		void Release();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		//绑定一个纹理到指定的纹理单元
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		bool  IsDeviceLost();

	public:

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);
		
	private:

		ITextureMgr*   m_pTextureMgr;
		IImageMgr*     m_pImageMgr;
		

	};

}
