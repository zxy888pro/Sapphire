#pragma once
#include <GraphicsDefs.h>
#include <Image.h>
#include <vector>
#include <map>

namespace Sapphire
{
	class SAPPHIRE_CLASS Material
	{
	public:

		Material();
		virtual ~Material();

		

	private:

		//保存紋理的句柄和纹理单元的映射
		std::map<TextureUnit, RHANDLE> m_textures;

	};
}