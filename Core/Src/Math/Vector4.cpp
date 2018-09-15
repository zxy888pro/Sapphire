#include <Math/Vector4.h>


namespace Sapphire
{


	String Vector4::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g %g %g", x_, y_, z_, w_);
		return String(tempBuffer);
	}

	const Vector4 Vector4::ZERO;
	const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

}