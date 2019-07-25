#pragma once
#include "Graphics.h"
#include "Math/Rect.h"




namespace Sapphire
{
	struct ITexture;
	class  UIElment;
	 
	static const unsigned UI_VERTEX_SIZE = 6;  //UI�����С


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


		static Vector3			      posAdjust;     //λ�õ���

	private:

		UIElment*			  m_element;
		BlendMode			  m_blendMode;
		ITexture*			  m_pTexture;
		Vector2				  m_invTexSize;     //���س���ĵ���
		std::vector<float>*   m_vertexData; //����Ķ�������ָ��
		uint				  m_vertexStart;
		uint				  m_vertexEnd;
		Color				  m_color;    //��ǰ��ɫ
		IntRect				  m_scissor; //��������
		bool				  m_bUseGradient;   //�ݶ���ɫ
		
		

	};


}



