#include "Math/Plane.h"

namespace Sapphire
{


	void Plane::Transform(const Matrix3x3& transform)
	{
		Define(Matrix4x4(transform).Inverse().Transpose() * ToVector4());
	}

	void Plane::Transform(const Matrix3x4& transform)
	{
		Define(transform.ToMatrix4x4().Inverse().Transpose() * ToVector4());
	}

	void Plane::Transform(const Matrix4x4& transform)
	{
		Define(transform.Inverse().Transpose() * ToVector4());
	}


	Sapphire::Vector3 Plane::Project(const Vector3& point) const
	{
		//������㵽ƽ�����, ���������ʵ�ʳ��ȣ��������������ͶӰ����
		return point - _normal * (_normal.DotProduct(point) + _d);
	}

	float Plane::Distance(const Vector3& point) const
	{
		//�ȼ���ƽ���ԭ�㣬 ����㵽��ԭ��ƽ��ľ�������ƫ��d
		return _normal.DotProduct(point) + _d;
	}

	Sapphire::Vector3 Plane::Reflect(const Vector3& direction) const
	{
		return direction - (2.0f * _normal.DotProduct(direction) * _normal);
	}

	//�������
	Matrix3x4 Plane::ReflectionMatrix() const
	{
		return Matrix3x4(
			-2.0f * _normal.x_ * _normal.x_ + 1.0f,
			-2.0f * _normal.x_ * _normal.y_,
			-2.0f * _normal.x_ * _normal.z_,
			-2.0f * _normal.x_ * _d,
			-2.0f * _normal.y_ * _normal.x_,
			-2.0f * _normal.y_ * _normal.y_ + 1.0f,
			-2.0f * _normal.y_ * _normal.z_,
			-2.0f * _normal.y_ * _d,
			-2.0f * _normal.z_ * _normal.x_,
			-2.0f * _normal.z_ * _normal.y_,
			-2.0f * _normal.z_ * _normal.z_ + 1.0f,
			-2.0f * _normal.z_ * _d
			);
	}

	Plane Plane::Transformed(const Matrix3x3& transform) const
	{
		return Plane(Matrix4x4(transform).Inverse().Transpose() * ToVector4());
	}

	Plane Plane::Transformed(const Matrix3x4& transform) const
	{
		return Plane(transform.ToMatrix4x4().Inverse().Transpose() * ToVector4());
	}

	Plane Plane::Transformed(const Matrix4x4& transform) const
	{
		return Plane(transform.Inverse().Transpose() * ToVector4());
	}

	const Plane Plane::UP(Vector3(0.0f, 1.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));

	
}