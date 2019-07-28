#include "UI/UIElement.h"
#include "ITexture.h"

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
		return m_position;
	}

	void UIElment::SetScreenPos(IntVector2 pos)
	{
		m_position = pos;
	}

	const Sapphire::Color& UIElment::GetDerivedColor() const
	{
		return Color::WHITE; 
	}

	bool UIElment::HasColorGradient() const
	{
		return false;
	}

	int UIElment::GetHeight() const
	{
		return m_size.y_;
	}

	int UIElment::GetWidth() const
	{
		return m_size.x_;
	}

	const Sapphire::IntVector2& UIElment::GetSize() const
	{
		return m_size;
	}

	void UIElment::SetName(const std::string& name)
	{
		m_name = name;
	}

	void UIElment::SetPosition(const IntVector2& position)
	{
		m_position = position;
	}

	void UIElment::SetPosition(int x, int y)
	{
		m_position = IntVector2(x, y);
	}

	void UIElment::SetSize(const IntVector2& size)
	{
		m_size = size;
	}

	void UIElment::SetSize(int width, int height)
	{
		SetSize(IntVector2(width, height));
	}

	void UIElment::SetWidth(int width)
	{
		SetSize(IntVector2(width, m_size.y_));
	}

	void UIElment::SetHeight(int height)
	{
		SetSize(IntVector2(m_size.x_, height));
	}

	void UIElment::Update(float timestep)
	{

	}

	void UIElment::GetBatches(std::vector<UIBatch>& batches, std::vector<float>& vertexData, const IntRect& currentScissor)
	{
		bool allOpaque = true;
		UIBatch batch(this, m_blendMode == BLEND_REPLACE && !allOpaque ? BLEND_ALPHA : m_blendMode, currentScissor, NULL, &vertexData);
		batch.AddQuad(0, 0, m_size.x_, m_size.y_, 0, 0, 0, 0);

		batch.AddOrMerge(batches);
			
	}

}