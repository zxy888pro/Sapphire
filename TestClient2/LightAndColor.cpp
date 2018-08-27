#include "lightAndColor.h"
#include <logUtil.h>
#include <stringHelper.h>
#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Camera.h"

namespace Sapphire
{

	LightAndColor::LightAndColor()
	{
		m_uCubeShader = NULL;
		m_uLampShader = NULL;
		m_uVAO = 0;
		m_uVBO = 0;
		m_uEBO = 0;
		m_uLightVAO = 0;
	}

	LightAndColor::~LightAndColor()
	{
		Release();
	}

	void LightAndColor::Init(const char* cubeVs, const char* cubeFs, const char* lampVs, const char* lampFs)
	{
		m_uCubeShader = new Shader(cubeVs, cubeFs, "");
		if (m_uCubeShader == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", cubeVs, cubeFs));
			return;
		}
		m_uLampShader = new Shader(lampVs, lampFs, "");
		if (m_uLampShader == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", lampVs, lampFs));
			return;
		}
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,

			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,

			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,

			-0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, -0.5f,
			0.5f, -0.5f, 0.5f,
			0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, 0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, -0.5f,
			0.5f, 0.5f, 0.5f,
			0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, -0.5f,
		};
		glGenVertexArrays(1, &m_uVAO);
		glGenBuffers(1, &m_uVBO);
		glBindBuffer(GL_VERTEX_ARRAY, m_uVBO);
		glBufferData(GL_VERTEX_ARRAY, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(m_uVAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenVertexArrays(1, &m_uLightVAO);
		glBindVertexArray(m_uLightVAO);
		//和Cube相同的，用同樣的頂點數據，不需要再重新填充VBO對象了
		glBindBuffer(GL_VERTEX_ARRAY, m_uVBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		
	}

	void LightAndColor::Render()
	{
		// lighting
		glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

		m_uCubeShader->Use();
		glEnable(GL_DEPTH_TEST);
		m_uCubeShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		m_uCubeShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 projection = glm::perspective(glm::radians(Camera::GetSingletonPtr()->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = Camera::GetSingletonPtr()->GetViewMatrix();
		m_uCubeShader->setMat4("projection", projection);
		m_uCubeShader->setMat4("view", view);

		// world transformation
		glm::mat4 model;
		m_uCubeShader->setMat4("model", model);

		glBindVertexArray(m_uVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_uLampShader->Use();
		m_uLampShader->setMat4("projection", projection);
		m_uLampShader->setMat4("view", view);

		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		glBindVertexArray(m_uLightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glDisable(GL_DEPTH_TEST);
		m_uCubeShader->UnUse();
	}

	void LightAndColor::Release()
	{
		glDeleteProgram(m_uLampShader->getShaderProgram());
		glDeleteProgram(m_uCubeShader->getShaderProgram());
		safeDelete(m_uLampShader);
		safeDelete(m_uCubeShader);
		glBindVertexArray(m_uLightVAO);
		glDeleteVertexArrays(1, &m_uLightVAO);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uLightVBO);
		glDeleteBuffers(1, &m_uLightVBO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);


		glBindVertexArray(m_uVAO);
		glDeleteVertexArrays(1, &m_uVAO);
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_uVBO);
		glDeleteBuffers(1, &m_uVBO);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		m_uCubeShader = NULL;
		m_uLampShader = NULL;
		m_uVAO = 0;
		m_uVBO = 0;
		m_uEBO = 0;
		m_uLightVAO = 0;

	}

}