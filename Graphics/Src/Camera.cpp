#pragma once

#include "Camera.h"
#include "Node.h"
#include "TransformComponent.h"


namespace Sapphire
{

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
		//观察投影矩阵的逆矩阵
		Matrix4x4 viewProjInverse = (GetProjection(false) * GetView()).Inverse();

		// 参数范围是从0.0 - 1.0. 扩展到标准设备坐标系(-1.0 to 1.0) 并且翻转y轴
		x = 2.0f * x - 1.0f;
		y = 1.0f - 2.0f * y;
		Vector3 nearPoint(x, y, 0.0f);
		Vector3 farPoint(x, y, 1.0f);
		ret._origin = viewProjInverse * nearPoint;
		ret._direction = ((viewProjInverse * farPoint) - ret._origin).Normalized();
		return ret;
		return ret;
	}

}

