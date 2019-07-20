#include "GLTest02.h"
#include "stdafx.h"
#include "Sapphire.h"
#include "FileStream.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "Graphics.h"
#include "Image.h"
#include <GraphicDriver.h>
#include <IGraphicDriver.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

GLTest02::GLTest02()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	vertices = NULL;
	indices = NULL;
	shaderProgram = 0;
	texture1 = 0;
}

GLTest02::~GLTest02()
{
	Clean();
}

Sapphire::Core* pcore;

void GLTest02::Init(const char* vertexShaderFile, const char* fragmentShaderFile)
{
	using namespace Sapphire;
	pcore = new Core();
	createGraphicDriver(pcore);
	IGraphicDriver* pDriver = dynamic_cast<IGraphicDriver*>(pcore->GetSubSystemWithType(ESST_GRAPHICDRIVER));
	imgMgr = pDriver->getImageMgr();
	if (FileIsExistA(vertexShaderFile) && FileIsExistA(fragmentShaderFile))
	{
		LogUtil::LogMsgLn("找到shader文件");
		FileStream fs;
		fs.Open(vertexShaderFile, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		std::string  vs1Str = fs.ReadString(4096);
		ulong len = vs1Str.size();
		GLchar* lpszVsBuf = new GLchar[len + 1];
		strcpy(lpszVsBuf, vs1Str.c_str());
		fs.Release();
		fs.Open(fragmentShaderFile, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		std::string ps1Str = fs.ReadString(4096);
		len = ps1Str.size();
		GLchar* lpszPsBuf = new GLchar[len + 1];
		strcpy(lpszPsBuf, ps1Str.c_str());
		fs.Read(lpszPsBuf, len);
		fs.Release();

		//定义顶点着色器句柄
		GLuint vertexShader;
		//创建shader
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//添加shader源代码
		glShaderSource(vertexShader, 1, &lpszVsBuf, NULL);
		glCompileShader(vertexShader);

		GLint success;
		GLchar infoLog[512];
		//获取编译状态
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			LogUtil::LogMsgLn(StringFormatA("ERROR::SHADER::VERTEX::COMPILATION_FAILED !  %s ", infoLog));
			goto InitClean;
		}

		GLuint fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &lpszPsBuf, NULL);
		glCompileShader(fragmentShader);

		//获取编译状态
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			LogUtil::LogMsgLn(StringFormatA("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED !  %s ", infoLog));
			goto InitClean;
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			GLenum errorcode = glGetError();
			LogUtil::LogMsgLn(StringFormatA("SHADER::PROGRAME::LINK_FAILED\n  %s  ERROR CODE: %d", infoLog, errorcode));
		}

		glValidateProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_VALIDATE_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			GLenum errorcode = glGetError();
			LogUtil::LogMsgLn(StringFormatA("SHADER::PROGRAME::VALIDATE_FAILED\n  %s  ERROR CODE: %d", infoLog, errorcode));
		}

	InitClean:

		if (vertexShader != 0)
			glDeleteShader(vertexShader);
		if (fragmentShader != 0)
			glDeleteShader(fragmentShader);
		if (lpszVsBuf != NULL)
			delete[] lpszVsBuf;
		if (lpszPsBuf != NULL)
			delete[] lpszPsBuf;

		return;
	}
}

void GLTest02::Render()
{
	glm::mat4 trans;
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transMat");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));


	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void GLTest02::Clean()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	if (vertices != NULL)
	{
		delete[] vertices;
		vertices = 0;
	}
	if (indices != NULL)
	{
		delete[] indices;
		indices = 0;
	}
	if (imgMgr != NULL)
	{
		delete imgMgr;
	}
}

void GLTest02::CreateRenderInfo()
{
	vertices = new GLfloat[(sizeof(GLfloat)*3+sizeof(GLfloat)*3 + sizeof(GLfloat)*2)*4]{
	   //---- 位置 ----    ---- 颜色 ----    - 纹理坐标 -
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上  

	};

	indices = new GLuint[6]
	{
		0, 1, 3, // first triangle
        1, 2, 3  // second triangle
	};

	//创建顶点数组对象
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);
	//创建缓冲区
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//为VBO设置数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 32, vertices, GL_STATIC_DRAW);
	//设置EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_UNSIGNED_INT) * 6, indices, GL_STATIC_DRAW);
	//位置
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0);
	//颜色
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat)*3));
	//uv
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(sizeof(GLfloat) * 6));
	//解绑VBO VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	Sapphire::HIMAGE   hImg = imgMgr->GetImage("1.jpg");
	PRAWIMAGE pImgData = imgMgr->GetImageRaw(hImg, 0);
	uint width = imgMgr->GetWidth(hImg);
	uint height = imgMgr->GetHeight(hImg);
	uint nrChannels = imgMgr->GetNumChannels(hImg);

	if (pImgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pImgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		Sapphire::LogUtil::LogMsgLn("Failed to load texture");
	}
	glUseProgram(shaderProgram);
	glUniform1i(glGetUniformLocation(shaderProgram, "myTexture1"), 0); //设置纹理单元0

	 
}
