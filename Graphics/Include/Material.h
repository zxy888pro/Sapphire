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

		//����y��ľ��������Ԫ��ӳ��
		std::map<TextureUnit, RHANDLE> m_textures;

	};
}