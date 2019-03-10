#pragma once
#include "Graphics.h"
#include "ShaderScript.h"

namespace Sapphire
{
	class ShaderScriptMgr
	{
	public:

		ShaderScriptMgr();
		virtual ~ShaderScriptMgr();

		typedef HandleMgr <ShaderScript, HSHADERSCRIPT> HShaderScriptMgr;

		HSHADERSCRIPT   LoadScript(const char* name);

		HSHADERSCRIPT   GetScript(const char*  name);

		void            DeleteScript(HSHADERSCRIPT hscript);

		const std::string&  GetName(HSHADERSCRIPT  hscript) const;

		const std::string&  GetContent(HSHADERSCRIPT  hscript) const;

		ShaderType  GetType(HSHADERSCRIPT  hscript) const;

	private:

		struct istring_less
		{
			bool operator () (const std::string& l, const std::string& r) const
			{
				return (std::strcmp(l.c_str(), r.c_str()) < 0);
			}
		};

		typedef std::map <std::string, HSHADERSCRIPT, istring_less > ShaderScriptMap;
		typedef std::pair <ShaderScriptMap::iterator, bool> ShaderScriptMapInsertRc;

		HShaderScriptMgr m_scripts;
		ShaderScriptMap   m_scriptMap;

	};
}