#include "Math/Ray.h"
#include "Math/Plane.h"
#include "Math/BoundingBox.h"
#include "Math/Sphere.h"
#include "Math/Frustum.h"
#include "mathHelper.h"

namespace Sapphire
{

	Sapphire::Vector3 Ray::ClosestPoint(const Ray& ray) const
	{
		Vector3 p13 = _origin - ray._origin;
		Vector3 p43 = ray._direction;
		Vector3 p21 = _direction;

		float d1343 = p13.DotProduct(p43);
		float d4321 = p43.DotProduct(p21);
		float d1321 = p13.DotProduct(p21);
		float d4343 = p43.DotProduct(p43);
		float d2121 = p21.DotProduct(p21);

		float d = d2121 * d4343 - d4321 * d4321;
		if (MathHelper::Abs(d) < M_EPSILON)
			return _origin;
		float n = d1343 * d4321 - d1321 * d4343;
		float a = n / d;

		return _origin + a * _direction;
	}


	float Ray::HitDistance(const Plane& plane) const
	{
		float d = plane._normal.DotProduct(_direction);
		if (MathHelper::Abs(d) >= M_EPSILON)
		{
			float t = -(plane._normal.DotProduct(_origin) + plane._d) / d;
			if (t >= 0.0f)
				return t;
			else
				return M_INFINITY;
		}
		else
			return M_INFINITY;
	}

	float Ray::HitDistance(const BoundingBox& box) const
	{
		// If undefined, no hit (infinite distance)
		if (!box.Defined())
			return M_INFINITY;

		// Check for ray origin being inside the box
		if (box.IsInside(_origin))
			return 0.0f;

		float dist = M_INFINITY;

		// Check for intersecting in the X-direction
		if (_origin.x_ < box.min_.x_ && _direction.x_ > 0.0f)
		{
			float x = (box.min_.x_ - _origin.x_) / _direction.x_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.y_ >= box.min_.y_ && point.y_ <= box.max_.y_ && point.z_ >= box.min_.z_ && point.z_ <= box.max_.z_)
					dist = x;
			}
		}
		if (_origin.x_ > box.max_.x_ && _direction.x_ < 0.0f)
		{
			float x = (box.max_.x_ - _origin.x_) / _direction.x_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.y_ >= box.min_.y_ && point.y_ <= box.max_.y_ && point.z_ >= box.min_.z_ && point.z_ <= box.max_.z_)
					dist = x;
			}
		}
		// Check for intersecting in the Y-direction
		if (_origin.y_ < box.min_.y_ && _direction.y_ > 0.0f)
		{
			float x = (box.min_.y_ - _origin.y_) / _direction.y_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.x_ >= box.min_.x_ && point.x_ <= box.max_.x_ && point.z_ >= box.min_.z_ && point.z_ <= box.max_.z_)
					dist = x;
			}
		}
		if (_origin.y_ > box.max_.y_ && _direction.y_ < 0.0f)
		{
			float x = (box.max_.y_ - _origin.y_) / _direction.y_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.x_ >= box.min_.x_ && point.x_ <= box.max_.x_ && point.z_ >= box.min_.z_ && point.z_ <= box.max_.z_)
					dist = x;
			}
		}
		// Check for intersecting in the Z-direction
		if (_origin.z_ < box.min_.z_ && _direction.z_ > 0.0f)
		{
			float x = (box.min_.z_ - _origin.z_) / _direction.z_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.x_ >= box.min_.x_ && point.x_ <= box.max_.x_ && point.y_ >= box.min_.y_ && point.y_ <= box.max_.y_)
					dist = x;
			}
		}
		if (_origin.z_ > box.max_.z_ && _direction.z_ < 0.0f)
		{
			float x = (box.max_.z_ - _origin.z_) / _direction.z_;
			if (x < dist)
			{
				Vector3 point = _origin + x * _direction;
				if (point.x_ >= box.min_.x_ && point.x_ <= box.max_.x_ && point.y_ >= box.min_.y_ && point.y_ <= box.max_.y_)
					dist = x;
			}
		}

		return dist;
	}

	float Ray::HitDistance(const Frustum& frustum, bool solidInside) const
	{
		float maxOutside = 0.0f;
		float minInside = M_INFINITY;
		bool allInside = true;

		for (unsigned i = 0; i < NUM_FRUSTUM_PLANES; ++i)
		{
			const Plane& plane = frustum.planes_[i];
			float distance = HitDistance(frustum.planes_[i]);

			if (plane.Distance(_origin) < 0.0f)
			{
				maxOutside = MAX(maxOutside, distance);
				allInside = false;
			}
			else
				minInside = MIN(minInside, distance);
		}

		if (allInside)
			return solidInside ? 0.0f : minInside;
		else if (maxOutside <= minInside)
			return maxOutside;
		else
			return M_INFINITY;
	}

	float Ray::HitDistance(const Sphere& sphere) const
	{
		Vector3 centeredOrigin = _origin - sphere.center_;
		float squaredRadius = sphere.radius_ * sphere.radius_;

		// Check if ray originates inside the sphere
		if (centeredOrigin.LengthSquared() <= squaredRadius)
			return 0.0f;

		// Calculate intersection by quadratic equation
		float a = _direction.DotProduct(_direction);
		float b = 2.0f * centeredOrigin.DotProduct(_direction);
		float c = centeredOrigin.DotProduct(centeredOrigin) - squaredRadius;
		float d = b * b - 4.0f * a * c;

		// No solution
		if (d < 0.0f)
			return M_INFINITY;

		// Get the nearer solution
		float dSqrt = sqrtf(d);
		float dist = (-b - dSqrt) / (2.0f * a);
		if (dist >= 0.0f)
			return dist;
		else
			return (-b + dSqrt) / (2.0f * a);
	}

	float Ray::HitDistance(const Vector3& v0, const Vector3& v1, const Vector3& v2, Vector3* outNormal, Vector3* outBary) const
	{
		// Based on Fast, Minimum Storage Ray/Triangle Intersection by Möller & Trumbore
		// http://www.graphics.cornell.edu/pubs/1997/MT97.pdf
		// Calculate edge vectors
		Vector3 edge1(v1 - v0);
		Vector3 edge2(v2 - v0);

		// Calculate determinant & check backfacing
		Vector3 p(_direction.CrossProduct(edge2));
		float det = edge1.DotProduct(p);
		if (det >= M_EPSILON)
		{
			// Calculate u & v parameters and test
			Vector3 t(_origin - v0);
			float u = t.DotProduct(p);
			if (u >= 0.0f && u <= det)
			{
				Vector3 q(t.CrossProduct(edge1));
				float v = _direction.DotProduct(q);
				if (v >= 0.0f && u + v <= det)
				{
					float distance = edge2.DotProduct(q) / det;
					// Discard hits behind the ray
					if (distance >= 0.0f)
					{
						// There is an intersection, so calculate distance & optional normal
						if (outNormal)
							*outNormal = edge1.CrossProduct(edge2);
						if (outBary)
							*outBary = Vector3(1 - (u / det) - (v / det), u / det, v / det);

						return distance;
					}
				}
			}
		}

		return M_INFINITY;
	}

	float Ray::HitDistance(const void* vertexData, unsigned vertexStride, unsigned vertexStart, unsigned vertexCount,
		Vector3* outNormal, Vector2* outUV, unsigned uvOffset) const
	{
		float nearest = M_INFINITY;
		const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * vertexStride;
		unsigned index = 0, nearestIdx = M_MAX_UNSIGNED;
		Vector3 barycentric;
		Vector3* outBary = outUV ? &barycentric : 0;

		while (index + 2 < vertexCount)
		{
			const Vector3& v0 = *((const Vector3*)(&vertices[index * vertexStride]));
			const Vector3& v1 = *((const Vector3*)(&vertices[(index + 1) * vertexStride]));
			const Vector3& v2 = *((const Vector3*)(&vertices[(index + 2) * vertexStride]));
			float distance = HitDistance(v0, v1, v2, outNormal, outBary);
			if (distance < nearest)
			{
				nearestIdx = index;
				nearest = distance;
			}
			index += 3;
		}

		if (outUV)
		{
			if (nearestIdx == M_MAX_UNSIGNED)
				*outUV = Vector2::ZERO;
			else
			{
				// Interpolate the UV coordinate using barycentric coordinate
				const Vector2& uv0 = *((const Vector2*)(&vertices[uvOffset + nearestIdx * vertexStride]));
				const Vector2& uv1 = *((const Vector2*)(&vertices[uvOffset + (nearestIdx + 1) * vertexStride]));
				const Vector2& uv2 = *((const Vector2*)(&vertices[uvOffset + (nearestIdx + 2) * vertexStride]));
				*outUV = Vector2(uv0.x_ * barycentric.x_ + uv1.x_ * barycentric.y_ + uv2.x_ * barycentric.z_,
					uv0.y_ * barycentric.x_ + uv1.y_ * barycentric.y_ + uv2.y_ * barycentric.z_);
			}
		}

		return nearest;
	}

	float Ray::HitDistance(const void* vertexData, unsigned vertexStride, const void* indexData, unsigned indexSize,
		unsigned indexStart, unsigned indexCount, Vector3* outNormal, Vector2* outUV, unsigned uvOffset) const
	{
		float nearest = M_INFINITY;
		const unsigned char* vertices = (const unsigned char*)vertexData;
		Vector3 barycentric;
		Vector3* outBary = outUV ? &barycentric : 0;

		// 16-bit indices
		if (indexSize == sizeof(unsigned short))
		{
			const unsigned short* indices = ((const unsigned short*)indexData) + indexStart;
			const unsigned short* indicesEnd = indices + indexCount;
			const unsigned short* nearestIndices = 0;

			while (indices < indicesEnd)
			{
				const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * vertexStride]));
				const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * vertexStride]));
				const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * vertexStride]));
				float distance = HitDistance(v0, v1, v2, outNormal, outBary);
				if (distance < nearest)
				{
					nearestIndices = indices;
					nearest = distance;
				}
				indices += 3;
			}

			if (outUV)
			{
				if (nearestIndices == 0)
					*outUV = Vector2::ZERO;
				else
				{
					// Interpolate the UV coordinate using barycentric coordinate
					const Vector2& uv0 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[0] * vertexStride]));
					const Vector2& uv1 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[1] * vertexStride]));
					const Vector2& uv2 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[2] * vertexStride]));
					*outUV = Vector2(uv0.x_ * barycentric.x_ + uv1.x_ * barycentric.y_ + uv2.x_ * barycentric.z_,
						uv0.y_ * barycentric.x_ + uv1.y_ * barycentric.y_ + uv2.y_ * barycentric.z_);
				}
			}
		}
		// 32-bit indices
		else
		{
			const unsigned* indices = ((const unsigned*)indexData) + indexStart;
			const unsigned* indicesEnd = indices + indexCount;
			const unsigned* nearestIndices = 0;

			while (indices < indicesEnd)
			{
				const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * vertexStride]));
				const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * vertexStride]));
				const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * vertexStride]));
				float distance = HitDistance(v0, v1, v2, outNormal, outBary);
				if (distance < nearest)
				{
					nearestIndices = indices;
					nearest = distance;
				}
				indices += 3;
			}

			if (outUV)
			{
				if (nearestIndices == 0)
					*outUV = Vector2::ZERO;
				else
				{
					// Interpolate the UV coordinate using barycentric coordinate
					const Vector2& uv0 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[0] * vertexStride]));
					const Vector2& uv1 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[1] * vertexStride]));
					const Vector2& uv2 = *((const Vector2*)(&vertices[uvOffset + nearestIndices[2] * vertexStride]));
					*outUV = Vector2(uv0.x_ * barycentric.x_ + uv1.x_ * barycentric.y_ + uv2.x_ * barycentric.z_,
						uv0.y_ * barycentric.x_ + uv1.y_ * barycentric.y_ + uv2.y_ * barycentric.z_);
				}
			}
		}

		return nearest;
	}

	bool Ray::InsideGeometry(const void* vertexData, unsigned vertexSize, unsigned vertexStart, unsigned vertexCount) const
	{
		float currentFrontFace = M_INFINITY;
		float currentBackFace = M_INFINITY;
		const unsigned char* vertices = ((const unsigned char*)vertexData) + vertexStart * vertexSize;
		unsigned index = 0;

		while (index + 2 < vertexCount)
		{
			const Vector3& v0 = *((const Vector3*)(&vertices[index * vertexSize]));
			const Vector3& v1 = *((const Vector3*)(&vertices[(index + 1) * vertexSize]));
			const Vector3& v2 = *((const Vector3*)(&vertices[(index + 2) * vertexSize]));
			float frontFaceDistance = HitDistance(v0, v1, v2);
			float backFaceDistance = HitDistance(v2, v1, v0);
			currentFrontFace = MIN(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
			// A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
			// checking reversed frontfaces
			currentBackFace = MIN(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
			index += 3;
		}

		// If the closest face is a backface, that means that the ray originates from the inside of the geometry
		// NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
		// (ray doesnt hit either) by this conditional
		if (currentFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
			return currentBackFace < currentFrontFace;

		// It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
		// As such, it is safe to assume they are not
		return false;
	}

	bool Ray::InsideGeometry(const void* vertexData, unsigned vertexSize, const void* indexData, unsigned indexSize,
		unsigned indexStart, unsigned indexCount) const
	{
		float currentFrontFace = M_INFINITY;
		float currentBackFace = M_INFINITY;
		const unsigned char* vertices = (const unsigned char*)vertexData;

		// 16-bit indices
		if (indexSize == sizeof(unsigned short))
		{
			const unsigned short* indices = ((const unsigned short*)indexData) + indexStart;
			const unsigned short* indicesEnd = indices + indexCount;

			while (indices < indicesEnd)
			{
				const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * vertexSize]));
				const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * vertexSize]));
				const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * vertexSize]));
				float frontFaceDistance = HitDistance(v0, v1, v2);
				float backFaceDistance = HitDistance(v2, v1, v0);
				currentFrontFace = MIN(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
				// A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
				// checking reversed frontfaces
				currentBackFace = MIN(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
				indices += 3;
			}
		}
		// 32-bit indices
		else
		{
			const unsigned* indices = ((const unsigned*)indexData) + indexStart;
			const unsigned* indicesEnd = indices + indexCount;

			while (indices < indicesEnd)
			{
				const Vector3& v0 = *((const Vector3*)(&vertices[indices[0] * vertexSize]));
				const Vector3& v1 = *((const Vector3*)(&vertices[indices[1] * vertexSize]));
				const Vector3& v2 = *((const Vector3*)(&vertices[indices[2] * vertexSize]));
				float frontFaceDistance = HitDistance(v0, v1, v2);
				float backFaceDistance = HitDistance(v2, v1, v0);
				currentFrontFace = MIN(frontFaceDistance > 0.0f ? frontFaceDistance : M_INFINITY, currentFrontFace);
				// A backwards face is just a regular one, with the vertices in the opposite order. This essentially checks backfaces by
				// checking reversed frontfaces
				currentBackFace = MIN(backFaceDistance > 0.0f ? backFaceDistance : M_INFINITY, currentBackFace);
				indices += 3;
			}
		}

		// If the closest face is a backface, that means that the ray originates from the inside of the geometry
		// NOTE: there may be cases where both are equal, as in, no collision to either. This is prevented in the most likely case
		// (ray doesn't hit either) by this conditional
		if (currentFrontFace != M_INFINITY || currentBackFace != M_INFINITY)
			return currentBackFace < currentFrontFace;

		// It is still possible for two triangles to be equally distant from the triangle, however, this is extremely unlikely.
		// As such, it is safe to assume they are not
		return false;
	}

	Ray Ray::Transformed(const Matrix3x4& transform) const
	{
		Ray ret;
		ret._origin = transform * _origin;
		ret._direction = transform * Vector4(_direction, 0.0f);
		return ret;
	}

}