#include "GLUIRenderer.h"


namespace Sapphire
{

	GLUIRenderer::GLUIRenderer(Core* pcore) :BaseObject(pcore)
	{

	}

	GLUIRenderer::~GLUIRenderer()
	{

	}

	void GLUIRenderer::Clear()
	{

	}

	void GLUIRenderer::SetVertexData(VertexBuffer* dest, const std::vector<float>& vertexData)
	{

	}

	void GLUIRenderer::Update(float timeStep)
	{

	}

	void GLUIRenderer::RenderUpdate()
	{
		m_bIsRenderered = false;

		m_batches.clear();
		m_vertexData.clear();


	}

	void GLUIRenderer::Render()
	{

	}

}