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
		LOCK_SHADOW,  //Ӱ�ӻ���
		LOCK_SCRATCH   //Scratch��ʱ����
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

	//ͼԪ����
	enum PrimitiveType
	{
		TRIANGLE_LIST = 0,
		LINE_LIST,
		POINT_LIST,
		TRIANGLE_STRIP,
		LINE_STRIP,
		TRIANGLE_FAN
	};

	/// �ӿ���Ⱦ����ĸ���ģʽ
	enum RenderSurfaceUpdateMode
	{
		SURFACE_MANUALUPDATE = 0,
		SURFACE_UPDATEVISIBLE,
		SURFACE_UPDATEALWAYS
	};

	enum GraphicDriverType
	{
		GRAPHICDRIVER_OPENGL,
		GRAPHICDRIVER_DIRECTX,
		GRAPHICDRIVER_VULKAN,
		MAX_GRAPHICDRIVERS
	};

	/// Shader ����.
	enum ShaderType
	{
		VS = 0,  //������ɫ��
		PS,		//������ɫ��
		GS,     //������ɫ��
		CS,      //������ɫ��
		UNKNOWN
	};
	/// ���ģʽ
	enum FillMode
	{
		FILL_SOLID = 0,
		FILL_WIREFRAME,
		FILL_POINT
	};

	/// Alpha���ģʽ
	enum BlendMode
	{
		BLEND_REPLACE = 0,
		BLEND_ADD,
		BLEND_MULTIPLY,
		BLEND_ALPHA,
		BLEND_ADDALPHA,
		BLEND_PREMULALPHA,
		BLEND_INVDESTALPHA,
		BLEND_SUBTRACT,
		BLEND_SUBTRACTALPHA,
		MAX_BLENDMODES
	};
	/// ��Ȼ�ģ��Ƚ�ģʽ
	enum CompareMode
	{
		CMP_ALWAYS = 0,
		CMP_EQUAL,
		CMP_NOTEQUAL,
		CMP_LESS,
		CMP_LESSEQUAL,
		CMP_GREATER,
		CMP_GREATEREQUAL,
		MAX_COMPAREMODES
	};

	//�޳�ģʽ
	enum CullMode
	{
		CULL_NONE = 0,
		CULL_CCW,
		CULL_CW,
		MAX_CULLMODES
	};

	//ģ�����
	enum StencilOp
	{
		OP_KEEP = 0,
		OP_ZERO,
		OP_REF,
		OP_INCR,
		OP_DECR
	};

	/// Billboardx�������ģʽ
	enum FaceCameraMode
	{
		FC_NONE = 0,
		FC_ROTATE_XYZ,
		FC_ROTATE_Y,
		FC_LOOKAT_XYZ,
		FC_LOOKAT_Y
	};


	/// shader ������ ���ھ����Ƿ����. 
	enum ShaderParameterGroup
	{
		SP_FRAME = 0,
		SP_CAMERA,
		SP_ZONE,
		SP_LIGHT,
		SP_MATERIAL,
		SP_OBJECT,
		SP_CUSTOM,
		MAX_SHADER_PARAMETER_GROUPS
	};

#define	QUALITY_LOW  0
#define	QUALITY_MEDIUM  1
#define	QUALITY_HIGH  2
#define	QUALITY_MAX  15
#define MAX_TEXTURE_QUALITY_LEVELS  3
#define MAX_SCRIPT_LENGTH  8192


	//Ĭ����������
	static const unsigned DEFAULT_VIEWMASK = 0xffffffff;
	static const unsigned DEFAULT_LIGHTMASK = 0xffffffff;
	static const unsigned DEFAULT_SHADOWMASK = 0xffffffff;
	static const unsigned DEFAULT_ZONEMASK = 0xffffffff;



	//����Ԫ������
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
	static const unsigned MAX_FOV = 160;


	static const unsigned CLEAR_COLOR = 0x1;
	static const unsigned CLEAR_DEPTH = 0x2;
	static const unsigned CLEAR_STENCIL = 0x4;

	//��󶥵�����, ����ͬʱ�ж��ٶ��㻺��������
	static const int MAX_VERTEX_STREAMS = 4;
	//�����ȾĿ����
	static const int MAX_RENDERTARGETS = 4;

}

