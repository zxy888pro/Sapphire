#pragma once
#include "BaseMesh.h"


namespace Sapphire
{
	class ITexture;

	class BaseEnviromentMapMesh : public BaseMesh
	{
	public:

		BaseEnviromentMapMesh();
		virtual ~BaseEnviromentMapMesh();




		virtual void LoadBaseShader(const char* vs, const char* ps) override;

		virtual void Init() override;

		virtual void Render() override;

		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

		virtual void SetCubeMap(ITexture* pTexture);

	protected:


		virtual void Release() override;
		GLuint  m_cubeMapObj;
	private:

	};

}