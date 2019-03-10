#include "ShaderScriptMgr.h"

namespace Sapphire
{

	ShaderScriptMgr::ShaderScriptMgr()
	{

	}

	ShaderScriptMgr::~ShaderScriptMgr()
	{
		ShaderScriptMap::iterator i, begin = m_scriptMap.begin(), end = m_scriptMap.end();
		for (i = begin; i != end; ++i)
		{
			m_scripts.Dereference(i->second)->UnLoad();
		}
	}

	Sapphire::HSHADERSCRIPT ShaderScriptMgr::LoadScript(const char* name)
	{
		ShaderScriptMapInsertRc rc = m_scriptMap.insert(std::make_pair(name, HSHADERSCRIPT()));
		if (rc.second)
		{
			ShaderScript* pScript = m_scripts.Acquire(rc.first->second);
			if (!pScript->Load(rc.first->first))
			{
				DeleteScript(rc.first->second);
				rc.first->second = HSHADERSCRIPT();
			}
		}
		return (rc.first->second);
	}

	Sapphire::HSHADERSCRIPT ShaderScriptMgr::GetScript(const char* name)
	{
		ShaderScriptMap::iterator it = m_scriptMap.find(name);
		if (it != m_scriptMap.end())
		{
			return it->second;
		}
		return LoadScript(name);
	}

	void ShaderScriptMgr::DeleteScript(HSHADERSCRIPT hscript)
	{
		ShaderScript* pScript = m_scripts.Dereference(hscript);
		if (pScript != 0)
		{
			std::map<std::string, HSHADERSCRIPT, istring_less >::iterator it = m_scriptMap.find(pScript->GetName());
			if (it != m_scriptMap.end())
			{
				// 通过索引从Map中删除
				m_scriptMap.erase(it);
			}
			// 从数据库中删除
			pScript->UnLoad();
			m_scripts.Release(hscript);
		}
	}

	const std::string& ShaderScriptMgr::GetName(HSHADERSCRIPT hscript) const
	{
		return (m_scripts.Dereference(hscript)->GetName());
	}

	const std::string& ShaderScriptMgr::GetContent(HSHADERSCRIPT hscript) const
	{
		return (m_scripts.Dereference(hscript)->GetScriptContent());
	}

	Sapphire::ShaderType ShaderScriptMgr::GetType(HSHADERSCRIPT hscript) const
	{
		return (m_scripts.Dereference(hscript)->GetShaderType());
	}

}