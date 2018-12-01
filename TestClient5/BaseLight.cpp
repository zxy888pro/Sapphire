#include <MemoryMgr.h>
#include "BaseLight.h"
#include "BaseEmissionMesh.h"

namespace Sapphire
{



	BaseLight::BaseLight()
	{
		m_pos = glm::vec3(0, 0, 0);
		m_direction = glm::vec3(0, 0, 0);
		m_diffuse = glm::vec3(1, 1, 1);
		m_ambient = glm::vec3(1, 1, 1);
		m_specular = glm::vec3(1, 1, 1);
		m_type = LT_POINT;
		m_constant = 1.0;
		m_linear = 0.005;
		m_quadratic = 0.0005;
		m_cutOff = 0.5;
		m_outerCutOff = 0.2;
		m_pMesh = new BaseEmissionMesh();

	}

	BaseLight::BaseLight(glm::vec3 pos, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, LightType type) :
		m_type(type),
		m_pos(pos),
		m_direction(direction),
		m_ambient(ambient),
		m_diffuse(diffuse),
		m_specular(specular)
	{
		m_pos = glm::vec3(0, 0, 0);
		m_direction = glm::vec3(0, 0, 0);
		m_type = LT_POINT;
		m_constant = 1.0;
		m_linear = 0.005;
		m_quadratic = 0.0005;
		m_cutOff = 0.5;
		m_outerCutOff = 0.2;
		m_pMesh = new BaseEmissionMesh();
	}


	BaseLight::~BaseLight()
	{
		if (m_pMesh)
			_Delete m_pMesh;
	}

	void BaseLight::Init()
	{
		m_pMesh->LoadBaseShader("shaders\\BaseEmissionLightVs.glsl", "shaders\\BaseEmissionLightFs.glsl");
		m_pMesh->Init();
		Update();
	}


	void BaseLight::Update()
	{
		if (m_pMesh)
		{
			m_pMesh->setPos(m_pos);
			m_pMesh->setColor(m_diffuse);
		}
		m_pMesh->Render();
	}



}