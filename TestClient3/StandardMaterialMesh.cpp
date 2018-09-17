#include "StandardMaterialMesh.h"
#include <Sapphire.h>
#include <MemoryMgr.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <ITexture.h>
#include <ITextureMgr.h>
#include "Camera.h"
#include "BaseLight.h"
#include <glm.hpp>

namespace Sapphire
{

	StandardMaterialMesh::StandardMaterialMesh()
	{
		m_type = MT_StandardMaterialMesh;
		m_shininess = 12;
		m_showOutline = false;
	}

	StandardMaterialMesh::~StandardMaterialMesh()
	{
		setOutlineSize(0.1);
	}

	void StandardMaterialMesh::Init()
	{
		LoadBaseShader("shaders\\StandardMaterialVs.glsl", "shaders\\StandardMaterialFs.glsl");
		LoadOutlineShader("shaders\\OutlineVs.glsl", "shaders\\OutlineFs.glsl");
		m_vertices = _New float[288]{
			// positions          // normals   // texture coords
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
				-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
		};

		glGenVertexArrays(1, &m_mVao);


		glGenBuffers(1, &m_mVbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_mVbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 288, m_vertices, GL_STATIC_DRAW);

		glBindVertexArray(m_mVao);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void StandardMaterialMesh::Render()
	{

		if (m_showOutline)
		{
			BackupRenderState();
			//打开模板测试
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);   //模板测试函数 不等于参考值
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  //如果模板/深度测试都通过替换模板缓冲区的值为ref参考值
			//先正常绘制并更新模板缓冲区的值为1
			glStencilFunc(GL_ALWAYS, 1, 0xFF); //更新模板缓冲区
			glStencilMask(0xFF);
			m_pShader->Use();
			glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
			glBindTexture(GL_TEXTURE_2D, m_diffMapObj);
			glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
			glBindTexture(GL_TEXTURE_2D, m_sepcMapObj);
			glBindVertexArray(m_mVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			m_pShader->UnUse();
			// unbind diffuse map
			glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
			glBindTexture(GL_TEXTURE_2D, 0);
			// unbind specular map
			glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			//第二次绘制边框
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);  //模板不能在模板值=1的地方绘制
			glStencilMask(0x00);  //不更新只读模板缓冲区
			glDisable(GL_DEPTH_TEST);
			m_pOutlineShader->Use();
			Camera* pCam = Camera::GetSingletonPtr();
			if (pCam == NULL)
			{
				return;
			}
			glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
			glm::mat4 view = pCam->GetViewMatrix();
			m_pOutlineShader->setMat4("projection", projection);
			m_pOutlineShader->setMat4("view", view);
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, m_pos);
			float outlineScale = 1 + getOutlineSize();
			model = glm::scale(model, glm::vec3(outlineScale, outlineScale, outlineScale));
			m_pOutlineShader->setMat4("model", model);
			glBindVertexArray(m_mVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			glStencilMask(0xFF);
			glEnable(GL_DEPTH_TEST);
			m_pOutlineShader->UnUse();




			RestoreRenderState();
		}
		else
		{
			m_pShader->Use();
			BackupRenderState();
			glEnable(GL_DEPTH_TEST);
			// bind diffuse map
			glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
			glBindTexture(GL_TEXTURE_2D, m_diffMapObj);
			// bind specular map
			glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
			glBindTexture(GL_TEXTURE_2D, m_sepcMapObj);

			glBindVertexArray(m_mVao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
			m_pShader->UnUse();
			// unbind diffuse map
			glActiveTexture(GL_TEXTURE0 + TU_DIFFUSE);
			glBindTexture(GL_TEXTURE_2D, 0);
			// unbind specular map
			glActiveTexture(GL_TEXTURE0 + TU_SPECULAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			RestoreRenderState();
		}

		
	}

	void StandardMaterialMesh::Update(std::vector<SharedPtr<BaseLight>>& lightVec)
	{
		SharedPtr<BaseLight> dirLight;
		std::vector<SharedPtr<BaseLight>> pointLights;
		SharedPtr<BaseLight> spotLight;

		for (int i = 0; i < lightVec.size(); ++i)
		{
			switch (lightVec[i]->getType())
			{
			case  LT_DIRECTION:
			{
				if (dirLight.NotNull())
				{
					continue;
				}
				dirLight = lightVec[i];
			}
			break;
			case LT_POINT:
			{
				if (pointLights.size() < 8)
				{
					pointLights.push_back(lightVec[i]);
				}
				continue;
			}
				break;
			case LT_SPOT:
			{
				if (spotLight.NotNull())
				{
					continue;
				}
				spotLight = lightVec[i];
			}
				break;
			default:
				break;
			}
		}
		m_pShader->Use();
		if (dirLight.NotNull())
		{
			//方向光
			m_pShader->setVec3("dirLight.direction", dirLight->getDirection());
			m_pShader->setVec3("dirLight.ambient", dirLight->getAmbient());
			m_pShader->setVec3("dirLight.diffuse", dirLight->getDiffuse());
			m_pShader->setVec3("dirLight.specular", dirLight->getSpecular());
		}
		else
		{
			//设一个默认值，可能除0异常
			m_pShader->setVec3("dirLight.direction", glm::vec3(0, 0, 0));
			m_pShader->setVec3("dirLight.ambient", glm::vec3(0, 0, 0));
			m_pShader->setVec3("dirLight.diffuse", glm::vec3(0, 0, 0));
			m_pShader->setVec3("dirLight.specular", glm::vec3(0, 0, 0));

		}
		
		//聚光灯
		if (spotLight.NotNull())
		{
			m_pShader->setVec3("spotLight.position", spotLight->getPos());
			m_pShader->setVec3("spotLight.direction", spotLight->getDirection());
			m_pShader->setFloat("spotLight.outerCutOff", spotLight->getOuterCutOff());
			m_pShader->setFloat("spotLight.cutOff", spotLight->getCutOff());
			m_pShader->setFloat("spotLight.constant", spotLight->getConstant());
			m_pShader->setFloat("spotLight.linear", spotLight->getLinear());
			m_pShader->setFloat("spotLight.quadratic", spotLight->getQuadratic());
			m_pShader->setVec3("spotLight.ambient", spotLight->getAmbient());
			m_pShader->setVec3("spotLight.diffuse", spotLight->getDiffuse());
			m_pShader->setVec3("spotLight.specular", spotLight->getSpecular());
		}
		else
		{
			//设一个默认值，可能除0异常
			m_pShader->setVec3("spotLight.position", glm::vec3(0,0,0));
			m_pShader->setVec3("spotLight.direction", glm::vec3(0, 0, 0));
			m_pShader->setFloat("spotLight.outerCutOff", 0);
			m_pShader->setFloat("spotLight.cutOff", 0);
			m_pShader->setFloat("spotLight.constant", 1);
			m_pShader->setFloat("spotLight.linear", 0);
			m_pShader->setFloat("spotLight.quadratic", 0);
			m_pShader->setVec3("spotLight.ambient", glm::vec3(0, 0, 0));
			m_pShader->setVec3("spotLight.diffuse", glm::vec3(0, 0, 0));
			m_pShader->setVec3("spotLight.specular", glm::vec3(0, 0, 0));
		}
		//动态设置光源数目
		if (pointLights.size() > 0)
		{
			m_pShader->setInt("pointLightCount", pointLights.size());
			std::string tmp;
			for (int i = 0; i < pointLights.size(); ++i)
			{ 
				tmp = StringFormatA("pointLights[%d].position", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getPos());
				tmp = StringFormatA("pointLights[%d].direction", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getDirection());
				tmp = StringFormatA("pointLights[%d].constant", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getConstant());
				tmp = StringFormatA("pointLights[%d].linear", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getLinear());
				tmp = StringFormatA("pointLights[%d].quadratic", i);
				m_pShader->setFloat(tmp.c_str(), pointLights[i]->getQuadratic());
				tmp = StringFormatA("pointLights[%d].ambient", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getAmbient());
				tmp = StringFormatA("pointLights[%d].diffuse", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getDiffuse());
				tmp = StringFormatA("pointLights[%d].specular", i);
				m_pShader->setVec3(tmp.c_str(), pointLights[i]->getSpecular());
			}
		}
		m_pShader->setFloat("material.shininess", m_shininess);
		Camera* pCam = Camera::GetSingletonPtr();
		if (pCam == NULL)
		{
			return;
		}

		glm::mat4 projection = glm::perspective(glm::radians(pCam->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
		glm::mat4 view = pCam->GetViewMatrix();
		m_pShader->setVec3("viewPos", pCam->getPosition());
		m_pShader->setMat4("projection", projection);
		m_pShader->setMat4("view", view);
		glm::mat4 model;
		model = glm::translate(model, m_pos);
		m_pShader->setMat4("model", model);
		m_pShader->UnUse();

	}

	void StandardMaterialMesh::Release()
	{
		BaseMesh::Release();
		if (m_vertices != NULL)
		{
			_Delete[] m_vertices;
		}
	}

	bool StandardMaterialMesh::LoadOutlineShader(const char* vs, const char* ps)
	{
		m_pOutlineShader = new Shader(vs, ps, "");
		if (m_pOutlineShader == NULL)
		{
			LogUtil::LogMsgLn(StringFormatA("Create %s %s Shader Failed!", vs, ps));
			return false;
		}
		return true;
	}

	void StandardMaterialMesh::BackupRenderState()
	{
		m_state.LoadCurRenderState();
	}

	void StandardMaterialMesh::RestoreRenderState()
	{
		m_state.SetState();
	}

}