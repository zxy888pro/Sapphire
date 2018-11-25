#pragma once
#include "BaseMesh.h"

namespace Sapphire
{
	class BaseUBOSimpleMesh : public BaseMesh
	{
	public:
		BaseUBOSimpleMesh();
		virtual ~BaseUBOSimpleMesh();


		virtual void Init() override;


		virtual void Render() override;


		virtual void Update(std::vector<SharedPtr<BaseLight>>& lightVec) override;
		 

		glm::vec3 getColor() const { return m_color; }
		void setColor(glm::vec3 val) { m_color = val; }

	protected:

		glm::vec3  m_color;

		GLuint m_uboObj;

		virtual void Release() override;

	};
}

