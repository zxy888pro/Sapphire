#pragma once
#include "Graphics.h"
#include "IShader.h"
#include "GLShaderVariation.h"

namespace Sapphire
{


	//Shader資源，由源碼和多個shader
	class GLShader :public IShader, public BaseResource, public RefCounter
	{
	public:
		GLShader();
		virtual ~GLShader();

		IShaderVariation* GetVariation(ShaderType type, const std::string& defines);
		IShaderVariation* GetVariation(ShaderType type, const char* defines);

		const std::string&  GetSourceCode(ShaderType type) const;

		uint GetTimeStamp() const { return m_timeStamp; }

        //Resource部分函數
		virtual bool Create() override;
		virtual bool Recreate() override;
		virtual void Dispose() override;
		virtual size_t GetSize() override;
		virtual bool IsDisposed() override;

	private:

		//处理源码并包含文件
		bool ProcessSource(std::string source);

		//排序各定义并避免的重复定义
		std::string  NormalizeDefines(const std::string& defines);
		

		//保存各种shader的原码
		std::string m_vsSource;
		std::string m_psSource;
		std::string m_gsSource;

		uint  m_timeStamp;
		uint  m_numVariation;

		std::unordered_map<std::string, SharedPtr<GLShaderVariation>> vsVariation;
		std::unordered_map<std::string, SharedPtr<GLShaderVariation>> psVariation;
		std::unordered_map<std::string, SharedPtr<GLShaderVariation>> gsVariation;

	};
}