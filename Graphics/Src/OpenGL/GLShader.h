#pragma once
#include "Graphics.h"
#include "IShader.h"
#include "GLShaderVariation.h"
#include "ShaderScript.h"
#include "FileStream.h"

namespace Sapphire
{

	class GLGraphicDriver;

	//Shader�YԴ����Դ�a�Ͷ���shader
	class GLShader :public IShader, public BaseResource
	{
		SAPPHIRE_OBJECT(GLShader, BaseResource)
	public:

		typedef std::unordered_map<StringHash, SharedPtr<GLShaderVariation>, StringHashFunc,StringHashCMP>  SHADERVARIATION_MAP;
		typedef std::unordered_map<StringHash, SharedPtr<GLShaderVariation>, StringHashFunc, StringHashCMP>::iterator  SHADERVARIATION_MAP_ITERATOR;
		typedef std::unordered_map<StringHash, SharedPtr<GLShaderVariation>, StringHashFunc, StringHashCMP>::const_iterator  SHADERVARIATION_MAP_CITERATOR;

		GLShader(Core* pCore,const char* name);
		virtual ~GLShader();

		//���ز�ͬԤ�����shader����
		IShaderVariation* GetVariation(ShaderType type, const std::string& defines);
		IShaderVariation* GetVariation(ShaderType type, const char* defines);

		const std::string&  GetSourceCode(ShaderType type) const;

		uint GetTimeStamp() const { return m_timeStamp; }

        //Resource���ֺ���
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


		virtual void Release();

		virtual bool Load() override;


		virtual bool Load(const char* resourcePath) override;


		virtual void Clear() override;


		virtual void Destroy() override;


		virtual void OnLoadStart() override;


		virtual void OnLoadEnd() override;


		virtual void OnLoadError() override;

	protected:

		virtual bool LoadShaderScript(const char* scriptPath, ShaderType type);  //����glsl shader�ű�

	private:

		//��stream����Դ��
		bool ProcessSource(std::string& source,  FileStream& istream);

		//��������岢������ظ�����
		std::string  NormalizeDefines(const std::string& defines);

		//���¼����ڴ�ռ��
		void RefreshMemoryUse();
		

		//�������shader��ԭ��
		std::string m_vsSource;
		std::string m_psSource;
		std::string m_gsSource;
		std::string m_csSource;
		//shader����
		std::string m_name;

		uint  m_timeStamp;
		uint  m_numVariation;
		


		//�������е�Shader����ʵ��
		SHADERVARIATION_MAP m_vsVariation;   
		SHADERVARIATION_MAP m_psVariation;
		SHADERVARIATION_MAP m_gsVariation;
		SHADERVARIATION_MAP m_csVariation;

		GLGraphicDriver* m_pGraphicDriver;
		bool  m_bIsDisposed;

	};
}