#pragma once

namespace Sapphire
{

	enum ImageType
	{
		ImageType_Bmp_R8G8B8,
		ImageType_Bmp_A8R8G8B8,
		ImageType_Jpeg,
		ImageType_Png,
		ImageType_Tga32,
		ImageType_Tga24,
		ImageType_RAW_R8G8B8A8,
		ImageType_RAW_R8G8B8,
		ImageType_Unknown
	};

	enum LockState
	{
		LOCK_NONE = 0,
		LOCK_HARDWARE,
		LOCK_SHADOW,
		LOCK_SCRATCH
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

	enum VertexElement
	{
		ELEMENT_POSITION = 0,
		ELEMENT_NORMAL,
		ELEMENT_COLOR,
		ELEMENT_TEXCOORD1,
		ELEMENT_TEXCOORD2,
		ELEMENT_CUBETEXCOORD1,
		ELEMENT_CUBETEXCOORD2,
		ELEMENT_TANGENT,
		ELEMENT_BLENDWEIGHTS,
		ELEMENT_BLENDINDICES,
		ELEMENT_INSTANCEMATRIX1,
		ELEMENT_INSTANCEMATRIX2,
		ELEMENT_INSTANCEMATRIX3,
		MAX_VERTEX_ELEMENTS
	};

#define	QUALITY_LOW  0
#define	QUALITY_MEDIUM  1
#define	QUALITY_HIGH  2
#define	QUALITY_MAX  15
#define MAX_TEXTURE_QUALITY_LEVELS  3

	//顶点元素掩码
	static const unsigned MASK_NONE = 0x0;
	static const unsigned MASK_POSITION = 0x1;
	static const unsigned MASK_NORMAL = 0x2;
	static const unsigned MASK_COLOR = 0x4;
	static const unsigned MASK_TEXCOORD1 = 0x8;
	static const unsigned MASK_TEXCOORD2 = 0x10;
	static const unsigned MASK_CUBETEXCOORD1 = 0x20;
	static const unsigned MASK_CUBETEXCOORD2 = 0x40;
	static const unsigned MASK_TANGENT = 0x80;
	static const unsigned MASK_BLENDWEIGHTS = 0x100;
	static const unsigned MASK_BLENDINDICES = 0x200;
	static const unsigned MASK_INSTANCEMATRIX1 = 0x400;
	static const unsigned MASK_INSTANCEMATRIX2 = 0x800;
	static const unsigned MASK_INSTANCEMATRIX3 = 0x1000;
	static const unsigned MASK_DEFAULT = 0xffffffff;
	static const unsigned NO_ELEMENT = 0xffffffff;

	//最大顶点流数, 决定同时有多少顶点缓冲区可用
	static const int MAX_VERTEX_STREAMS = 4;
	//最大渲染目标
	static const int MAX_RENDERTARGETS = 4;

}

