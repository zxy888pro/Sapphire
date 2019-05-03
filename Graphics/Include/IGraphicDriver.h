#pragma once

#include "Graphics.h"


namespace Sapphire
{
	struct  ITextureMgr;
	struct  IImageMgr;
	

	//OpenGL ��Ӳ������������ࡣ  
	//�޹�ͼ��API
	class SAPPHIRE_CLASS IGraphicDriver : public SubSystem
	{
		SAPPHIRE_OBJECT(IGraphicDriver, SubSystem)

	public:

		IGraphicDriver(Core* pCore):
			SubSystem(pCore)
		{
			 
		}
		virtual ~IGraphicDriver(){}

		virtual void Init() = 0;
		virtual void Release() = 0;

		virtual void PrepareDraw() = 0;
		
		virtual ITextureMgr* getTextureMgr() const = 0;
		
		virtual IImageMgr* getImageMgr() const = 0;
		virtual GraphicDriverType getDriverType() const = 0;

	protected:

		 

	};
}