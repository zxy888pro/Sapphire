#pragma once
#include "stdafx.h"
#include <IImageMgr.h>

namespace Sapphire
{
	class ImageMgr;
}

class GLTest02
{
public:
	GLTest02();
	virtual ~GLTest02();

	virtual void Init(const char* vertexShaderFile, const char* fragmentShaderFile);
	virtual void Render();
	virtual void Clean();
	virtual void CreateRenderInfo();

protected:

	GLuint shaderProgram;
	GLfloat* vertices;
	GLuint* indices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	GLuint texture1;
	Sapphire::IImageMgr*  imgMgr;
};
