#pragma once
#include <SapphireDef.h>
#include "BaseLightMapMesh.h"


namespace Sapphire
{
	class BaseAlphaBlendMesh : public BaseLightMapMesh
	{
	public:

		BaseAlphaBlendMesh();
		virtual ~BaseAlphaBlendMesh();


		virtual void Init() override;


		virtual void Render() override;


		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

	protected:

		float*  m_vertices;

		virtual void Release() override;

	private:


		virtual void BackupRenderState() override;


		virtual void RestoreRenderState() override;

	};
}