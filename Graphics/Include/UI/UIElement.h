#pragma once


#include "Graphics.h"
#include "Math/Vector2.h"


namespace Sapphire
{

	class SAPPHIRE_CLASS UIElment : public BaseObject
	{
		SAPPHIRE_OBJECT(UIElment, BaseObject)

	public:

		UIElment(Core* pCore);
		virtual ~UIElment();

		const IntVector2&		GetScreenPos() const;

		void					SetScreenPos(IntVector2 pos);

		const Color&			GetDerivedColor() const;

		bool					HasColorGradient() const;


	protected:


		IntVector2				m_screenPos;

	};


}