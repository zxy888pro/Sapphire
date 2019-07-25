#include "Math/Matrix3x4.h"
#include "UI/UIBatch.h"
#include "UI/UIElement.h"
#include "ITexture.h"


namespace Sapphire
{
	Vector3 UIBatch::posAdjust(0.0f, 0.0f, 0.0f);


	UIBatch::UIBatch():
		m_element(NULL),
		m_blendMode(BlendMode::BLEND_REPLACE),
		m_pTexture(NULL),
		m_invTexSize(Vector2::ZERO),
		m_vertexData(NULL),
		m_vertexStart(0),
		m_vertexEnd(0)
	{
		SetDefaultColor();
	}

	 

	UIBatch::UIBatch(UIElment* uiElement, BlendMode blendMode, const IntRect& scissorRect, ITexture* texture, std::vector<float>* vertexData) :
		m_element(uiElement),
		m_blendMode(blendMode),
		m_pTexture(texture),
		m_invTexSize(texture ? Vector2(1.0f / (float)texture->getWidth(), 1.0f / (float)texture->getHeight()) : Vector2::ONE),
		m_scissor(scissorRect),
		m_vertexData(vertexData),
		m_vertexStart(vertexData->size()), //从顶点数据末尾开始添加
		m_vertexEnd(vertexData->size())
	{
		SetDefaultColor();
	}

	void UIBatch::SetColor(const Color& color, bool overrideAlpha)
	{
		if (!m_element)
			overrideAlpha = true;

		m_bUseGradient = false;
		m_color = Color(color.r_, color.g_, color.b_, color.a_);
	}

	void UIBatch::SetDefaultColor()
	{
		if (m_element)
		{
			m_color = m_element->GetDerivedColor();
			m_bUseGradient = m_element->HasColorGradient();
		}
		else
		{
			m_color = Color::WHITE;
			m_bUseGradient = false;
		}
		
	}

	uint UIBatch::GetInterpolatedColor(int x, int y)
	{
		return 0;
	}

	void UIBatch::AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth /*= 0*/, int texHeight /*= 0*/)
	{
		//四个角的颜色
		unsigned topLeftColor, topRightColor, bottomLeftColor, bottomRightColor;
		if (!m_bUseGradient)
		{
			// 如果alpha为0， 不渲染, 不用添加四边形
			if (!(m_color.ToUInt() & 0xff000000))
				return;

			topLeftColor = m_color.ToUInt();
			topRightColor = m_color.ToUInt();
			bottomLeftColor = m_color.ToUInt();
			bottomRightColor = m_color.ToUInt();
		}
		else
		{
			topLeftColor = GetInterpolatedColor(x, y);
			topRightColor = GetInterpolatedColor(x + width, y);
			bottomLeftColor = GetInterpolatedColor(x, y + height);
			bottomRightColor = GetInterpolatedColor(x + width, y + height);
		}

		const IntVector2& screenPos = m_element->GetScreenPos();  //获取UIElment的屏幕位置
		float left = (float)(x + screenPos.x_) - posAdjust.x_;
		float right = left + (float)width;
		float top = (float)(y + screenPos.y_) - posAdjust.x_;
		float bottom = top + (float)height;

		//通过纹理长宽将像素偏移缩到0~1.0范围中
		float leftUV = texOffsetX * m_invTexSize.x_;
		float topUV = texOffsetY * m_invTexSize.y_;
		float rightUV = (texOffsetX + (texWidth ? texWidth : width)) * m_invTexSize.x_;
		float bottomUV = (texOffsetY + (texHeight ? texHeight : height)) * m_invTexSize.y_;

		//扩展vertexData，在末尾添加四变形的空间,2个三角形/6个顶点， 每个顶点6个属性
		unsigned begin = m_vertexData->size();
		m_vertexData->resize(begin + 6 * UI_VERTEX_SIZE);
		float* dest = &(m_vertexData->at(begin));
		m_vertexEnd = m_vertexData->size();

		//为每个顶点设置数据
		dest[0] = left;
		dest[1] = top;
		dest[2] = 0.0f;
		((unsigned&)dest[3]) = topLeftColor;
		dest[4] = leftUV;
		dest[5] = topUV;

		dest[6] = right;
		dest[7] = top;
		dest[8] = 0.0f;
		((unsigned&)dest[9]) = topRightColor;
		dest[10] = rightUV;
		dest[11] = topUV;

		dest[12] = left;
		dest[13] = bottom;
		dest[14] = 0.0f;
		((unsigned&)dest[15]) = bottomLeftColor;
		dest[16] = leftUV;
		dest[17] = bottomUV;

		dest[18] = right;
		dest[19] = top;
		dest[20] = 0.0f;
		((unsigned&)dest[21]) = topRightColor;
		dest[22] = rightUV;
		dest[23] = topUV;

		dest[24] = right;
		dest[25] = bottom;
		dest[26] = 0.0f;
		((unsigned&)dest[27]) = bottomRightColor;
		dest[28] = rightUV;
		dest[29] = bottomUV;

		dest[30] = left;
		dest[31] = bottom;
		dest[32] = 0.0f;
		((unsigned&)dest[33]) = bottomLeftColor;
		dest[34] = leftUV;
		dest[35] = bottomUV;

	}

	void UIBatch::AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth, int texHeight, bool tiled)
	{
		if (!(m_element->HasColorGradient() || m_element->GetDerivedColor().ToUInt() & 0xff000000))
			return;  

		if (!tiled)
		{
			AddQuad(x, y, width, height, texOffsetX, texOffsetY, texWidth, texHeight);
			return;
		}

		int tileX = 0;
		int tileY = 0;
		int tileW = 0;
		int tileH = 0;

		while (tileY < height)
		{
			tileX = 0;
			tileH = MIN(height - tileY, texHeight);

			while (tileX < width)
			{
				tileW = MIN(width - tileX, texWidth);

				AddQuad(x + tileX, y + tileY, tileW, tileH, texOffsetX, texOffsetY, tileW, tileH);

				tileX += tileW;
			}

			tileY += tileH;
		}
	}

	void UIBatch::AddQuad(const Matrix3x4& transform, int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth, int texHeight)
	{
		//四个角的颜色
		unsigned topLeftColor, topRightColor, bottomLeftColor, bottomRightColor;
		if (!m_bUseGradient)
		{
			// 如果alpha为0， 不渲染, 不用添加四边形
			if (!(m_color.ToUInt() & 0xff000000))
				return;

			topLeftColor = m_color.ToUInt();
			topRightColor = m_color.ToUInt();
			bottomLeftColor = m_color.ToUInt();
			bottomRightColor = m_color.ToUInt();
		}
		else
		{
			topLeftColor = GetInterpolatedColor(x, y);
			topRightColor = GetInterpolatedColor(x + width, y);
			bottomLeftColor = GetInterpolatedColor(x, y + height);
			bottomRightColor = GetInterpolatedColor(x + width, y + height);
		}

		Vector3 v1 = (transform * Vector3((float)x, (float)y, 0.0f)) - posAdjust;
		Vector3 v2 = (transform * Vector3((float)x + (float)width, (float)y, 0.0f)) - posAdjust;
		Vector3 v3 = (transform * Vector3((float)x, (float)y + (float)height, 0.0f)) - posAdjust;
		Vector3 v4 = (transform * Vector3((float)x + (float)width, (float)y + (float)height, 0.0f)) - posAdjust;


		//通过纹理长宽将像素偏移缩到0~1.0范围中
		float leftUV = texOffsetX * m_invTexSize.x_;
		float topUV = texOffsetY * m_invTexSize.y_;
		float rightUV = (texOffsetX + (texWidth ? texWidth : width)) * m_invTexSize.x_;
		float bottomUV = (texOffsetY + (texHeight ? texHeight : height)) * m_invTexSize.y_;

		//扩展vertexData，在末尾添加四变形的空间,2个三角形/6个顶点， 每个顶点6个属性
		unsigned begin = m_vertexData->size();
		m_vertexData->resize(begin + 6 * UI_VERTEX_SIZE);
		float* dest = &(m_vertexData->at(begin));
		m_vertexEnd = m_vertexData->size();

		//为每个顶点设置数据
		dest[0] = v1.x_;
		dest[1] = v1.y_;
		dest[2] = 0.0f;
		((unsigned&)dest[3]) = topLeftColor;
		dest[4] = leftUV;
		dest[5] = topUV;

		dest[6] = v2.x_;
		dest[7] = v2.y_;
		dest[8] = 0.0f;
		((unsigned&)dest[9]) = topRightColor;
		dest[10] = rightUV;
		dest[11] = topUV;

		dest[12] = v3.x_;
		dest[13] = v3.y_;
		dest[14] = 0.0f;
		((unsigned&)dest[15]) = bottomLeftColor;
		dest[16] = leftUV;
		dest[17] = bottomUV;

		dest[18] = v2.x_;
		dest[19] = v2.y_;
		dest[20] = 0.0f;
		((unsigned&)dest[21]) = topRightColor;
		dest[22] = rightUV;
		dest[23] = topUV;

		dest[24] = v4.x_;
		dest[25] = v4.y_;
		dest[26] = 0.0f;
		((unsigned&)dest[27]) = bottomRightColor;
		dest[28] = rightUV;
		dest[29] = bottomUV;

		dest[30] = v3.x_;
		dest[31] = v3.y_;
		dest[32] = 0.0f;
		((unsigned&)dest[33]) = bottomLeftColor;
		dest[34] = leftUV;
		dest[35] = bottomUV;
	}

}