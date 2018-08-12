#include "SDFTest.h"
#include "FileStream.h"


namespace Sapphire
{




	SDFTest::SDFTest()
	{

	}

	SDFTest::~SDFTest()
	{

	}

	void SDFTest::init(std::string sdfFontName, int width, int height)
	{
		std::string fontPath = sdfFontName + ".TTF_sdf.txt";
		FileStream fs;
		bool ret = fs.Open(fontPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (ret)
		{
			std::string s1 = fs.ReadLine();
			std::string s2 = fs.ReadLine();

			fs.Release();
		}
	}

}