#pragma once
#include <Sapphire.h>
#include <glm.hpp>

namespace Sapphire
{
	class BaseEmissionMesh;

	enum LightType
	{
		LT_POINT,
		LT_DIRECTION,
		LT_SPOT,
		LT_MAXCOUNT
	};

	class BaseLight :public RefCounter
	{
	public:

		BaseLight();
		BaseLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType type);
		virtual ~BaseLight();

		void Init();
		void Update();

		glm::vec3 getPos() const { return m_pos; }
		void setPos(glm::vec3 val){ m_pos = val; };

		float getConstant() const { return m_constant; }
		void setConstant(float val) { m_constant = val; }
		float getLinear() const { return m_linear; }
		void setLinear(float val) { m_linear = val; }
		float getQuadratic() const { return m_quadratic; }
		void setQuadratic(float val) { m_quadratic = val; }
		glm::vec3 getDirection() const { return m_direction; }
		void setDirection(glm::vec3 val) { m_direction = val; }
		float getCutOff() const { return m_cutOff; }
		void setCutOff(float val) { m_cutOff = val; }
		float getOuterCutOff() const { return m_outerCutOff; }
		void setOuterCutOff(float val) { m_outerCutOff = val; }
		Sapphire::LightType getType() const { return m_type; }
		void setType(Sapphire::LightType val) { m_type = val; }
		glm::vec3 getAmbient() const { return m_ambient; }
		void setAmbient(glm::vec3 val) { m_ambient = val; }
		glm::vec3 getDiffuse() const { return m_diffuse; }
		void setDiffuse(glm::vec3 val) { m_diffuse = val; }
		glm::vec3 getSpecular() const { return m_specular; }
		void setSpecular(glm::vec3 val) { m_specular = val; }


	protected:


		float m_constant;
		float m_linear;
		float m_quadratic;
		float m_cutOff;
		float m_outerCutOff;

		LightType m_type;
		glm::vec3  m_pos;
		glm::vec3 m_direction;
		glm::vec3  m_ambient;
		glm::vec3  m_diffuse;
		glm::vec3  m_specular;


		BaseEmissionMesh* m_pMesh;
	};
}