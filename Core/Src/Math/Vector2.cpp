#include "Math/Vector2.h"

namespace Sapphire
{

 

	std::string Vector2::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g", x_, y_);
		return std::string(tempBuffer);
	}

	const Sapphire::Vector2 Vector2::ZERO;

	const Sapphire::Vector2 Vector2::LEFT(-1.0f, 0.0f);;

	const Sapphire::Vector2 Vector2::RIGHT(1.0f, 0.0f);

	const Sapphire::Vector2 Vector2::UP(0.0f, 1.0f);

	const Sapphire::Vector2 Vector2::DOWN(0.0f, -1.0f);

	const Sapphire::Vector2 Vector2::ONE(1.0f, 1.0f);

	std::string IntVector2::ToString() const
	{
		char tempBuffer[STRING_BUFFERSIZE];
		sprintf(tempBuffer, "%g %g", x_, y_);
		return std::string(tempBuffer);
	}

	const Sapphire::IntVector2 IntVector2::ZERO;

}