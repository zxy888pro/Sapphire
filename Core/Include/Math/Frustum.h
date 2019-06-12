#pragma once

#include "Sapphire.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix3x3.h"
#include "Math/Matrix3x4.h"
#include "Math/Matrix4x4.h"
#include "Math/Plane.h"


namespace Sapphire
{
	class BoundingBox;
	class Sphere;

	static const unsigned NUM_FRUSTUM_PLANES = 6;
	static const unsigned NUM_FRUSTUM_VERTICES = 8;

	class SAPPHIRE_CLASS Frustum
	{
	public:
		Frustum();
		Frustum(const Frustum& frustum);

		Frustum& operator =(const Frustum& rhs);

		void
			Define(float fov, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform = Matrix3x4::IDENTITY);
		/// Define with near and far dimension vectors and a transform matrix.
		void Define(const Vector3& near, const Vector3& far, const Matrix3x4& transform = Matrix3x4::IDENTITY);
		/// Define with a bounding box and a transform matrix.
		void Define(const BoundingBox& box, const Matrix3x4& transform = Matrix3x4::IDENTITY);
		/// Define with orthographic projection parameters and a transform matrix.
		void DefineOrtho
			(float orthoSize, float aspectRatio, float zoom, float nearZ, float farZ, const Matrix3x4& transform = Matrix3x4::IDENTITY);
		/// Transform by a 3x3 matrix.
		void Transform(const Matrix3x3& transform);
		/// Transform by a 3x4 matrix.
		void Transform(const Matrix3x4& transform);

		/// Test if a point is inside or outside.
		Intersection IsInside(const Vector3& point) const;

		Intersection IsInside(const Sphere& sphere) const;

		Intersection IsInsideFast(const Sphere& sphere) const;

		Intersection IsInside(const BoundingBox& box) const;

		Intersection IsInsideFast(const BoundingBox& box) const;

		float Distance(const Vector3& point) const;



		/// Return transformed by a 3x3 matrix.
		Frustum Transformed(const Matrix3x3& transform) const;
		/// Return transformed by a 3x4 matrix.
		Frustum Transformed(const Matrix3x4& transform) const;
		/// Return projected by a 4x4 projection matrix.
		Rect Projected(const Matrix4x4& transform) const;

		/// Update the planes. Called internally.
		void UpdatePlanes();

		/// Frustum planes.
		Plane planes_[NUM_FRUSTUM_PLANES];
		/// Frustum vertices.
		Vector3 vertices_[NUM_FRUSTUM_VERTICES];

	};

}