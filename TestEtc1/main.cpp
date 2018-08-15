#include "stdafx.h"
#include <iostream>
#include <string>
#include <FileStream.h>
#include "etc1.h"
#include <stb_image.h>
#include <stb_image_write.h>


struct RGBA32_Data
{
	byte r;
	byte g;
	byte b;
	byte a;
	
};

struct RGB24_Data
{
	byte r;
	byte g;
	byte b;
	 
};


int main()
{
	Sapphire::FileStream fs;
	//bool bRet = fs.Open("2.tga", Sapphire::FileMode::FILE_EXIST | Sapphire::FileMode::FILE_READ);

	int width;
	int height;
	int channel;
	/*byte* pRGBAData = stbi_load("55589.jpg", &width, &height, &channel, 0);
	byte* pRGBData = new byte[width * height * 3];
	memset(pRGBData, 0, width * height * 3);

	for (int row = 0; row < height; row++)
	{
	uint pitch32 = width * row * channel;
	uint pitch24 = width * row * 3;
	for (int col = 0; col < width; col++)
	{
	byte* pixelSrc = &pRGBAData[pitch32 + col * channel];
	if (channel == 3)
	{
	RGB24_Data* pSrc = (RGB24_Data*)pixelSrc;
	byte* pixelDst = &pRGBData[pitch24 + col * 3];
	RGB24_Data* pDst = (RGB24_Data*)pixelDst;
	memcpy(pDst, pSrc, sizeof(RGB24_Data));
	}
	else if (channel == 4)
	{
	RGBA32_Data* pRGBA = (RGBA32_Data*)pixelSrc;
	byte* pixelDesc = &pRGBData[pitch24 + col * 3];
	RGB24_Data* pRGB = (RGB24_Data*)pixelDesc;
	pRGB->r = pRGBA->r;
	pRGB->g = pRGBA->g;
	pRGB->b = pRGBA->b;
	}


	}
	}
	uint pkmSize = etc1_get_encoded_data_size(width, height);
	byte* pkmData = new byte[pkmSize];
	etc1_encode_image(pRGBData, width, height, 3, width * 3, pkmData);
	Sapphire::Path curPath = Sapphire::GetCurrentDirA();

	Sapphire::Path outfile = curPath;
	outfile = outfile.addTailSlash() + "55589.etc";
	byte pkmHeader[ETC_PKM_HEADER_SIZE];
	memset(pkmHeader, 0, ETC_PKM_HEADER_SIZE);

	etc1_pkm_format_header(pkmHeader, width, height);

	if (fs.Open(outfile.c_str(), Sapphire::FileMode::FILE_WRITE))
	{
	fs.Write(pkmHeader, ETC_PKM_HEADER_SIZE);
	fs.Write(pkmData, pkmSize);
	fs.Release();
	}
	if (pRGBAData)
	stbi_image_free(pRGBAData);
	if (pRGBData)
	delete[] pRGBData;
	if (pkmData)
	delete[] pkmData;*/

	{
		Sapphire::FileStream fs;
		if (fs.Open("55589.etc", Sapphire::FILE_EXIST | Sapphire::FILE_READ))
		{
			uint uLen = fs.GetLength();
			byte* pData = new byte[uLen];
			fs.Read(pData, uLen);
			fs.Release();
			int width = 0;
			int height = 0;
			width = etc1_pkm_get_width(pData);
			height = etc1_pkm_get_height(pData);
			byte* pRGBData = new byte[width * height * 3];
			etc1_decode_image((pData + ETC_PKM_HEADER_SIZE), pRGBData, width, height, 3, width * 3);
			stbi_write_jpg("55589_2.jpg", width, height, 3, pRGBData, 0);
			delete[] pRGBData;
			delete[] pData;
		}
	}

	return 0;
}
