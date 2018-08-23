#pragma once
#include <Sapphire.h>
#include <singleton.h>
#include <Graphics.h>
#include <ITextureMgr.h>


namespace Sapphire
{

	class GraphicDriver : public Singleton<GraphicDriver>
	{
	public:

		GraphicDriver();
		virtual ~GraphicDriver();
		void Init();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);
		
	private:

		ITextureMgr*   m_pTextureMgr;

	};

}
