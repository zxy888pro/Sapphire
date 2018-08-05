#include "Shader.h"
#include "FileStream.h"
#include "stringHelper.h"
#include "logUtil.h"
#include "Graphics.h"
#include "Image.h"


namespace Sapphire
{


	Shader::Shader(const char* vsPath, const char* psPath, const char* gsPath)
	{
		if (FileIsExistA(vsPath) && FileIsExistA(psPath))
		{
			LogUtil::LogMsgLn("找到shader文件");
			FileStream fs;
			fs.Open(vsPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
			m_vertexShaderStr = fs.ReadString(4096);
			ulong len = m_vertexShaderStr.size();
			GLchar* lpszVsBuf = new GLchar[len + 1];
			strcpy(lpszVsBuf, m_vertexShaderStr.c_str());
			fs.Release();
			fs.Open(psPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
			m_fragShaderStr = fs.ReadString(4096);
			len = m_fragShaderStr.size();
			GLchar* lpszPsBuf = new GLchar[len + 1];
			strcpy(lpszPsBuf, m_fragShaderStr.c_str());
			fs.Read(lpszPsBuf, len);
			fs.Release();

			GLchar* lpszGsBuf = NULL;
			if (FileIsExistA(gsPath))
			{
				fs.Open(gsPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
				m_geometryShaderStr = fs.ReadString(4096);
				len = m_geometryShaderStr.size();
				lpszGsBuf = new GLchar[len + 1];
				strcpy(lpszGsBuf, m_geometryShaderStr.c_str());
				fs.Read(lpszGsBuf, len);
				fs.Release();
			}
		
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

			GLuint geometryShader = 0;
			if (lpszGsBuf != NULL)
			{
				geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
				glShaderSource(geometryShader, 1, &lpszGsBuf, NULL);
				glCompileShader(geometryShader);

				//获取编译状态
				glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

				if (!success)
				{
					glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
					LogUtil::LogMsgLn(StringFormatA("ERROR::SHADER::GEOMETRY::COMPILATION_FAILED !  %s ", infoLog));
					goto InitClean;
				}
			}
		

			m_shaderProgram = glCreateProgram();
			glAttachShader(m_shaderProgram, vertexShader);
			glAttachShader(m_shaderProgram, fragmentShader);
			if (lpszGsBuf != NULL)
				glAttachShader(m_shaderProgram, geometryShader);
			glLinkProgram(m_shaderProgram);

			glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
				GLenum errorcode = glGetError();
				LogUtil::LogMsgLn(StringFormatA("SHADER::PROGRAME::LINK_FAILED\n  %s  ERROR CODE: %d", infoLog, errorcode));
			}

			glValidateProgram(m_shaderProgram);
			glGetProgramiv(m_shaderProgram, GL_VALIDATE_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
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
			if (lpszGsBuf != NULL)
				delete[] lpszGsBuf;
		}
	}

	void Shader::Use()
	{
		glUseProgram(m_shaderProgram);
	}

}