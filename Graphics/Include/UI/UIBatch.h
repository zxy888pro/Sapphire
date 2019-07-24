#pragma once
#include "Graphics.h"
#include "Math/Rect.h"



namespace Sapphire
{
	struct ITexture;
	 

	class SAPPHIRE_CLASS UIBatch
	{

	public:

		UIBatch();



	private:

		BlendMode   m_blendMode;
		ITexture*   m_pTexture;
		std::vector<float>*   m_vertexData; //��������
		uint				  m_vertexStart;
		uint				  m_vertexEnd;

		IntRect		m_scissor; //��������

	};


}



