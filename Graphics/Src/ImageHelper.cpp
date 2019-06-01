#include "ImageHelper.h"
#include "stb_image.h"
#include "stb_image_resize.h"
#include "mathHelper.h"

byte* LoadImageFile(const char* path, int* width, int* height, int* channels)
{
	return stbi_load(path, width, height, channels, 0);
}

int ResizeImage(byte* pSrcImg, UINT parentWidth, UINT parentHeight, UINT parentStrideBytes, byte* pDestImg, UINT subWidth, UINT subHeight, UINT subStrideBytes, UINT channels)
{
	int ret = stbir_resize_uint8(pSrcImg, parentWidth, parentHeight, 0, pDestImg, subWidth, subHeight, 0, channels);
	return ret;
}

void FreeImage(byte* pImg)
{
	stbi_image_free(pImg);
}