#pragma once
#include "Sapphire.h"
#include "Graphics.h"

byte* LoadImageFile(const char* path, int* width, int* height, int* channels);

int ResizeImage(byte* pSrcImg, UINT parentWidth, UINT parentHeight, UINT parentStrideBytes, byte* pDestImg, UINT subWidth, UINT subHeight, UINT subStrideBytes, UINT channels);

void FreeImage(byte* pImg);