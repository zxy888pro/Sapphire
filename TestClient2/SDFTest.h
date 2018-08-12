#ifndef __SDF_TEST__
#define  __SDF_TEST__

#include "stdafx.h"

namespace Sapphire
{
	struct  SDFCharDataItem
	{
		uint id;
		uint x;
		uint y;
		uint width;
		uint height;
		uint offsetX;
		uint offsetY;
		uint xAdvance;
		uint yAdvance;
		uint page;
		uint chnl;

	};
	

	class SDFTest
	{
	public:

		SDFTest();
		virtual ~SDFTest();

		

		void init(std::string sdfFontName, int width, int height);

	private:

		std::string faceName;
		std::map<uint, SDFCharDataItem*> charDataMap;
		GLuint uTextureID;
	 
	};


}

#endif // __SDF_TEST__
