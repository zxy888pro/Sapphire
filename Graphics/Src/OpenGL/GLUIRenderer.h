#pragma once


#include "Sapphire.h"
#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "VertexBuffer.h"
#include "UI/UIBatch.h"
#include "IRenderer.h"


namespace Sapphire
{

	//OpenGL的UI渲染器
	class SAPPHIRE_CLASS GLUIRenderer : public IRenderer
	{
		SAPPHIRE_OBJECT(IRenderer, GLUIRenderer)

	public:

		GLUIRenderer(Core* pcore, GLGraphicDriver* pDriver);
		virtual ~GLUIRenderer();

		void Initialize();

		virtual void Clear();

		//设置顶点数据到顶点缓冲区中
		virtual void SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData);

		//逻辑更新
		virtual void Update(float timeStep);

		//更新准备渲染
		virtual void RenderUpdate();


		virtual void Render(bool resetRenderTargets = true);



		//virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL) override;

	protected:

		void		GetBatches(UIElment* element, IntRect curScissor);

		void		Render(bool resetRenderTargets, VertexBuffer* buffer, const std::vector<UIBatch>& batches, unsigned batchStart, unsigned batchEnd);

	private:

		void		OnBeginFrame(ushort eEventType, ushort eEvent, void* eventData);
		void		OnRenderUpdate(ushort eEventType, ushort eEvent, void* eventData);
		void		OnPostUpdate(ushort eEventType, ushort eEvent, void* eventData);

	private:

		WeakPtr<IGraphicDriver> m_pGraphicDriver;


		std::vector<UIBatch>			m_batches;      //要渲染的UI批次
		std::vector<float>				m_vertexData;   //顶点数据
		
		SharedPtr<VertexBuffer>			m_vertexBuffer; //顶点缓冲区
		std::vector<UIElment*>			m_elementArray;  //用于查询的UI元素数组

		bool							m_bIsRenderered;    //是否已经渲染的标志
		bool							m_bIsInitilaized;   //是否初始化

		uint							m_batcheSize;      //批次大小
		SharedPtr<UIElment>				m_rootElement;

	};


}