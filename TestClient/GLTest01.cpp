#include "Include/GLTest01.h"
#include "stdafx.h"
#include "Sapphire.h"
#include "FileStream.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "Image.h"

GLTest01::GLTest01()
{
	VAO = 0;
	VBO = 0;
	vertices = NULL;
	indices = NULL;
	shaderProgram = 0;
}

GLTest01::~GLTest01()
{
	Clean();
}

void GLTest01::Init()
{
	using namespace Sapphire;
	if (FileIsExistA("ps1.glsl") && FileIsExistA("vs1.glsl"))
	{
		LogUtil::LogMsgLn("找到shader文件");
		FileStream fs;
		fs.Open("vs1.glsl", FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		std::string  vs1Str = fs.ReadString(4096);
		ulong len = vs1Str.size();
		GLchar* lpszVsBuf = new GLchar[len+1];
		strcpy(lpszVsBuf, vs1Str.c_str());
		fs.Release();
		fs.Open("ps1.glsl", FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		std::string ps1Str = fs.ReadString(4096);
		len = ps1Str.size();
		GLchar* lpszPsBuf = new GLchar[len+1];
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

void GLTest01::Render()
{
	
	glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(shaderProgram);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLTest01::Clean()
{
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
	
	
	 
}

void GLTest01::CreateRenderInfo()
{

	vertices = new GLfloat[9]{
		    -0.5f, -0.5f, 0.0f, // left  
			0.5f, -0.5f, 0.0f, // right 
			0.0f, 0.5f, 0.0f  // top  
	};


	indices = new GLuint[3] { // 起始于0!

		0, 1, 2 // 第一个三角形
	};
	//创建顶点数组对象
	glGenVertexArrays(1, &VAO);
	//绑定VAO
	glBindVertexArray(VAO);
	//创建缓冲区
	glGenBuffers(1, &VBO);	
	//绑定VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//为VBO设置数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT)*9, vertices, GL_STATIC_DRAW);
	//顶点属性location作为它的参数   属性位置0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	//解绑VBO VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	
	


	
}
