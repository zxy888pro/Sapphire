#pragma once


#include "Sapphire.h"
#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "VertexBuffer.h"
#include "UI/UIBatch.h"



namespace Sapphire
{


	class SAPPHIRE_CLASS GLUIRenderer : public BaseObject
	{
		SAPPHIRE_OBJECT(GLUIRenderer, BaseObject)

	public:

		GLUIRenderer(Core* pcore);
		virtual ~GLUIRenderer();


		void Clear();

		//设置顶点数据到顶点缓冲区中
		void SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData);

		//逻辑更新
		void Update(float timeStep);

		//更新准备渲染
		void RenderUpdate();


		virtual void Render();


	protected:


	private:

		WeakPtr<GLGraphicDriver> m_pGraphicDriver;

		std::vector<UIBatch>			m_batches;      //要渲染的UI批次
		std::vector<float>				m_vertexData;   //顶点数据
		
		SharedPtr<VertexBuffer>			m_vertexBuffer; //顶点缓冲区
		std::vector<UIElment*>			m_elementArray;  //用于查询的UI元素数组

		bool							m_bIsRenderered;    //是否已经渲染的标志
		bool							m_bIsInitilaized;   //是否初始化

	};


}