#pragma once

namespace Sapphire
{

	enum ImageType
	{
		ImageType_Bmp_R8G8B8,
		ImageType_Bmp_A8R8G8B8,
		ImageType_Jpeg,
		ImageType_Png,
		ImageType_Tga,
		ImageType_RAW_R8G8B8A8,
		ImageType_RAW_R8G8B8,
		ImageType_Unknown
	};


	enum TextureUsage
	{
		TEXTURE_STATIC = 0,
		TEXTURE_DYNAMIC,
		TEXTURE_RENDERTARGET,
		TEXTURE_DEPTHSTENCIL
	};

	enum TextureFilterMode
	{
		FILTER_NEAREST = 0,
		FILTER_BILINEAR,
		FILTER_TRILINEAR,
		FILTER_ANISOTROPIC,
		FILTER_DEFAULT,
		MAX_FILTERMODES
	};

	enum TextureCoordinate
	{
		COORD_U = 0,
		COORD_V,
		COORD_W,
		MAX_COORDS
	};

	enum CubeMapFace
	{
		FACE_POSITIVE_X = 0,
		FACE_NEGATIVE_X,
		FACE_POSITIVE_Y,
		FACE_NEGATIVE_Y,
		FACE_POSITIVE_Z,
		FACE_NEGATIVE_Z,
		MAX_CUBEMAP_FACES
	};

	enum TextureAddressMode
	{
		ADDRESS_REPEAT = 0,
		ADDRESS_MIRROR,
		ADDRESS_CLAMP,
		ADDRESS_BORDER,
		MAX_ADDRESSMODES
	};

	enum TextureUnit
	{
		TU_DIFFUSE = 0,
		TU_NORMAL = 1,
		TU_SPECULAR = 2,
		TU_EMISSIVE = 3,
		TU_ENVIRONMENT = 4,
		TU_CUBEMAP = 5,
		TU_LIGHTRAMP = 6,
		TU_LIGHTSHAPE = 7,
		TU_SHADOWMAP = 8,
		TU_POSTEFFECT = 9,
		MAX_TEXTURE_UNITS
	};

	enum PixelFormat
	{
		PF_L8,
		PF_L16,
		PF_A8,
		PF_A4L4,
		PF_R8G8B8A8,
		PF_A8R8G8B8,
		PF_A8B8G8R8,
		PF_R8G8B8,
		PF_B8G8R8,
		PF_R8,
		PF_UNDEFINED,
		MAX_PIXELFORMATS
	};


 

}

