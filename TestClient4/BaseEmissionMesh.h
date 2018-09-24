#pragma once
#include <Sapphire.h>
#include <glm.hpp>
#include "BaseMesh.h"


namespace Sapphire
{
	class BaseEmissionMesh : public BaseMesh
	{
	public:

		BaseEmissionMesh();

		virtual ~BaseEmissionMesh();



		virtual void Init() override;

		virtual void Render() override;

		glm::vec3 getColor() const { return m_color; }
		void setColor(glm::vec3 val) { m_color = val; }

	protected:

		virtual void Release() override;

	private:

		float*  m_vertices;

		glm::vec3  m_color;

		virtual void BackupRenderState() override;

		virtual void RestoreRenderState() override;
	};
}

