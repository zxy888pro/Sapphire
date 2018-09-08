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

		std::map<TextureUnit, HIMAGE> m_textures;

	};
}