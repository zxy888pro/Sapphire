#pragma once
#include "Sapphire.h"
#include "Math/Frustum.h"
#include "Math/Ray.h"
#include "Component.h"


namespace Sapphire
{
	static const float DEFAULT_NEARCLIP = 0.1f;
	static const float DEFAULT_FARCLIP = 1000.0f;
	static const float DEFAULT_CAMERA_FOV = 45.0f;
	static const float DEFAULT_ORTHOSIZE = 20.0f;

	static const unsigned VO_NONE = 0x0;
	static const unsigned VO_LOW_MATERIAL_QUALITY = 0x1;
	static const unsigned VO_DISABLE_SHADOWS = 0x2;
	static const unsigned VO_DISABLE_OCCLUSION = 0x4;

	//相机组件
	class SAPPHIRE_CLASS Camera :public Component
	{
	public:


	};

}