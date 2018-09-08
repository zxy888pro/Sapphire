#include <Math/Vector4.h>


namespace Sapphire
{


	std::string Vector4::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g %g %g", x_, y_, z_, w_);
		return std::string(tempBuffer);
	}

	const Vector4 Vector4::ZERO;
	const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

}