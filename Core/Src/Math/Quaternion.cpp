#include <Math/Matrix3x3.h>
#include <Math/Vector3.h>
#include <Math/Vector4.h>
#include <Math/Quaternion.h>



namespace Sapphire
{

	Quaternion::~Quaternion()
	{

	}

	String Quaternion::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g %g %g", w_, x_, y_, z_);
		return String(tempBuffer);
	}

	void Quaternion::FromAngleAxis(float angle, const Vector3& axis)
	{
		Vector3 normAxis = axis.Normalized();
		angle = DEG_TO_RAD(angle);
		float sinAngle = sinf(angle);
		float cosAngle = cosf(angle);

		w_ = cosAngle;
		x_ = normAxis.x_ * sinAngle;
		y_ = normAxis.y_ * sinAngle;
		z_ = normAxis.z_ * sinAngle;
	}

	void Quaternion::FromEulerAngles(float x, float y, float z)
	{
		//顺序Z->X->Y
		x = DEG_TO_RAD(x);
		y = DEG_TO_RAD(y);
		z = DEG_TO_RAD(z);
		float sinX = sinf(x);
		float cosX = cosf(x);
		float sinY = sinf(y);
		float cosY = cosf(y);
		float sinZ = sinf(z);
		float cosZ = cosf(z);

		w_ = cosY * cosX * cosZ + sinY * sinX * sinZ;
		x_ = cosY * sinX * cosZ + sinY * cosX * sinZ;
		y_ = sinY * cosX * cosZ - cosY * sinX * sinZ;
		z_ = cosY * cosX * sinZ - sinY * sinX * cosZ;
	}

	void Quaternion::FromRotationTo(const Vector3& start, const Vector3& end)
	{
		Vector3 normStart = start.Normalized();
		Vector3 normEnd = end.Normalized();
		float d = normStart.DotProduct(normEnd);

		if (d > -1.0f + M_EPSILON)
		{
			Vector3 c = normStart.CrossProduct(normEnd);
			float s = sqrtf((1.0f + d) * 2.0f);
			float invS = 1.0f / s;

			x_ = c.x_ * invS;
			y_ = c.y_ * invS;
			z_ = c.z_ * invS;
			w_ = 0.5f * s;
		}
		else
		{
			Vector3 axis = Vector3::RIGHT.CrossProduct(normStart);
			if (axis.Length() < M_EPSILON)
				axis = Vector3::UP.CrossProduct(normStart);

			FromAngleAxis(180.f, axis);
		}
	}

	void Quaternion::FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
	{
		Matrix3x3 matrix(
			xAxis.x_, yAxis.x_, zAxis.x_,
			xAxis.y_, yAxis.y_, zAxis.y_,
			xAxis.z_, yAxis.z_, zAxis.z_
			);

		FromRotationMatrix(matrix);
	}

	void Quaternion::FromRotationMatrix(const Matrix3x3& matrix)
	{
		float t = matrix.m00_ + matrix.m11_ + matrix.m22_;

		if (t > 0.0f)
		{
			float invS = 0.5f / sqrtf(1.0f + t);

			x_ = (matrix.m21_ - matrix.m12_) * invS;
			y_ = (matrix.m02_ - matrix.m20_) * invS;
			z_ = (matrix.m10_ - matrix.m01_) * invS;
			w_ = 0.25f / invS;
		}
		else
		{
			if (matrix.m00_ > matrix.m11_ && matrix.m00_ > matrix.m22_)
			{
				float invS = 0.5f / sqrtf(1.0f + matrix.m00_ - matrix.m11_ - matrix.m22_);

				x_ = 0.25f / invS;
				y_ = (matrix.m01_ + matrix.m10_) * invS;
				z_ = (matrix.m20_ + matrix.m02_) * invS;
				w_ = (matrix.m21_ - matrix.m12_) * invS;
			}
			else if (matrix.m11_ > matrix.m22_)
			{
				float invS = 0.5f / sqrtf(1.0f + matrix.m11_ - matrix.m00_ - matrix.m22_);

				x_ = (matrix.m01_ + matrix.m10_) * invS;
				y_ = 0.25f / invS;
				z_ = (matrix.m12_ + matrix.m21_) * invS;
				w_ = (matrix.m02_ - matrix.m20_) * invS;
			}
			else
			{
				float invS = 0.5f / sqrtf(1.0f + matrix.m22_ - matrix.m00_ - matrix.m11_);

				x_ = (matrix.m02_ + matrix.m20_) * invS;
				y_ = (matrix.m12_ + matrix.m21_) * invS;
				z_ = 0.25f / invS;
				w_ = (matrix.m10_ - matrix.m01_) * invS;
			}
		}
	}

	bool Quaternion::FromLookRotation(const Vector3& direction, const Vector3& up /*= Vector3::UP*/)
	{
		Quaternion ret;
		Vector3 forward = direction.Normalized();

		Vector3 v = forward.CrossProduct(up);
		// 如果directionhe 和up 平行， 叉积将是0， 返回用FromRotationTo()
		if (v.LengthSquared() >= M_EPSILON)
		{
			v.Normalize();
			Vector3 up = v.CrossProduct(forward);
			Vector3 right = up.CrossProduct(forward);
			ret.FromAxes(right, up, forward);
		}
		else
			ret.FromRotationTo(Vector3::FORWARD, forward);

		if (!ret.IsNaN())
		{
			(*this) = ret;
			return true;
		}
		else
			return false;
	}

	void Quaternion::Normalize()
	{
		float lenSquared = LengthSquared();
		if (!MathHelper::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			w_ *= invLen;
			x_ *= invLen;
			y_ *= invLen;
			z_ *= invLen;
		}
	}

	Sapphire::Quaternion Quaternion::Normalized() const
	{
		float lenSquared = LengthSquared();
		if (!MathHelper::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
		{
			float invLen = 1.0f / sqrtf(lenSquared);
			return *this * invLen;
		}
		else
			return *this;
	}

	Sapphire::Quaternion Quaternion::Inverse() const
	{
#ifdef SAPPHIRE_SSE
		__m128 q = _mm_loadu_ps(&w_);
		__m128 n = _mm_mul_ps(q, q);
		n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(2, 3, 0, 1)));
		n = _mm_add_ps(n, _mm_shuffle_ps(n, n, _MM_SHUFFLE(0, 1, 2, 3)));
		return Quaternion(_mm_div_ps(_mm_xor_ps(q, _mm_castsi128_ps(_mm_set_epi32((int)0x80000000UL, (int)0x80000000UL, (int)0x80000000UL, 0))), n));
#else
		float lenSquared = LengthSquared();
		if (lenSquared == 1.0f)
			return Conjugate();
		else if (lenSquared >= M_EPSILON)
			return Conjugate() * (1.0f / lenSquared);
		else
			return IDENTITY;
#endif
	}

	float Quaternion::LengthSquared() const
	{
		return w_ * w_ + x_ * x_ + y_ * y_ + z_ * z_;
	}

	float Quaternion::DotProduct(const Quaternion& rhs) const
	{
		return w_ * rhs.w_ + x_ * rhs.x_ + y_ * rhs.y_ + z_ * rhs.z_;
	}

	bool Quaternion::Equals(const Quaternion& rhs) const
	{
		return MathHelper::Equals(w_, rhs.w_) && MathHelper::Equals(x_, rhs.x_) && MathHelper::Equals(y_, rhs.y_) && MathHelper::Equals(z_, rhs.z_);
	}

	Sapphire::Quaternion Quaternion::Conjugate() const
	{
		return Quaternion(w_, -x_, -y_, -z_);
	}

	Sapphire::Vector3 Quaternion::EulerAngles() const
	{
		// 见http://www.geometrictools.com/Documentation/EulerAngles.pdf
		// 顺序Z->X->Y
		float check = 2.0f * (-y_ * z_ + w_ * x_);

		if (check < -0.995f)
		{
			return Sapphire::Vector3(
				-90.0f,
				0.0f,
				-RAD_TO_DEG(atan2f(2.0f * (x_ * z_ - w_ * y_), 1.0f - 2.0f * (y_ * y_ + z_ * z_)))
				);
		}
		else if (check > 0.995f)
		{
			return Sapphire::Vector3(
				90.0f,
				0.0f,
				RAD_TO_DEG(atan2f(2.0f * (x_ * z_ - w_ * y_), 1.0f - 2.0f * (y_ * y_ + z_ * z_)))
				);
		}
		else
		{
			return Sapphire::Vector3(
				RAD_TO_DEG(asinf(check)),
				RAD_TO_DEG(atan2f(2.0f * (x_ * z_ + w_ * y_), 1.0f - 2.0f * (x_ * x_ + y_ * y_))),
				RAD_TO_DEG((2.0f * (x_ * y_ + w_ * z_), 1.0f - 2.0f * (x_ * x_ + z_ * z_)))
				);
		}
	}

	float Quaternion::YawAngle() const
	{
		return EulerAngles().y_;
	}

	float Quaternion::PitchAngle() const
	{
		return EulerAngles().x_;
	}

	float Quaternion::RollAngle() const
	{
		return EulerAngles().z_;
	}

	Sapphire::Matrix3x3 Quaternion::RotationMatrix() const
	{
		return Matrix3x3(
			1.0f - 2.0f * y_ * y_ - 2.0f * z_ * z_,
			2.0f * x_ * y_ - 2.0f * w_ * z_,
			2.0f * x_ * z_ + 2.0f * w_ * y_,
			2.0f * x_ * y_ + 2.0f * w_ * z_,
			1.0f - 2.0f * x_ * x_ - 2.0f * z_ * z_,
			2.0f * y_ * z_ - 2.0f * w_ * x_,
			2.0f * x_ * z_ - 2.0f * w_ * y_,
			2.0f * y_ * z_ + 2.0f * w_ * x_,
			1.0f - 2.0f * x_ * x_ - 2.0f * y_ * y_
			);
	}

	Sapphire::Quaternion Quaternion::Slerp(Quaternion rhs, float t) const
	{
	
		float cosAngle = DotProduct(rhs);
		//检查最短路径
		if (cosAngle < 0.0f)
		{
			cosAngle = -cosAngle;
			rhs = -rhs;
		}

		float angle = acosf(cosAngle);
		float sinAngle = sinf(angle);
		float t1, t2;

		if (sinAngle > 0.001f)
		{
			float invSinAngle = 1.0f / sinAngle;
			t1 = sinf((1.0f - t) * angle) * invSinAngle;
			t2 = sinf(t * angle) * invSinAngle;
		}
		else
		{
			t1 = 1.0f - t;
			t2 = t;
		}

		return *this * t1 + rhs * t2;
	}

	Sapphire::Quaternion Quaternion::Nlerp(Quaternion rhs, float t, bool shortestPath /*= false*/) const
	{
		Quaternion result;
		float fCos = DotProduct(rhs);
		if (fCos < 0.0f && shortestPath)
			result = (*this) + (((-rhs) - (*this)) * t);
		else
			result = (*this) + ((rhs - (*this)) * t);
		result.Normalize();
		return result;
	}

	const Sapphire::Quaternion Quaternion::IDENTITY;

}