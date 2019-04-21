#pragma once
#include <Sapphire.h>
#include "BaseMesh.h"

namespace Sapphire
{



	class BaseLightMesh :public BaseMesh
	{
		SAPPHIRE_OBJECT(BaseLightMesh, BaseMesh)
	public:

		BaseLightMesh();

		virtual ~BaseLightMesh();



		virtual void Init() override;


		virtual void Render() override;

		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;

		glm::vec3 getColor() const { return m_color; }
		void setColor(glm::vec3 val) { m_color = val; }

	protected:

		virtual void Release() override;

	private: 

		glm::vec3 m_color;

	};

}