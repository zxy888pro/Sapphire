#pragma once


#include "Sapphire.h"
#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "VertexBuffer.h"



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

		std::vector<float>      m_vertexData;   //顶点数据
		
		SharedPtr<VertexBuffer> m_vertexBuffer; //顶点缓冲区

	};


}