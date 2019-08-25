#pragma once
#include "Sapphire.h"
#include "GraphicsDefs.h"


namespace Sapphire
{

	class VertexBuffer;

	class SAPPHIRE_CLASS IRenderer : public BaseObject
	{

		SAPPHIRE_OBJECT(IRenderer, BaseObject)

	public:

		IRenderer(Core* pcore) :BaseObject(pcore)
		{

		}

		virtual void Clear() = 0;

		//���ö������ݵ����㻺������
		virtual void SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData) = 0;

		//�߼�����
		virtual void Update(float timeStep) = 0;

		//����׼����Ⱦ
		virtual void RenderUpdate() = 0;


		virtual void Render(bool resetRenderTargets = true) = 0;

	protected:



	};
}