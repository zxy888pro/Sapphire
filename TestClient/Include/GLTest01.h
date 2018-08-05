#pragma once
#include "stdafx.h"



class GLTest01
{
public:

	GLTest01();
	~GLTest01();

	void Init();
	void Render();
	void Clean();
	void CreateRenderInfo();

private:

	GLuint shaderProgram;
	GLfloat* vertices;
	GLuint* indices;
	GLuint VAO;
	GLuint VBO;
	
};