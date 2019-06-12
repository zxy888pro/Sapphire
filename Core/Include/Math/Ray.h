#pragma once

#include "Sapphire.h"
#include "Math/Vector3.h"

namespace Sapphire
{

	class BoundingBox;
	class Frustum;
	class Plane;
	class Sphere;

	class SAPPHIRE_CLASS Ray
	{
	public:
		Ray();

		Ray(const Vector3& origin, const Vector3& direction)
		{
			_origin = origin;
			_direction = direction.Normalized();
		}
		Ray(const Ray& ray) :
			_origin(ray._origin),
			_direction(ray._direction)
		{
		}

		Ray& operator =(const Ray& rhs)
		{
			_origin = rhs._origin;
			_direction = rhs._direction;
			return *this;
		}

		bool operator ==(const Ray& rhs) const { return _origin == rhs._origin && _direction == rhs._direction; }

		bool operator !=(const Ray& rhs) const { return _origin != rhs._origin || _direction != rhs._direction; }

		//投影一个点到这个射线上
		Vector3 Project(const Vector3& point) const
		{
			Vector3 offset = point - _origin; //原点到点的向量
			return _origin + offset.DotProduct(_direction) * _direction;  //V0 + V(t) => _origin + |offset|* |Vproj|/|offset| * _direction
		}
		//点到射线的距离
		float Distance(const Vector3& point) const
		{
			Vector3 projected = Project(point);
			return (point - projected).Length();
		}

		//与另一条射线最接近的点
		Vector3 ClosestPoint(const Ray& ray) const;

		float HitDistance(const Plane& plane) const;

		float HitDistance(const BoundingBox& box) const;


		float HitDistance(const Frustum& frustum, bool solidInside = true) const;


		float HitDistance(const Sphere& sphere) const;

		float HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal = 0, Vector3* outBary = 0) const;

		float HitDistance
			(const void* vertexData, unsigned vertexStride, unsigned vertexStart, unsigned vertexCount, Vector3* outNormal = 0,
			Vector2* outUV = 0, unsigned uvOffset = 0) const;


		float HitDistance(const void* vertexData, unsigned vertexStride, const void* indexData, unsigned indexSize, unsigned indexStart,
			unsigned indexCount, Vector3* outNormal = 0, Vector2* outUV = 0, unsigned uvOffset = 0) const;

		bool InsideGeometry(const void* vertexData, unsigned vertexSize, unsigned vertexStart, unsigned vertexCount) const;

		bool InsideGeometry(const void* vertexData, unsigned vertexSize, const void* indexData, unsigned indexSize, unsigned indexStart,
			unsigned indexCount) const;


		Ray Transformed(const Matrix3x4& transform) const;

		Vector3 _origin;
		Vector3 _direction;
	};

}