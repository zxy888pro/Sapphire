#pragma once
#include <Color.h>
#include <mathHelper.h>

namespace Sapphire
{

	void Color::Bounds(float* min, float* max, bool clipped /*= false*/) const
	{
		m_assert(min && max);

		if (r_ > g_)
		{
			if (g_ > b_) // r > g > b
			{
				*max = r_;
				*min = b_;
			}
			else // r > g && g <= b
			{
				*max = r_ > b_ ? r_ : b_;
				*min = g_;
			}
		}
		else
		{
			if (b_ > g_) // r <= g < b
			{
				*max = b_;
				*min = r_;
			}
			else // r <= g && b <= g
			{
				*max = g_;
				*min = r_ < b_ ? r_ : b_;
			}
		}

		if (clipped)
		{
			*max = *max > 1.0f ? 1.0f : (*max < 0.0f ? 0.0f : *max);
			*min = *min > 1.0f ? 1.0f : (*min < 0.0f ? 0.0f : *min);
		}
	}

	void Color::Clip(bool clipAlpha /*= false*/)
	{
		r_ = (r_ > 1.0f) ? 1.0f : ((r_ < 0.0f) ? 0.0f : r_);
		g_ = (g_ > 1.0f) ? 1.0f : ((g_ < 0.0f) ? 0.0f : g_);
		b_ = (b_ > 1.0f) ? 1.0f : ((b_ < 0.0f) ? 0.0f : b_);

		if (clipAlpha)
			a_ = (a_ > 1.0f) ? 1.0f : ((a_ < 0.0f) ? 0.0f : a_);
	}

	void Color::Invert(bool invertAlpha /*= false*/)
	{
		r_ = 1.0f - r_;
		g_ = 1.0f - g_;
		b_ = 1.0f - b_;

		if (invertAlpha)
			a_ = 1.0f - a_;
	}

	Color Color::Lerp(const Color& rhs, float t) const
	{
		float invT = 1.0f - t;
		return Color(
			r_ * invT + rhs.r_ * t,
			g_ * invT + rhs.g_ * t,
			b_ * invT + rhs.b_ * t,
			a_ * invT + rhs.a_ * t
			);
	}

	unsigned Color::ToUInt() const
	{
		unsigned r = (unsigned)MathHelper::Clamp(((int)(r_ * 255.0f)), 0, 255);
		unsigned g = (unsigned)MathHelper::Clamp(((int)(g_ * 255.0f)), 0, 255);
		unsigned b = (unsigned)MathHelper::Clamp(((int)(b_ * 255.0f)), 0, 255);
		unsigned a = (unsigned)MathHelper::Clamp(((int)(a_ * 255.0f)), 0, 255);
		return (a << 24) | (b << 16) | (g << 8) | r;
	}

	bool Color::Equals(const Color& rhs) const
	{
		return MathHelper::Equals(r_, rhs.r_) && MathHelper::Equals(g_, rhs.g_) && MathHelper::Equals(b_, rhs.b_) && MathHelper::Equals(a_, rhs.a_);
	}

	std::string Color::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g %g %g", r_, g_, b_, a_);
		return std::string(tempBuffer);
	}

	const Color Color::WHITE;
	const Color Color::GRAY(0.5f, 0.5f, 0.5f);
	const Color Color::BLACK(0.0f, 0.0f, 0.0f);
	const Color Color::RED(1.0f, 0.0f, 0.0f);
	const Color Color::GREEN(0.0f, 1.0f, 0.0f);
	const Color Color::BLUE(0.0f, 0.0f, 1.0f);
	const Color Color::CYAN(0.0f, 1.0f, 1.0f);
	const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
	const Color Color::YELLOW(1.0f, 1.0f, 0.0f);
	//const Color Color::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);



	Color operator*(float lhs, const Color& rhs)
	{
		return rhs * lhs;
	}

}