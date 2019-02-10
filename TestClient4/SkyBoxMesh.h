#pragma once

#include <Sapphire.h>
#include "Shader.h"
#include <glm.hpp>
#include "RenderState.h"
#include "BaseMesh.h"

namespace Sapphire
{
	class BaseLight;
	class IGraphicDriver;

	class SkyBoxMesh :public BaseMesh
	{
	public:
		SkyBoxMesh();
		virtual ~SkyBoxMesh();

		virtual void LoadBaseShader(const char* vs, const char* ps);
		virtual void Init();
		virtual void Render();
		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec);
		virtual void LoadCubeMap(std::string filePath);
		virtual uint GetSkyBoxUID();

	protected:

		virtual void Release() override;

	private:
		GLuint m_cubeMapObj;
		IGraphicDriver* m_pDriver;
	};
}

