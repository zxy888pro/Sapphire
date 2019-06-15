
#include "Math/Frustum.h"
#include "Math/Sphere.h"
#include "Math/BoundingBox.h"

namespace Sapphire
{

	Intersection Frustum::IsInside(const Vector3& point) const
	{
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			if (planes_[i].Distance(point) < 0.0f)
				return OUTSIDE;
		}

		return INSIDE;
	}

	/// Test if a sphere is inside, outside or intersects.
	Intersection Frustum::IsInside(const Sphere& sphere) const
	{
		bool allInside = true;
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			float dist = planes_[i].Distance(sphere.center_);
			if (dist < -sphere.radius_)
				return OUTSIDE;
			else if (dist < sphere.radius_)
				allInside = false;
		}

		return allInside ? INSIDE : INTERSECTS;
	}

	/// Test if a sphere if (partially) inside or outside.
	Intersection Frustum::IsInsideFast(const Sphere& sphere) const
	{
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			if (planes_[i].Distance(sphere.center_) < -sphere.radius_)
				return OUTSIDE;
		}

		return INSIDE;
	}

	/// Test if a bounding box is inside, outside or intersects.
	Intersection Frustum::IsInside(const BoundingBox& box) const
	{
		Vector3 center = box.Center();
		Vector3 edge = center - box.min_;
		bool allInside = true;

		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			const Plane& plane = planes_[i];
			float dist = plane._normal.DotProduct(center) + plane._d;
			float absDist = plane._absNormal.DotProduct(edge);

			if (dist < -absDist)
				return OUTSIDE;
			else if (dist < absDist)
				allInside = false;
		}

		return allInside ? INSIDE : INTERSECTS;
	}

	/// Test if a bounding box is (partially) inside or outside.
	Intersection Frustum::IsInsideFast(const BoundingBox& box) const
	{
		Vector3 center = box.Center();
		Vector3 edge = center - box.min_;

		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			const Plane& plane = planes_[i];
			float dist = plane._normal.DotProduct(center) + plane._d;
			float absDist = plane._absNormal.DotProduct(edge);

			if (dist < -absDist)
				return OUTSIDE;
		}

		return INSIDE;
	}

	/// Return distance of a point to the frustum, or 0 if inside.
	float Frustum::Distance(const Vector3& point) const
	{
		float distance = 0.0f;
		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
			distance = MAX(-planes_[i].Distance(point), distance);

		return distance;
	}
}