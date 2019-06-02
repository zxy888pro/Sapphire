#pragma once
#include "Graphics.h"
#include "IShader.h"
#include "GLShaderVariation.h"
#include "ShaderScript.h"

namespace Sapphire
{

	class GLGraphicDriver;

	//Shader資源，由源碼和多個shader
	class GLShader :public IShader, public BaseResource
	{
		SAPPHIRE_OBJECT(GLShader, BaseResource)
	public:
		GLShader(Core* pCore,const char* name);
		virtual ~GLShader();

		//加载不同预定义的shader对象
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
		virtual bool Load(HSHADERSCRIPT shaderPath);
		virtual const std::string& GetName() const
		{
			return m_name;
		}


		virtual bool Load() override;


		virtual bool Load(const char* resourcePath) override;


		virtual void Clear() override;


		virtual void Destroy() override;


		virtual void OnLoadStart() override;


		virtual void OnLoadEnd() override;


		virtual void OnLoadError() override;

	protected:

		virtual bool LoadShaderScript(const char* scriptPath, ShaderType type);  //加载glsl shader脚本

	private:

		//处理源码并包含文件
		bool ProcessSource(std::string source);

		//排序各定义并避免的重复定义
		std::string  NormalizeDefines(const std::string& defines);

		//重新计算内存占用
		void RefreshMemoryUse();
		

		//保存各种shader的原码
		std::string m_vsSource;
		std::string m_psSource;
		std::string m_gsSource;
		std::string m_csSource;
		//shader名字
		std::string m_name;

		uint  m_timeStamp;
		uint  m_numVariation;
		
		//保存所有的Shader对象实例
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_vsVariation;   
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_psVariation;
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_gsVariation;
		std::unordered_map<uint, SharedPtr<GLShaderVariation>> m_csVariation;

		GLGraphicDriver* m_pGraphicDriver;
		bool  m_bIsDisposed;

	};
}