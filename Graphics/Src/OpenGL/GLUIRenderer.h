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

		//���ö������ݵ����㻺������
		void SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData);

		//�߼�����
		void Update(float timeStep);

		//����׼����Ⱦ
		void RenderUpdate();


		virtual void Render();


	protected:


	private:

		WeakPtr<GLGraphicDriver> m_pGraphicDriver;

		std::vector<UIBatch>			m_batches;      //Ҫ��Ⱦ��UI����
		std::vector<float>				m_vertexData;   //��������
		
		SharedPtr<VertexBuffer>			m_vertexBuffer; //���㻺����
		std::vector<UIElment*>			m_elementArray;  //���ڲ�ѯ��UIԪ������

		bool							m_bIsRenderered;    //�Ƿ��Ѿ���Ⱦ�ı�־
		bool							m_bIsInitilaized;   //�Ƿ��ʼ��

	};


}