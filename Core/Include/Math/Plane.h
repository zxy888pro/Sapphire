#pragma once

#include "Sapphire.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix4x4.h"

namespace Sapphire
{

	class SAPPHIRE_CLASS Plane
	{
	public: 
		Plane() :
			_d(0.0f)
		{
		}

		Plane(const Plane& plane) :
			_normal(plane._normal),
			_absNormal(plane._absNormal),
			_d(plane._d)
		{
		}

		Plane(const Vector3& v0, const Vector3& v1, const Vector3& v2)
		{
			Define(v0, v1, v2);
		}

		Plane(const Vector3& normal, const Vector3& point)
		{
			Define(normal, point);
		}

		Plane(const Vector4& plane)
		{
			Define(plane);
		}

		void Define(const Vector3& v0, const Vector3& v1, const Vector3& v2)
		{
			Vector3 dist1 = v1 - v0;
			Vector3 dist2 = v2 - v0;

			Define(dist1.CrossProduct(dist2), v0);
		}

		void Define(const Vector3& normal, const Vector3& point)
		{
			_normal = normal.Normalized();
			_absNormal = _normal.Abs();
			_d = -_normal.DotProduct(point);
		}

		void Define(const Vector4& plane)
		{
			_normal = Vector3(plane.x_, plane.y_, plane.z_);
			_absNormal = _normal.Abs();
			_d = plane.w_;
		}

		Plane& operator =(const Plane& rhs)
		{
			_normal = rhs._normal;
			_absNormal = rhs._absNormal;
			_d = rhs._d;
			return *this;
		}

		
		void Transform(const Matrix3x3& transform);
		 
		void Transform(const Matrix3x4& transform);
		 
		void Transform(const Matrix4x4& transform);

		Vector3 Project(const Vector3& point) const;

		float Distance(const Vector3& point) const;

		Vector3 Reflect(const Vector3& direction) const;

		 
		Matrix3x4 ReflectionMatrix() const;
		 
		Plane Transformed(const Matrix3x3& transform) const;
		 
		Plane Transformed(const Matrix3x4& transform) const;
		 
		Plane Transformed(const Matrix4x4& transform) const;

		 
		Vector4 ToVector4() const { return Vector4(_normal, _d); }

		
		Vector3 _normal;
		
		Vector3 _absNormal;
		 
		float _d; //平面与坐标原点的偏移量

		/// 在原点朝up方向的的平面
		static const Plane UP;

	};


}