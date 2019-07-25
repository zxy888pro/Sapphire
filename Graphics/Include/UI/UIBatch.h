#pragma once
#include "Graphics.h"
#include "Math/Rect.h"




namespace Sapphire
{
	struct ITexture;
	class  UIElment;
	 
	static const unsigned UI_VERTEX_SIZE = 6;  //UI顶点大小


	class SAPPHIRE_CLASS UIBatch
	{

	public:

		UIBatch();

		UIBatch(UIElment* uiElement, BlendMode blendMode, const IntRect& scissorRect, ITexture* texture, std::vector<float>* vertexData);

		void SetColor(const Color& color, bool overrideAlpha);

		void SetDefaultColor();

		uint GetInterpolatedColor(int x, int y);

		void AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth = 0, int texHeight = 0);

		void AddQuad(int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth, int texHeight, bool tiled);

		void AddQuad(const Matrix3x4& transform,  int x, int y, int width, int height, int texOffsetX, int texOffsetY, int texWidth, int texHeight);


		static Vector3			      posAdjust;     //位置调整

	private:

		UIElment*			  m_element;
		BlendMode			  m_blendMode;
		ITexture*			  m_pTexture;
		Vector2				  m_invTexSize;     //像素长宽的倒数
		std::vector<float>*   m_vertexData; //传入的顶点数据指针
		uint				  m_vertexStart;
		uint				  m_vertexEnd;
		Color				  m_color;    //当前颜色
		IntRect				  m_scissor; //剪裁区域
		bool				  m_bUseGradient;   //梯度颜色
		
		

	};


}



