#pragma once



#include "Sapphire.h"


namespace Sapphire
{
	class BoundingBox;
	class Frustum;
	class Matrix3;
	class Matrix3x4;
	class Plane;


	class SAPPHIRE_CLASS Polyhedron
	{
	public:

		Polyhedron()
		{

		}

		Polyhedron(const Polyhedron& polyhedron)
		{

		}
		~Polyhedron();

		Polyhedron(const std::vector<std::vector<Vector3>>& faces) :
			faces_(faces)
		{
		}
		Polyhedron(const BoundingBox& box)
		{
			Define(box);
		}
		Polyhedron(const Frustum& frustum)
		{
			Define(frustum);
		}
		Polyhedron& operator =(const Polyhedron& rhs)
		{
			faces_ = rhs.faces_;
			return *this;
		}

		void Define(const BoundingBox& box);
		void Define(const Frustum& frustum);
		/// 添加一个三角形
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2);
		/// 添加一个四边形
		void AddFace(const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);
		/// 添加一个任意多边形
		void AddFace(const std::vector<Vector3>& face);
		/// 通过一个平面裁剪
		void Clip(const Plane& plane);
		/// 通过一个碰撞盒裁剪
		void Clip(const BoundingBox& box);
		/// 通过一个视椎体裁剪
		void Clip(const Frustum& box);
		
		void Clear();
		
		void Transform(const Matrix3x3& transform);
		
		void Transform(const Matrix3x4& transform);

		
		Polyhedron Transformed(const Matrix3x3& transform) const;
		 
		Polyhedron Transformed(const Matrix3x4& transform) const;

		 
		bool Empty() const { return faces_.empty(); }


		std::vector<std::vector<Vector3>> faces_;
	private:

		/// 通过索引设置一个三角形
		void SetFace(unsigned index, const Vector3& v0, const Vector3& v1, const Vector3& v2);
		/// 通过索引设置一个四边形
		void SetFace(unsigned index, const Vector3& v0, const Vector3& v1, const Vector3& v2, const Vector3& v3);

		std::vector<Vector3>   clippedVertices_;
		std::vector<Vector3>   outFace_;
	};

}