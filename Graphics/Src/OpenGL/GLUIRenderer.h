#pragma once


#include "Sapphire.h"
#include "Graphics.h"
#include "GLGraphicDriver.h"
#include "VertexBuffer.h"
#include "UI/UIBatch.h"
#include "IRenderer.h"


namespace Sapphire
{

	//OpenGL��UI��Ⱦ��
	class SAPPHIRE_CLASS GLUIRenderer : public IRenderer
	{
		SAPPHIRE_OBJECT(IRenderer, GLUIRenderer)

	public:

		GLUIRenderer(Core* pcore, GLGraphicDriver* pDriver);
		virtual ~GLUIRenderer();

		void Initialize();

		virtual void Clear();

		//���ö������ݵ����㻺������
		virtual void SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData);

		//�߼�����
		virtual void Update(float timeStep);

		//����׼����Ⱦ
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


		std::vector<UIBatch>			m_batches;      //Ҫ��Ⱦ��UI����
		std::vector<float>				m_vertexData;   //��������
		
		SharedPtr<VertexBuffer>			m_vertexBuffer; //���㻺����
		std::vector<UIElment*>			m_elementArray;  //���ڲ�ѯ��UIԪ������

		bool							m_bIsRenderered;    //�Ƿ��Ѿ���Ⱦ�ı�־
		bool							m_bIsInitilaized;   //�Ƿ��ʼ��

		uint							m_batcheSize;      //���δ�С
		SharedPtr<UIElment>				m_rootElement;

	};


}