#pragma once
#include <SapphireDef.h>
#include "Vector4.h"
#include "Vector3.h"
#include "Matrix3x3.h"

namespace Sapphire
{
	class Quaternion;
	class Matrix3x4;

	class SAPPHIRE_CLASS Matrix4x4
	{
	public:

		Matrix4x4() :
			m00_(1.0f),
			m01_(0.0f),
			m02_(0.0f),
			m03_(0.0f),
			m10_(0.0f),
			m11_(1.0f),
			m12_(0.0f),
			m13_(0.0f),
			m20_(0.0f),
			m21_(0.0f),
			m22_(1.0f),
			m23_(0.0f),
			m30_(0.0f),
			m31_(0.0f),
			m32_(0.0f),
			m33_(1.0f)
		{

		}

		Matrix4x4(const Matrix4x4& matrix) :
			m00_(matrix.m00_),
			m01_(matrix.m01_),
			m02_(matrix.m02_),
			m03_(matrix.m03_),
			m10_(matrix.m10_),
			m11_(matrix.m11_),
			m12_(matrix.m12_),
			m13_(matrix.m13_),
			m20_(matrix.m20_),
			m21_(matrix.m21_),
			m22_(matrix.m22_),
			m23_(matrix.m23_),
			m30_(matrix.m30_),
			m31_(matrix.m31_),
			m32_(matrix.m32_),
			m33_(matrix.m33_)
		{

		}

		Matrix4x4(const Matrix3x3& matrix) :
			m00_(matrix.m00_),
			m01_(matrix.m01_),
			m02_(matrix.m02_),
			m03_(0.0f),
			m10_(matrix.m10_),
			m11_(matrix.m11_),
			m12_(matrix.m12_),
			m13_(0.0f),
			m20_(matrix.m20_),
			m21_(matrix.m21_),
			m22_(matrix.m22_),
			m23_(0.0f),
			m30_(0.0f),
			m31_(0.0f),
			m32_(0.0f),
			m33_(1.0f)
		{

		}

		Matrix4x4(float v00, float v01, float v02, float v03,
			float v10, float v11, float v12, float v13,
			float v20, float v21, float v22, float v23,
			float v30, float v31, float v32, float v33) :
			m00_(v00),
			m01_(v01),
			m02_(v02),
			m03_(v03),
			m10_(v10),
			m11_(v11),
			m12_(v12),
			m13_(v13),
			m20_(v20),
			m21_(v21),
			m22_(v22),
			m23_(v23),
			m30_(v30),
			m31_(v31),
			m32_(v32),
			m33_(v33)
		{
		}


		explicit Matrix4x4(const float* data) 
			:m00_(data[0]),
			m01_(data[1]),
			m02_(data[2]),
			m03_(data[3]),
			m10_(data[4]),
			m11_(data[5]),
			m12_(data[6]),
			m13_(data[7]),
			m20_(data[8]),
			m21_(data[9]),
			m22_(data[10]),
			m23_(data[11]),
			m30_(data[12]),
			m31_(data[13]),
			m32_(data[14]),
			m33_(data[15])
		{

		}

		Matrix4x4& operator =(const Matrix4x4& rhs)
		{
			m00_ = rhs.m00_;
			m01_ = rhs.m01_;
			m02_ = rhs.m02_;
			m03_ = rhs.m03_;
			m10_ = rhs.m10_;
			m11_ = rhs.m11_;
			m12_ = rhs.m12_;
			m13_ = rhs.m13_;
			m20_ = rhs.m20_;
			m21_ = rhs.m21_;
			m22_ = rhs.m22_;
			m23_ = rhs.m23_;
			m30_ = rhs.m30_;
			m31_ = rhs.m31_;
			m32_ = rhs.m32_;
			m33_ = rhs.m33_;
			return *this;
		}

		Matrix4x4& operator =(const Matrix3x3& rhs)
		{
			m00_ = rhs.m00_;
			m01_ = rhs.m01_;
			m02_ = rhs.m02_;
			m03_ = 0.0f;
			m10_ = rhs.m10_;
			m11_ = rhs.m11_;
			m12_ = rhs.m12_;
			m13_ = 0.0f;
			m20_ = rhs.m20_;
			m21_ = rhs.m21_;
			m22_ = rhs.m22_;
			m23_ = 0.0f;
			m30_ = 0.0f;
			m31_ = 0.0f;
			m32_ = 0.0f;
			m33_ = 1.0f;
			return *this;
		}

		bool operator ==(const Matrix4x4& rhs) const
		{
			const float* leftData = Data();
			const float* rightData = rhs.Data();

			for (unsigned i = 0; i < 16; ++i)
			{
				if (leftData[i] != rightData[i])
					return false;
			}

			return true;	
		}

		bool operator !=(const Matrix4x4& rhs) const { return !(*this == rhs); }

		Vector3 operator *(const Vector3& rhs) const
		{
			float invW = 1.0f / (m30_ * rhs.x_ + m31_ * rhs.y_ + m32_ * rhs.z_ + m33_);

			return Vector3(
				(m00_ * rhs.x_ + m01_ * rhs.y_ + m02_ * rhs.z_ + m03_) * invW,
				(m10_ * rhs.x_ + m11_ * rhs.y_ + m12_ * rhs.z_ + m13_) * invW,
				(m20_ * rhs.x_ + m21_ * rhs.y_ + m22_ * rhs.z_ + m23_) * invW
				);
		}

		Vector4 operator *(const Vector4& rhs) const
		{
			return Vector4(
				m00_ * rhs.x_ + m01_ * rhs.y_ + m02_ * rhs.z_ + m03_ * rhs.w_,
				m10_ * rhs.x_ + m11_ * rhs.y_ + m12_ * rhs.z_ + m13_ * rhs.w_,
				m20_ * rhs.x_ + m21_ * rhs.y_ + m22_ * rhs.z_ + m23_ * rhs.w_,
				m30_ * rhs.x_ + m31_ * rhs.y_ + m32_ * rhs.z_ + m33_ * rhs.w_
				);
		}

		Matrix4x4 operator +(const Matrix4x4& rhs) const
		{
			return Matrix4x4(
				m00_ + rhs.m00_,
				m01_ + rhs.m01_,
				m02_ + rhs.m02_,
				m03_ + rhs.m03_,
				m10_ + rhs.m10_,
				m11_ + rhs.m11_,
				m12_ + rhs.m12_,
				m13_ + rhs.m13_,
				m20_ + rhs.m20_,
				m21_ + rhs.m21_,
				m22_ + rhs.m22_,
				m23_ + rhs.m23_,
				m30_ + rhs.m30_,
				m31_ + rhs.m31_,
				m32_ + rhs.m32_,
				m33_ + rhs.m33_
				);
		}

		Matrix4x4 operator -(const Matrix4x4& rhs) const
		{
			return Matrix4x4(
				m00_ - rhs.m00_,
				m01_ - rhs.m01_,
				m02_ - rhs.m02_,
				m03_ - rhs.m03_,
				m10_ - rhs.m10_,
				m11_ - rhs.m11_,
				m12_ - rhs.m12_,
				m13_ - rhs.m13_,
				m20_ - rhs.m20_,
				m21_ - rhs.m21_,
				m22_ - rhs.m22_,
				m23_ - rhs.m23_,
				m30_ - rhs.m30_,
				m31_ - rhs.m31_,
				m32_ - rhs.m32_,
				m33_ - rhs.m33_
				);
		}

		Matrix4x4 operator *(float rhs) const
		{
			return Matrix4x4(
				m00_ * rhs,
				m01_ * rhs,
				m02_ * rhs,
				m03_ * rhs,
				m10_ * rhs,
				m11_ * rhs,
				m12_ * rhs,
				m13_ * rhs,
				m20_ * rhs,
				m21_ * rhs,
				m22_ * rhs,
				m23_ * rhs,
				m30_ * rhs,
				m31_ * rhs,
				m32_ * rhs,
				m33_ * rhs
				);
		}

		Matrix4x4 operator *(const Matrix4x4& rhs) const
		{
			return Matrix4x4(
				m00_ * rhs.m00_ + m01_ * rhs.m10_ + m02_ * rhs.m20_ + m03_ * rhs.m30_,
				m00_ * rhs.m01_ + m01_ * rhs.m11_ + m02_ * rhs.m21_ + m03_ * rhs.m31_,
				m00_ * rhs.m02_ + m01_ * rhs.m12_ + m02_ * rhs.m22_ + m03_ * rhs.m32_,
				m00_ * rhs.m03_ + m01_ * rhs.m13_ + m02_ * rhs.m23_ + m03_ * rhs.m33_,
				m10_ * rhs.m00_ + m11_ * rhs.m10_ + m12_ * rhs.m20_ + m13_ * rhs.m30_,
				m10_ * rhs.m01_ + m11_ * rhs.m11_ + m12_ * rhs.m21_ + m13_ * rhs.m31_,
				m10_ * rhs.m02_ + m11_ * rhs.m12_ + m12_ * rhs.m22_ + m13_ * rhs.m32_,
				m10_ * rhs.m03_ + m11_ * rhs.m13_ + m12_ * rhs.m23_ + m13_ * rhs.m33_,
				m20_ * rhs.m00_ + m21_ * rhs.m10_ + m22_ * rhs.m20_ + m23_ * rhs.m30_,
				m20_ * rhs.m01_ + m21_ * rhs.m11_ + m22_ * rhs.m21_ + m23_ * rhs.m31_,
				m20_ * rhs.m02_ + m21_ * rhs.m12_ + m22_ * rhs.m22_ + m23_ * rhs.m32_,
				m20_ * rhs.m03_ + m21_ * rhs.m13_ + m22_ * rhs.m23_ + m23_ * rhs.m33_,
				m30_ * rhs.m00_ + m31_ * rhs.m10_ + m32_ * rhs.m20_ + m33_ * rhs.m30_,
				m30_ * rhs.m01_ + m31_ * rhs.m11_ + m32_ * rhs.m21_ + m33_ * rhs.m31_,
				m30_ * rhs.m02_ + m31_ * rhs.m12_ + m32_ * rhs.m22_ + m33_ * rhs.m32_,
				m30_ * rhs.m03_ + m31_ * rhs.m13_ + m32_ * rhs.m23_ + m33_ * rhs.m33_
				);
		}

		Matrix4x4 operator *(const Matrix3x4& rhs) const;

		void SetTranslation(const Vector3& translation)
		{
			m03_ = translation.x_;
			m13_ = translation.y_;
			m23_ = translation.z_;
		}

		void SetRotation(const Matrix3x3& rotation)
		{
			m00_ = rotation.m00_;
			m01_ = rotation.m01_;
			m02_ = rotation.m02_;
			m10_ = rotation.m10_;
			m11_ = rotation.m11_;
			m12_ = rotation.m12_;
			m20_ = rotation.m20_;
			m21_ = rotation.m21_;
			m22_ = rotation.m22_;
		}

		void SetScale(float scale)
		{
			m00_ = scale;
			m11_ = scale;
			m22_ = scale;
		}

		Matrix3x3 ToMatrix3x3() const
		{
			return Matrix3x3(
				m00_,
				m01_,
				m02_,
				m10_,
				m11_,
				m12_,
				m20_,
				m21_,
				m22_
				);
		}

		Matrix3x3 RotationMatrix() const
		{
			Vector3 invScale(
				1.0f / sqrtf(m00_ * m00_ + m10_ * m10_ + m20_ * m20_),
				1.0f / sqrtf(m01_ * m01_ + m11_ * m11_ + m21_ * m21_),
				1.0f / sqrtf(m02_ * m02_ + m12_ * m12_ + m22_ * m22_)
				);

			return ToMatrix3x3().Scaled(invScale);
		}


		Vector3 GetTranslation() const
		{
			return Vector3(
				m03_,
				m13_,
				m23_
				);
		}

		Quaternion GetRotation() const;

		Vector3 GetScale() const
		{
			return Vector3(
				sqrtf(m00_ * m00_ + m10_ * m10_ + m20_ * m20_),
				sqrtf(m01_ * m01_ + m11_ * m11_ + m21_ * m21_),
				sqrtf(m02_ * m02_ + m12_ * m12_ + m22_ * m22_)
				);
		}

		Matrix4x4 Transpose() const
		{
			return Matrix4x4(
				m00_,
				m10_,
				m20_,
				m30_,
				m01_,
				m11_,
				m21_,
				m31_,
				m02_,
				m12_,
				m22_,
				m32_,
				m03_,
				m13_,
				m23_,
				m33_
				);
		}

		bool Equals(const Matrix4x4& rhs) const
		{
			const float* leftData = Data();
			const float* rightData = rhs.Data();

			for (unsigned i = 0; i < 16; ++i)
			{
				if (!MathHelper::Equals(leftData[i], rightData[i]))
					return false;
			}

			return true;
		}

		void Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const;

		Matrix4x4 Inverse() const;

		const float* Data() const { return &m00_; }

		String ToString() const;

		float m00_;
		float m01_;
		float m02_;
		float m03_;
		float m10_;
		float m11_;
		float m12_;
		float m13_;
		float m20_;
		float m21_;
		float m22_;
		float m23_;
		float m30_;
		float m31_;
		float m32_;
		float m33_;

		static const Matrix4x4 ZERO;
		static const Matrix4x4 IDENTITY;
	};
}