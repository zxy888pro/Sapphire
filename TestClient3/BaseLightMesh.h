#pragma once
#include <Sapphire.h>
#include "BaseMesh.h"

namespace Sapphire
{
	 


	class BaseLightMesh :public BaseMesh
	{
	 
	public:

		BaseLightMesh();

		virtual ~BaseLightMesh();

		

		virtual void Init() override;


		virtual void Render() override;

		glm::vec3 getLightPos() const { return m_lightPos; }
		void setLightPos(glm::vec3 val) { m_lightPos = val; }

	protected:


		

		virtual void Release() override;
		glm::vec3  m_lightPos;
		

	private:

		float*  m_vertices;
		

		virtual void BackupRenderState() override;


		virtual void RestoreRenderState() override;

	};

}