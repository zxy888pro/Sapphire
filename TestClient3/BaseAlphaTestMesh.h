#pragma once

#include <SapphireDef.h>
#include "BaseMesh.h"

namespace Sapphire
{
	class BaseAlphaTestMesh : public BaseMesh
	{
	public:


		virtual void Init() override;


		virtual void Render() override;
		virtual void SetDiffuseMap(const char* fileName);

		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

	private:

	protected:

		GLuint   m_diffMapObj;
		float*  m_vertices;

		virtual void Release() override;

	};
}
