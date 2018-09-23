#include "BaseAlphaBlendMesh.h"
#include <Sapphire.h>
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "Camera.h"

namespace Sapphire
{

	void BaseAlphaBlendMesh::Init()
	{
		LoadBaseShader("shaders\\BaseAlphaTestVs.glsl", "shaders\\BaseAlphaTestFs.glsl");
		m_vertices = _New float[30]{
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

			0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
			1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.5f, 0.0f, 1.0f, 0.0f
		};
		glGenVertexArrays(1, &m_mVao);

		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 30, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void BaseAlphaBlendMesh::Render()
	{
		 
	}

	void BaseAlphaBlendMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		 
	}

	void BaseAlphaBlendMesh::Release()
	{
		 
	}

}