#pragma once



namespace Sapphire
{
	struct  IShaderVariation;
	 
	
	struct IShaderProgram 
	{
		virtual bool Link() = 0;
		virtual IShaderVariation* GetVertexShader() const = 0;
		virtual IShaderVariation* GetPixelShader() const = 0;
		virtual IShaderVariation* GetGeometryShader() const = 0;
		virtual bool HasTextureUnit(TextureUnit unit) const = 0;
		virtual bool HasParameter(std::string param) const = 0;
		virtual const std::string& GetLinkerOutput() const = 0;
		

	 

	};


}