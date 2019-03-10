#include "Graphics.h"
#include "ShaderMgr.h"
#include "GLGraphicDriver.h"
#include "ShaderScript.h"
#include "ShaderScriptMgr.h"
#include "GLShader.h"
#include <GraphicException.h>
#include "FileStream.h"
#include "json/json.h"

namespace Sapphire
{

	 

	ShaderMgr::ShaderMgr()
	{
		//设置最大内存上限
		SetMaximumMemory(1024 * 1024 * 512);
		//先预分配内存
		ReserveMemory(1024 * 1024 * 64);

		m_pGraphicDriver = dynamic_cast<GLGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(ESST_GRAPHICDRIVER));
	}

	ShaderMgr::~ShaderMgr()
	{
		Destroy();
	}

	Sapphire::GLShader* ShaderMgr::CreateShader(std::string filePath)
	{
		ShaderScriptMgr*   pScriptMgr = m_pGraphicDriver->getShaderScriptMgr();
		Core* pCore = Core::GetSingletonPtr();
		if (pScriptMgr == NULL || pCore == NULL)
		{
			throw GraphicDriverException("Sapphire Component is not Created!", GraphicDriverException::GDError_ComponentNotCreate);
		}
		FileStream fs(filePath.c_str(), FileMode::FILE_EXIST | FileMode::FILE_READ | FileMode::FILE_READ | FileMode::FILE_STRING);
		if (fs.IsOpen())
		{
			std::string jsonStr = fs.ReadString(MAX_JSON_LENGTH);
			Json::CharReaderBuilder builder;
			Json::CharReader* reader = builder.newCharReader();
			JSONCPP_STRING errs;
			Json::Value rootNode;
			if (reader->parse(jsonStr.c_str(), jsonStr.c_str() + strlen(jsonStr.c_str()), &rootNode, &errs))
			{
				Path vsFile = rootNode["vertexShader"].asCString;
				Path psFile = rootNode["pixelShader"].asCString;
				Path gsFile = rootNode["geometryShader"].asCString;
				Path csFile = rootNode["computeShader"].asCString;
				GLShader* pShader = new GLShader();
				//这里判断一下shader类型是否对应
				HSHADERSCRIPT hvs = pScriptMgr->GetScript(vsFile.c_str());
				if (!hvs.IsNull() && (pScriptMgr->GetType(hvs) == ShaderType::VS))
				{
					pShader->Load(hvs);
				}
				HSHADERSCRIPT hps = pScriptMgr->GetScript(psFile.c_str());
				if (!hps.IsNull() && (pScriptMgr->GetType(hps) == ShaderType::PS))
				{
					pShader->Load(hps);
				}
				HSHADERSCRIPT hgs = pScriptMgr->GetScript(gsFile.c_str());
				if (!hgs.IsNull() && (pScriptMgr->GetType(hgs) == ShaderType::GS))
				{
					pShader->Load(hgs);
				}
				HSHADERSCRIPT hcs = pScriptMgr->GetScript(csFile.c_str());
				if (!hcs.IsNull() && (pScriptMgr->GetType(hcs) == ShaderType::CS))
				{
					pShader->Load(hcs);
				}
				RHANDLE handle = 0;
				InsertResource(&handle, pShader);
				return pShader;
			}
			fs.Release();
		}
		return NULL;

	}

	 

	void ShaderMgr::StoreScriptDependency(std::string name, ShaderScript* pScripts)
	{

	}

	std::string ShaderMgr::GetFileName(const std::string Name, ShaderType type)
	{
		std::string fileName = Name + "_" + ENUM2STR(type);
		fileName += ".glsl";
		return fileName;
	}

}