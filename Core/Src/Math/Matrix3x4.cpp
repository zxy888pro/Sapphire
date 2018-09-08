#include <Math/Matrix3x4.h>
#include <Math/Quaternion.h>

namespace Sapphire
{

	Matrix3x4::Matrix3x4() :
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
		m23_(0.0f)
	{
		   
	}

	Matrix3x4::~Matrix3x4()
	{

	}

	Sapphire::Matrix3x4 Matrix3x4::Inverse() const
	{
		float det = m00_ * m11_ * m22_ +
			m10_ * m21_ * m02_ +
			m20_ * m01_ * m12_ -
			m20_ * m11_ * m02_ -
			m10_ * m01_ * m22_ -
			m00_ * m21_ * m12_;

		float invDet = 1.0f / det;
		Matrix3x4 ret;

		ret.m00_ = (m11_ * m22_ - m21_ * m12_) * invDet;
		ret.m01_ = -(m01_ * m22_ - m21_ * m02_) * invDet;
		ret.m02_ = (m01_ * m12_ - m11_ * m02_) * invDet;
		ret.m03_ = -(m03_ * ret.m00_ + m13_ * ret.m01_ + m23_ * ret.m02_);
		ret.m10_ = -(m10_ * m22_ - m20_ * m12_) * invDet;
		ret.m11_ = (m00_ * m22_ - m20_ * m02_) * invDet;
		ret.m12_ = -(m00_ * m12_ - m10_ * m02_) * invDet;
		ret.m13_ = -(m03_ * ret.m10_ + m13_ * ret.m11_ + m23_ * ret.m12_);
		ret.m20_ = (m10_ * m21_ - m20_ * m11_) * invDet;
		ret.m21_ = -(m00_ * m21_ - m20_ * m01_) * invDet;
		ret.m22_ = (m00_ * m11_ - m10_ * m01_) * invDet;
		ret.m23_ = -(m03_ * ret.m20_ + m13_ * ret.m21_ + m23_ * ret.m22_);

		return ret;
	}

	Sapphire::Quaternion Matrix3x4::GetRotation() const
	{
		{ return Quaternion(RotationMatrix()); }
	}

	void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
	{
		translation.x_ = m03_;
		translation.y_ = m13_;
		translation.z_ = m23_;

		scale.x_ = sqrtf(m00_ * m00_ + m10_ * m10_ + m20_ * m20_);
		scale.y_ = sqrtf(m01_ * m01_ + m11_ * m11_ + m21_ * m21_);
		scale.z_ = sqrtf(m02_ * m02_ + m12_ * m12_ + m22_ * m22_);

		Vector3 invScale(1.0f / scale.x_, 1.0f / scale.y_, 1.0f / scale.z_);
		rotation = Quaternion(ToMatrix3x3().Scaled(invScale));
	}

	std::string Matrix3x4::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g", m00_, m01_, m02_, m03_, m10_, m11_, m12_, m13_, m20_, m21_, m22_,
			m23_);
		return std::string(tempBuffer);
	}

	const Sapphire::Matrix3x4 Matrix3x4::ZERO(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	const Sapphire::Matrix3x4 Matrix3x4::IDENTITY;

}