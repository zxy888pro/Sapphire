#include "UI/UIElement.h"

namespace Sapphire
{

	UIElment::UIElment(Core* pCore):
		BaseObject(pCore)
	{

	}

	UIElment::~UIElment()
	{

	}

	const Sapphire::IntVector2& UIElment::GetScreenPos() const
	{
		return m_screenPos;
	}

	void UIElment::SetScreenPos(IntVector2 pos)
	{
		m_screenPos = pos;
	}

	const Sapphire::Color& UIElment::GetDerivedColor() const
	{
		return Color::WHITE; 
	}

	bool UIElment::HasColorGradient() const
	{
		return false;
	}

}