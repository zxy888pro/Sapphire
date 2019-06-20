#pragma once

#include "Camera.h"
#include "Node.h"
#include "TransformComponent.h"


namespace Sapphire
{

	//翻转矩阵
	static const Matrix4x4 flipMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);

	Camera::Camera(Core* pCore) :Component(pCore),
		viewDirty_(false),
		projectionDirty_(false),
		frustumDirty_(true),
		orthographic_(false),
		nearClip_(DEFAULT_NEARCLIP),
		farClip_(DEFAULT_FARCLIP),
		fov_(DEFAULT_CAMERA_FOV),
		orthoSize_(DEFAULT_ORTHOSIZE),
		aspectRatio_(1.0f),
		zoom_(1.0f),
		lodBias_(1.0f),
		viewMask_(DEFAULT_VIEWMASK),
		viewOverrideFlags_(VO_NONE),
		fillMode_(FILL_SOLID),
		projectionOffset_(Vector2::ZERO),
		reflectionPlane_(Plane::UP),
		clipPlane_(Plane::UP),
		autoAspectRatio_(true),
		flipVertical_(false),
		useReflection_(false),
		useClipping_(false)
	{
		reflectionMatrix_ = reflectionPlane_.ReflectionMatrix();
	}

	void Camera::SetNearClip(float nearClip)
	{
		nearClip_ = MAX(nearClip, M_MIN_NEARCLIP);
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetFarClip(float farClip)
	{
		farClip_ = MAX(farClip, M_MIN_NEARCLIP);
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetFov(float fov)
	{
		fov_ = MathHelper::Clamp(fov, 0.0f, MAX_FOV);
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetOrthoSize(float orthoSize)
	{
		orthoSize_ = orthoSize;
		aspectRatio_ = 1.0f;
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetOrthoSize(const Vector2& orthoSize)
	{
		autoAspectRatio_ = false;
		orthoSize_ = orthoSize.y_;
		aspectRatio_ = orthoSize.x_ / orthoSize.y_;
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		autoAspectRatio_ = false;
		SetAspectRatioInternal(aspectRatio);
	}

	void Camera::SetFillMode(FillMode mode)
	{
		fillMode_ = mode;
	}

	void Camera::SetZoom(float zoom)
	{
		zoom_ = MAX(zoom, M_EPSILON);
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetLodBias(float bias)
	{
		lodBias_ = MAX(bias, M_EPSILON);
		
	}

	void Camera::SetViewMask(unsigned mask)
	{
		viewMask_ = mask;
	}

	void Camera::SetViewOverrideFlags(unsigned flags)
	{
		viewOverrideFlags_ = flags;
	}

	void Camera::SetOrthographic(bool enable)
	{
		orthographic_ = enable;
		frustumDirty_ = true;
		projectionDirty_ = true;
	}

	void Camera::SetAutoAspectRatio(bool enable)
	{
		autoAspectRatio_ = enable;
	}

	void Camera::SetProjectionOffset(const Vector2& offset)
	{
		projectionOffset_ = offset;
		projectionDirty_ = true;
	}

	void Camera::SetUseReflection(bool enable)
	{
		useReflection_ = enable;
		viewDirty_ = true;
		frustumDirty_ = true;
	}

	void Camera::SetReflectionPlane(const Plane& plane)
	{
		reflectionPlane_ = plane;
		reflectionMatrix_ = reflectionPlane_.ReflectionMatrix();
		viewDirty_ = true;
		frustumDirty_ = true;
	}

	void Camera::SetUseClipping(bool enable)
	{
		useClipping_ = enable;
		projectionDirty_ = true;
	}

	void Camera::SetClipPlane(const Plane& plane)
	{
		clipPlane_ = plane;
		projectionDirty_ = true;
	}

	void Camera::SetFlipVertical(bool enable)
	{
		flipVertical_ = enable;
		projectionDirty_ = true;
	}

	float Camera::GetNearClip() const
	{
		if (!orthographic_)
			return nearClip_;
		else
			return 0.0f;
	}

	const Sapphire::Frustum& Camera::GetFrustum() const
	{
		//视椎体改动了，重新计算
		if (frustumDirty_)
		{
			Matrix3x4 worldTransform = GetEffectiveWorldTransform();

			if (!orthographic_) //投射投影
				frustum_.Define(fov_, aspectRatio_, zoom_, GetNearClip(), farClip_, worldTransform);
			else
				frustum_.DefineOrtho(orthoSize_, aspectRatio_, zoom_, GetNearClip(), farClip_, worldTransform);

			frustumDirty_ = false;
		}

		return frustum_;
	}

	const Sapphire::Matrix4x4& Camera::GetProjection() const
	{
		if (projectionDirty_)
		{
			projection_ = GetProjection(true);
			projectionDirty_ = false;
		}

		return projection_;
	}

	Sapphire::Matrix4x4 Camera::GetProjection(bool apiSpecific) const
	{
		Matrix4x4 ret(Matrix4x4::ZERO);

#ifdef SAPPHIRE_OPENGL
		bool openGLFormat = apiSpecific;
#else
		bool openGLFormat = false;
#endif

		if (!orthographic_)
		{
			float nearClip = GetNearClip(); 
			float h = (1.0f / tanf(DEG_TO_RAD(fov_) * 0.5f)) * zoom_;  //以高为基准，宽通过横纵比算出   n/w/2 => 2n/(r-l) => ctg(fov/2) => 1/tg(fov/2)
			float w = h / aspectRatio_;                                //互为倒数  
			float q, r;
			/*
			r-l和t-b可以分别看作是投影平面的宽w和高h
			| 2n/(r-l)		 0		 r+l/r-l     0		 |
			| 0			2n/(t-b)	 t+b/t-b	 0		 |
			| 0				 0	   -(f+n/f-n) 2nf/(f-n)  |
			| 0				 0			-1		 0		 |

			*/
			if (openGLFormat)
			{
				q = (farClip_ + nearClip) / (farClip_ - nearClip); //以DX为基准，opengl z方向是左手坐标x轴向右, y轴向上时，z轴是相反的
				r = -2.0f * farClip_ * nearClip / (farClip_ - nearClip);
			}
			else
			{
				q = farClip_ / (farClip_ - nearClip);
				r = -q * nearClip;
			}

			ret.m00_ = w;
			ret.m02_ = projectionOffset_.x_ * 2.0f;
			ret.m11_ = h;
			ret.m12_ = projectionOffset_.y_ * 2.0f;
			ret.m22_ = q;
			ret.m23_ = r;
			ret.m32_ = 1.0f;   //以DX为基准，opengl z方向是左手坐标x轴向右,y轴向上时，z轴是相反的
		}
		else
		{
			//丢弃近剪裁平面, 它不会想透视投影影响深度的精度
			float h = (1.0f / (orthoSize_ * 0.5f)) * zoom_;
			float w = h / aspectRatio_;
			float q, r;

			if (openGLFormat)
			{
				q = 2.0f / farClip_;
				r = -1.0f;
			}
			else
			{
				q = 1.0f / farClip_;
				r = 0.0f;
			}

			ret.m00_ = w;
			ret.m03_ = projectionOffset_.x_ * 2.0f;
			ret.m11_ = h;
			ret.m13_ = projectionOffset_.y_ * 2.0f;
			ret.m22_ = q;
			ret.m23_ = r;
			ret.m33_ = 1.0f;
		}

		if (flipVertical_)
			ret = flipMatrix * ret;

		return ret;
	}

	const Sapphire::Matrix3x4& Camera::GetView() const
	{
		if (viewDirty_)
		{
			// 注意矩阵不受父节点缩放影响
			view_ = GetEffectiveWorldTransform().Inverse();
			viewDirty_ = false;
		}

		return view_;
	}

	void Camera::GetFrustumSize(Vector3& nearPoint, Vector3& farPoint) const
	{
		nearPoint.z_ = GetNearClip();
		farPoint.z_ = farClip_;

		if (!orthographic_)
		{
			float halfViewSize = tanf(DEG_TO_RAD(fov_)  * 0.5f) / zoom_;
			nearPoint.y_ = nearPoint.z_ * halfViewSize;
			nearPoint.x_ = nearPoint.y_ * aspectRatio_;
			farPoint.y_ = farPoint.z_ * halfViewSize;
			farPoint.x_ = farPoint.y_ * aspectRatio_;
		}
		else
		{
			float halfViewSize = orthoSize_ * 0.5f / zoom_;
			nearPoint.y_ = farPoint.y_ = halfViewSize;
			nearPoint.x_ = farPoint.x_ = nearPoint.y_ * aspectRatio_;
		}

		if (flipVertical_)
		{
			nearPoint.y_ = -nearPoint.y_;
			farPoint.y_ = -farPoint.y_;
		}
	}

	float Camera::GetHalfViewSize() const
	{
		if (!orthographic_)
			return tanf(DEG_TO_RAD(fov_) * 0.5f) / zoom_;
		else
			return orthoSize_ * 0.5f / zoom_;
	}

	Sapphire::Frustum Camera::GetSplitFrustum(float nearClip, float farClip) const
	{
		Frustum ret;

		Matrix3x4 worldTransform = GetEffectiveWorldTransform();
		nearClip = MAX(nearClip, GetNearClip());
		farClip = MIN(farClip, farClip_);
		if (farClip < nearClip)
			farClip = nearClip;

		if (!orthographic_)
			ret.Define(fov_, aspectRatio_, zoom_, nearClip, farClip, worldTransform);
		else
			ret.DefineOrtho(orthoSize_, aspectRatio_, zoom_, nearClip, farClip, worldTransform);

		return ret;
	}

	Sapphire::Frustum Camera::GetViewSpaceFrustum() const
	{
		Frustum ret;

		if (!orthographic_)
			ret.Define(fov_, aspectRatio_, zoom_, GetNearClip(), farClip_);
		else
			ret.DefineOrtho(orthoSize_, aspectRatio_, zoom_, GetNearClip(), farClip_);

		return ret;
	}

	Sapphire::Frustum Camera::GetViewSpaceSplitFrustum(float nearClip, float farClip) const
	{

	}

	Sapphire::Ray Camera::GetScreenRay(float x, float y) const
	{
		Ray ret;
		if (!IsProjectionValid())
		{
			if (m_pNode.NotNull())
			{
				SharedPtr<TransformComponent> transform;
				transform.DynamicCast(m_pNode->GetComponent(ComponentType_Transform)); 
				if (transform.NotNull)
				{
					ret._origin = transform->GetWorldPosition();
					ret._direction = transform->GetWorldDirection();
				}
				else
				{
					ret._origin = Vector3::ZERO;
					ret._direction = Vector3::FORWARD;
				}
				
			}
			else
			{
				ret._origin = Vector3::ZERO;
				ret._direction = Vector3::FORWARD;
			}
			return ret;
		}
		//先得到观察投影矩阵的逆矩阵
		Matrix4x4 viewProjInverse = (GetProjection(false) * GetView()).Inverse();

		// 参数范围是0.0 - 1.0. 扩展到标准设备坐标系(-1.0 to 1.0) 并且翻转y轴
		x = 2.0f * x - 1.0f;
		y = 1.0f - 2.0f * y;
		Vector3 nearPoint(x, y, 0.0f);
		Vector3 farPoint(x, y, 1.0f);
		ret._origin = viewProjInverse * nearPoint;
		ret._direction = ((viewProjInverse * farPoint) - ret._origin).Normalized();
		return ret;
	}

	Sapphire::Vector2 Camera::WorldToScreenPoint(const Vector3& worldPos) const
	{
		//转换到观察空间
		Vector3 eyeSpacePos = GetView() * worldPos;
		Vector2 ret;

		if (eyeSpacePos.z_ > 0.0f)
		{
			//转换到屏幕空间
			Vector3 screenSpacePos = GetProjection(false) * eyeSpacePos;
			ret.x_ = screenSpacePos.x_;
			ret.y_ = screenSpacePos.y_;
		}
		else
		{
			//相机背后
			ret.x_ = (-eyeSpacePos.x_ > 0.0f) ? -1.0f : 1.0f;
			ret.y_ = (-eyeSpacePos.y_ > 0.0f) ? -1.0f : 1.0f;
		}
	    //转到0.0-1.0
		ret.x_ = (ret.x_ / 2.0f) + 0.5f;
		ret.y_ = 1.0f - ((ret.y_ / 2.0f) + 0.5f);
		return ret;
	}

	Sapphire::Vector3 Camera::ScreenToWorldPoint(const Vector3& screenPos) const
	{
		Ray ray = GetScreenRay(screenPos.x_, screenPos.y_);
		return ray._origin + ray._direction * screenPos.z_;
	}

	float Camera::GetDistance(const Vector3& worldPos) const
	{
		
		if (!orthographic_)
		{
			Vector3 cameraPos = cameraPos = Vector3::ZERO;
			if (m_pNode.NotNull())
			{
				SharedPtr<TransformComponent> transform;
				transform.DynamicCast(m_pNode->GetComponent(ComponentType_Transform));
				cameraPos = transform->GetWorldPosition();
                  
			}
			return (worldPos - cameraPos).Length();
		}
		else
			return MathHelper::Abs((GetView() * worldPos).z_);
	}

	float Camera::GetDistanceSquared(const Vector3& worldPos) const
	{
		if (!orthographic_)
		{
			Vector3 cameraPos = cameraPos = Vector3::ZERO;
			if (m_pNode.NotNull())
			{
				SharedPtr<TransformComponent> transform;
				transform.DynamicCast(m_pNode->GetComponent(ComponentType_Transform));
				cameraPos = transform->GetWorldPosition();

			}
			return (worldPos - cameraPos).LengthSquared();
		}
		else
		{
			float distance = MathHelper::Abs((GetView() * worldPos).z_);
			return distance * distance;
		}
			
	}

	float Camera::GetLodDistance(float distance, float scale, float bias) const
	{
		float d = MAX(lodBias_ * bias * scale * zoom_, M_EPSILON);
		if (!orthographic_)
			return distance / d;
		else
			return orthoSize_ / d;
	}

	Sapphire::Quaternion Camera::GetFaceCameraRotation(const Vector3& position, const Quaternion& rotation, FaceCameraMode mode)
	{
		SharedPtr<TransformComponent> transform;
		if (m_pNode.NotNull())
		{
			transform.DynamicCast(m_pNode->GetComponent(ComponentType_Transform));
		}
		else
		{
			return rotation;
		}
	
		switch (mode)
		{
		default:
			return rotation;

		case FC_ROTATE_XYZ:
			return transform->GetWorldRotation();

		case FC_ROTATE_Y:
		{
			Vector3 euler = rotation.EulerAngles();
			euler.y_ = transform->GetWorldRotation().EulerAngles().y_;
			return Quaternion(euler.x_, euler.y_, euler.z_);
		}

		case FC_LOOKAT_XYZ:
		{
			Quaternion lookAt;
			lookAt.FromLookRotation(position - transform->GetWorldPosition());
			return lookAt;
		}

		case FC_LOOKAT_Y:
		{
			// Make the Y-only lookat happen on an XZ plane to make sure there are no unwanted transitions
			// or singularities
			Vector3 lookAtVec(position - transform->GetWorldPosition());
			lookAtVec.y_ = 0.0f;

			Quaternion lookAt;
			lookAt.FromLookRotation(lookAtVec);

			Vector3 euler = rotation.EulerAngles();
			euler.y_ = lookAt.EulerAngles().y_;
			return Quaternion(euler.x_, euler.y_, euler.z_);
		}
		}
	}

	Sapphire::Matrix3x4 Camera::GetEffectiveWorldTransform() const
	{
		if (m_pNode.NotNull())
		{
			SharedPtr<TransformComponent> transform;
			transform.DynamicCast(m_pNode->GetComponent(ComponentType_Transform));
			if (transform.NotNull())
			{
				Matrix3x4 worldTransform = Matrix3x4(transform->GetWorldPosition(), transform->GetWorldRotation(), 1.0f);
				return useReflection_ ? reflectionMatrix_ * worldTransform : worldTransform;
			}
		}
		Matrix3x4 worldTransform = Matrix3x4::IDENTITY;
		return useReflection_ ? reflectionMatrix_ * worldTransform : worldTransform;
		
	}

	bool Camera::IsProjectionValid() const
	{

	}

	void Camera::SetAspectRatioInternal(float aspectRatio)
	{

	}

	void Camera::SetReflectionPlaneAttr(const Vector4& value)
	{

	}

	Sapphire::Vector4 Camera::GetReflectionPlaneAttr() const
	{

	}

	void Camera::SetClipPlaneAttr(const Vector4& value)
	{

	}

	Sapphire::Vector4 Camera::GetClipPlaneAttr() const
	{

	}

}

