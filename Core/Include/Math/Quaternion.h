#pragma once


#include <SapphireDef.h>
#include "Vector3.h"

namespace Sapphire
{
	class Matrix3x3;
	class Vector4;

	class SAPPHIRE_CLASS Quaternion
	{
	public:

		Quaternion():
			w_(1.0f),
			x_(0.0f),
			y_(0.0f),
			z_(0.0f)
		{

		}
		virtual ~Quaternion();


		Quaternion(const Quaternion& quat)
			:w_(quat.w_),
			x_(quat.x_),
			y_(quat.y_),
			z_(quat.z_)
		{
		}

		Quaternion(float w, float x, float y, float z):
			w_(w),
			x_(x),
			y_(y),
			z_(z)
		{

		}

		explicit Quaternion(const float* data) :
			w_(data[0]),
			x_(data[1]),
			y_(data[2]),
			z_(data[3])
		{

		}

		explicit Quaternion(float angle)
		{
			FromAngleAxis(angle, Vector3::FORWARD);
		}

		Quaternion(float x, float y, float z)
		{
			FromEulerAngles(x, y, z);
		}
		Quaternion(const Vector3& start, const Vector3& end)
		{
			FromRotationTo(start, end);
		}

		Quaternion(float angle, const Vector3& axis)
		{
			FromAngleAxis(angle, axis);
		}
		Quaternion(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
		{
			FromAxes(xAxis, yAxis, zAxis);
		}

		explicit Quaternion(const Matrix3x3& matrix)
		{
			FromRotationMatrix(matrix);
		}

		Quaternion& operator +=(const Quaternion& rhs)
		{
			w_ += rhs.w_;
			x_ += rhs.x_;
			y_ += rhs.y_;
			z_ += rhs.z_;
			return *this;
		}

		Quaternion& operator *=(float rhs)
		{
			w_ *= rhs;
			x_ *= rhs;
			y_ *= rhs;
			z_ *= rhs;
			return *this;
		}

		bool operator ==(const Quaternion& rhs) const
		{
			return w_ == rhs.w_ && x_ == rhs.x_ && y_ == rhs.y_ && z_ == rhs.z_;
		}

		bool operator !=(const Quaternion& rhs) const { return !(*this == rhs); }

		Quaternion operator *(float rhs) const
		{
			return Quaternion(w_ * rhs, x_ * rhs, y_ * rhs, z_ * rhs);
		}

		Quaternion operator -() const
		{
			return Quaternion(-w_, -x_, -y_, -z_);
		}

		Quaternion operator +(const Quaternion& rhs) const
		{
			return Quaternion(w_ + rhs.w_, x_ + rhs.x_, y_ + rhs.y_, z_ + rhs.z_);
		}

		Quaternion operator -(const Quaternion& rhs) const
		{
			return Quaternion(w_ - rhs.w_, x_ - rhs.x_, y_ - rhs.y_, z_ - rhs.z_);
		}

		Quaternion operator *(const Quaternion& rhs) const
		{
			return Quaternion(
				w_ * rhs.w_ - x_ * rhs.x_ - y_ * rhs.y_ - z_ * rhs.z_,
				w_ * rhs.x_ + x_ * rhs.w_ + y_ * rhs.z_ - z_ * rhs.y_,
				w_ * rhs.y_ + y_ * rhs.w_ + z_ * rhs.x_ - x_ * rhs.z_,
				w_ * rhs.z_ + z_ * rhs.w_ + x_ * rhs.y_ - y_ * rhs.x_
				);
		}

		Vector3 operator *(const Vector3& rhs) const
		{
			Vector3 qVec(x_, y_, z_);
			Vector3 cross1(qVec.CrossProduct(rhs));
			Vector3 cross2(qVec.CrossProduct(cross1));

			return rhs + 2.0f * (cross1 * w_ + cross2);
		}

		const float* Data() const { return &w_; }

		std::string ToString() const;

		bool IsNaN() const { return MathHelper::IsNaN(w_) || MathHelper::IsNaN(x_) || MathHelper::IsNaN(y_) || MathHelper::IsNaN(z_); }

		void FromAngleAxis(float angle, const Vector3& axis);
		void FromEulerAngles(float x, float y, float z);
		void FromRotationTo(const Vector3& start, const Vector3& end);
		void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
		void FromRotationMatrix(const Matrix3x3& matrix);
		bool FromLookRotation(const Vector3& direction, const Vector3& up = Vector3::UP);
		void Normalize();
		Quaternion Normalized() const;
		Quaternion Inverse() const;
		float LengthSquared() const;
		float DotProduct(const Quaternion& rhs) const;
		bool Equals(const Quaternion& rhs) const;
		Quaternion Conjugate() const;

		Sapphire::Vector3 EulerAngles() const;
		float YawAngle() const;
		float PitchAngle() const;
		float RollAngle() const;

		Matrix3x3 RotationMatrix() const;
		Quaternion Slerp(Quaternion rhs, float t) const;
		Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;


		float w_;
		float x_;
		float y_;
		float z_;

		static const Quaternion IDENTITY;

	private:

	};


}