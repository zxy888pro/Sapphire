#pragma once


#include "Graphics.h"
#include "Color.h"
#include "Math/Vector2.h"
#include "UIBatch.h"


namespace Sapphire
{
	struct ITexture;
	
	///UI四个边角定义
	enum Corner
	{
		C_TOPLEFT = 0,
		C_TOPRIGHT,
		C_BOTTOMLEFT,
		C_BOTTOMRIGHT,
		MAX_UIELEMENT_CORNERS
	};

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

		int						GetHeight() const;

		int						GetWidth() const;

		const IntVector2&		GetSize() const;


		void					SetName(const std::string& name);

		void					SetPosition(const IntVector2& position);

		void					SetPosition(int x, int y);

		void					SetSize(const IntVector2& size);

		void					SetSize(int width, int height);

		void					SetWidth(int width);
	
		void					SetHeight(int height);

		virtual void			Update(float timestep);

		//将本UI元素的批次和顶点数据添加到batches中
		virtual void GetBatches(std::vector<UIBatch>& batches, std::vector<float>& vertexData, const IntRect& currentScissor);

	protected:


		IntVector2				m_position;  //位置
		IntVector2				m_size;      //大小

		std::string				m_name;		//名字

		bool					m_bEnabled;  //是否开启 
		bool					m_bVisible;	 //是否显示
		BlendMode				m_blendMode;
		ITexture*				m_pTexture;

		Color					m_color[MAX_UIELEMENT_CORNERS];

	};



}