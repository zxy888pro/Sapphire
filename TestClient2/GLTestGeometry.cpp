#include "GLTestGeometry.h"
#include <GraphicDriver.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Image.h"
#include "logUtil.h"
#include "Shader.h"
#include "Camera.h"
#include <IImageMgr.h>

Sapphire::GLTestGeometry::GLTestGeometry(Shader* pShader)
{
	m_shader = pShader;
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_vertices = NULL;
	m_indices = NULL;
	bDepthTest = false;
	m_textures.clear();
}

Sapphire::GLTestGeometry::~GLTestGeometry()
{
	Clean();
}

void Sapphire::GLTestGeometry::Init()
{
	 m_vertices =  new GLfloat[180]{
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};

	 //创建顶点数组对象
	 glGenVertexArrays(1, &m_VAO);
	 //绑定VAO
	 glBindVertexArray(m_VAO);
	 //创建缓冲区
	 glGenBuffers(1, &m_VBO);
	 //绑定VBO
	 glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	 //为VBO设置数据
	 glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 180, m_vertices, GL_STATIC_DRAW);
	 //位置
	 glEnableVertexAttribArray(0);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)0);
	 //uv
	 glEnableVertexAttribArray(1);
	 glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 3));
	 //解绑VBO VAO
	 glBindBuffer(GL_ARRAY_BUFFER, 0);
	 glBindVertexArray(0);

	 m_textures.push_back(0);
	 glGenTextures(1, &m_textures[0]);
	 glBindTexture(GL_TEXTURE_2D, m_textures[0]);

	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	 HIMAGE himg = GraphicDriver::GetSingletonPtr()->getImageMgr()->GetImage("1.jpg");
	 if (!himg.IsNull())
	 {
		 PRAWIMAGE pImgData = GraphicDriver::GetSingletonPtr()->getImageMgr()->GetTexture(himg);
		 uint width = GraphicDriver::GetSingletonPtr()->getImageMgr()->GetWidth(himg);
		 uint height = GraphicDriver::GetSingletonPtr()->getImageMgr()->GetHeight(himg);
		 uint nrChannels = GraphicDriver::GetSingletonPtr()->getImageMgr()->GetNumChannels(himg);

		if (pImgData)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImgData);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			LogUtil::LogMsgLn("Failed to load texture");
		}
	 }
	 m_shader->Use();
	 //glUseProgram(m_shader->getShaderProgram());
	 //glUniform1i(glGetUniformLocation(m_shader->getShaderProgram(), "myTexture1"), 0); //设置纹理单元0
	 m_shader->setInt("myTexture1", 0);
}

void Sapphire::GLTestGeometry::Render()
{
	backupState();
	m_shader->Use();
	glEnable(GL_DEPTH_TEST);
	glm::mat4 trans;
	//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 viewMat = Sapphire::Camera::GetSingletonPtr()->GetViewMatrix();
	glm::mat4 mvpMat = glm::perspective(glm::radians(Camera::GetSingletonPtr()->getZoom()), (float)800 / (float)600, 0.1f, 100.0f);
	trans = glm::translate(trans, m_position);
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

	mvpMat = mvpMat * viewMat * trans;
	m_shader->setMat4("transMat", mvpMat);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_textures[0]);
	glBindVertexArray(m_VAO);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	RestoreState();
	m_shader->UnUse();
}

void Sapphire::GLTestGeometry::Clean()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	if (m_vertices != NULL)
	{
		delete[] m_vertices;
		m_vertices = 0;
	}
	if (m_indices != NULL)
	{
		delete[] m_indices;
		m_indices = 0;
	}
	m_textures.clear();
}

void Sapphire::GLTestGeometry::backupState()
{
	glGetBooleanv(GL_DEPTH_TEST, &bDepthTest);
}

void Sapphire::GLTestGeometry::RestoreState()
{
	if (!bDepthTest)
	{
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glEnable(GL_DEPTH_TEST);
	}
}

