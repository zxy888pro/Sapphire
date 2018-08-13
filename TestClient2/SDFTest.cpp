#include "SDFTest.h"
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <FileStream.h>
#include <stringHelper.h>
#include <logUtil.h>
#include <regex>
#include <stdio.h>
#include "Image.h"

namespace Sapphire
{




	SDFTest::SDFTest(Shader* pShader)
	{
		m_pShader = pShader;
		VAO = 0;
		VBO = 0;
	}

	SDFTest::~SDFTest()
	{
		Clear();
	}

	void SDFTest::init(std::string sdfFontName, int width, int height)
	{
		std::string fontPath = sdfFontName + ".TTF_sdf.txt";
		FileStream fs;
		bool ret = fs.Open(fontPath, FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (ret)
		{
			std::string tmp = "";
			bool bRet = false;
			while (!bRet)
			{
				if (!fs.ReadLine(tmp))
					return;
				bRet = ReadFaceName(tmp);
			}
			bRet = false;
			while (!bRet && fs.GetCurPos())
			{
				if (!fs.ReadLine(tmp))
					return;
				bRet = ReadCharCount(tmp);
			}
			while (fs.ReadLine(tmp))
			{
				ReadCharData(tmp);
			}
			fs.Release();


		}

		//加载字体纹理
		std::string fontImgPath = sdfFontName + "_ttf_sdf.png";
		glGenTextures(1, &uTextureID);
		glBindTexture(GL_TEXTURE_2D, uTextureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		HIMAGE himg = ImageMgr::GetSingletonPtr()->GetImage(fontImgPath.c_str());
		if (!himg.IsNull())
		{
			LPRGBADATA pImgData = ImageMgr::GetSingletonPtr()->GetTexture(himg);
			uTextureWidth = ImageMgr::GetSingletonPtr()->GetWidth(himg);
			uTextrueHeight = ImageMgr::GetSingletonPtr()->GetHeight(himg);
			uTextureChannel = ImageMgr::GetSingletonPtr()->GetNumChannels(himg);

			if (pImgData)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, uTextureWidth, uTextrueHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pImgData);
			}
			else
			{
				LogUtil::LogMsgLn("Failed to load texture");
			}
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//动态更新
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void SDFTest::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
	{
		backupState();
		m_pShader->Use();
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(800), 0.0f, static_cast<GLfloat>(600));
		m_pShader->setMat4("projection", projection);
		m_pShader->setVec3("textColor", glm::vec3(color.x, color.y, color.z));
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(VAO);

		for (int i = 0; i < text.size(); ++i)
		{
			uint charcode = text[i];
			std::unordered_map<uint, SDFCharDataItem*>::iterator pair = charDataMap.find(charcode);
			if (pair != charDataMap.end())
			{
				SDFCharDataItem* item = pair->second;
				if (item != NULL)
				{
					//计算渲染位置
					GLfloat xpos = x;
					GLfloat ypos = y;
					GLfloat w = item->width * scale;
					GLfloat h = item->height * scale;
					//计算纹理坐标
					float tncX = ((float)item->x) / ((float)uTextureWidth);
					float tncY = ((float)item->y) / ((float)uTextrueHeight);
					float tnw = ((float)item->width) / ((float)uTextureWidth);
					float tnh = ((float)item->height) / ((float)uTextureWidth);

					
					GLfloat vertices[6][4] = {
						{ xpos, ypos + h, tncX, tncY },
						{ xpos, ypos, tncX, tncY+tnh },
						{ xpos + w, ypos, tncX + tnw, tncY+tnh },

						{ xpos, ypos + h, tncX, tncY },
						{ xpos + w, ypos, tncX+tnw, tncY+tnh },
						{ xpos + w, ypos + h, tncX+tnw, tncY }
					};


					// 在四边形上绘制字形纹理
					glBindTexture(GL_TEXTURE_2D, uTextureID);
					// 更新VBO内存的内容
					glBindBuffer(GL_ARRAY_BUFFER, VBO);
					glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
					glBindBuffer(GL_ARRAY_BUFFER, 0);

					x = xpos + item->xAdvance * scale;
					// 绘制四边形
					glDrawArrays(GL_TRIANGLES, 0, 6);
				}
			}
		}
		RestoreState();
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_pShader->UnUse();
	}

	void SDFTest::backupState()
	{
		glGetBooleanv(GL_CULL_FACE, &bCullFace);
		glGetBooleanv(GL_BLEND, &bAlphaBlend);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void SDFTest::RestoreState()
	{
		if (bCullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
		if (bAlphaBlend)
		{
			glEnable(GL_BLEND);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}

	bool SDFTest::ReadFaceName(std::string& src)
	{
		std::string facePattern = "[ ]*info[ ]*face=\"[A-Za-z]*\"";
		if (Match(src, facePattern))
		{
			std::smatch result;
			std::regex pattern("\"[A-Za-z]*\"");
			std::string::const_iterator iterStart = src.begin();
			std::string::const_iterator iterEnd = src.end();
			std::string tmp;
			if (std::regex_search(iterStart, iterEnd, result, pattern))
			{
				tmp = result[0];
				std::string repThis = "\"";
				ReplaceA(tmp, repThis, "");
				faceName = tmp;
				LogUtil::LogMsgLn(StringFormatA("Load SDFFont FaceName %s", faceName.c_str()));
				return true;
			}
		}
		return false;
	}

	bool SDFTest::ReadCharCount(std::string& src)
	{
		std::string facePattern = "[ ]*chars[ ]*count=[0-9]*";
		if (Match(src, facePattern))
		{
			std::smatch result;
			std::regex pattern("[1-9][0-9]*");
			std::string::const_iterator iterStart = src.begin();
			std::string::const_iterator iterEnd = src.end();
			std::string tmp;
			if (std::regex_search(iterStart, iterEnd, result, pattern))
			{
				tmp = result[0];
				charCount = atoi(tmp.c_str());
				LogUtil::LogMsgLn(StringFormatA("Load SDFFont CharCount %s", tmp.c_str()));
				return true;
			}
		}
		return false;
	}

	bool SDFTest::ReadCharData(std::string& src)
	{
		if (src.find("char id") != std::string::npos)
		{
			std::string tmp = src;
			std::regex pattern("[ ][ ]+");
			tmp = std::regex_replace(tmp, pattern, " ");
			SDFCharDataItem* item = new SDFCharDataItem();
			sscanf(src.c_str(), "char id=%d x=%d y=%d width=%d height=%d xoffset=%f yoffset=%f xadvance=%f page=%d chnl=%d", &(item->id),
				&(item->x), &(item->y), &(item->width), &(item->height), &(item->offsetX), &(item->offsetY), &(item->xAdvance), &(item->page), &(item->chnl)
				);
			charDataMap.insert(std::pair<uint, SDFCharDataItem*>(item->id, item));
			return true;
		}
		return false;
	}

	bool SDFTest::Match(std::string& src, std::string& pattarn)
	{
		std::regex repPattern(pattarn, std::regex_constants::extended);
		std::match_results<std::string::const_iterator> rerResult;
		bool bValid = regex_match(src, rerResult, repPattern);
		return bValid;
	}

	void SDFTest::Clear()
	{
		charCount = 0;
		faceName = "";
		std::unordered_map<uint, SDFCharDataItem*>::iterator it = charDataMap.begin();
		while (it != charDataMap.end())
		{
			if (it->second != NULL)
			{
				delete it->second;
				it->second = NULL;
			}
		}
		charDataMap.clear();
	}

}