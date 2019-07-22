#pragma once
#include <Graphics.h>
#include <GraphicDriver.h>
#include "GPUObject.h"
#include <Image.h>
#include <ITexture.h>
#include "GLRenderSurface.h"


namespace Sapphire
{
	class IGraphicDriver;
	class XMLElement;
	class XMLFile;
 

	class GLTexture : public GPUObject, public BaseResource, public ITexture
	{
		SAPPHIRE_OBJECT(GLTexture, BaseResource)

	public:

		GLTexture(Core* pCore, IGraphicDriver* pDriver, const char* resName = "");
		virtual ~GLTexture();

		virtual void Release();//释放掉纹理对象
		virtual bool Recreate();//重新创建纹理对象
		virtual void Dispose();
		virtual bool  IsDisposed();
		virtual size_t GetSize(); //获取内存占用大小
		virtual uint GetDataSize() const override;
		virtual uint  GetRowSize() const override;
		virtual void ReRequest();  //通过名字重新去请求纹理
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual bool Create() override; //创建纹理对象,完成紋理對象的初始設置

		virtual uint getLevelWidth(uint level) const override;
		virtual uint getLevelHeight(uint level) const override;
		virtual ITexture* getBackupTexture() const override;
		virtual uint getRequestMipLevel() const override;
		virtual void setRequestMipLevel(uint level) override;
		virtual void setBackupTexture(ITexture* tex) override;
		virtual bool IsCompressed() const override;

		virtual void SetSRGB(bool enable) override;
		virtual bool GetSRGB() const override;
		virtual uint GetHWFormat() const override;
		virtual void UpdateParameters() override;
		virtual void SetParameters(XMLFile* xmlFile); //解析xml配置参数
		virtual void SetParameter(const XMLElement& xmlElem); //解析xml元素
		virtual bool GetParametersDirty() const override;

		virtual uint getWidth() const { return m_uWidth; }
		virtual uint getHeight() const { return m_uHeight; }
		virtual uint getDepth() const { return m_uDepth; }
		virtual uint getNumMipmaps() const { return m_uNumMipmaps; }
		virtual PixelFormat getPixelFormat() const { return m_ePixelFormat; }
		virtual void setPixelFormat(PixelFormat val);
		virtual Sapphire::TextureFilterMode getFilterMode() const { return m_eFilterMode; }
		virtual void setFilterMode(Sapphire::TextureFilterMode val);
		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood){ return m_eAddressMode_[crood]; }
		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode);
		virtual std::string getName() const { return m_szName; }
		virtual void setName(std::string val) { m_szName = val; }
		virtual uint  getAnisotropyLevel() const { return m_uAnisotropyLevel; }
		virtual void setAnisotropyLevel(uint val) { m_uAnisotropyLevel = val; }
		virtual Sapphire::TextureUsage getUsage() const { return m_eUsage; }
		virtual void setUsage(Sapphire::TextureUsage val) { m_eUsage = val; }
		virtual int getTextureType() const { return m_glTexTarget; }
		virtual uint getUID() const override;
		virtual void RenderSurfaceUpdate();
		virtual uint getGPUHandle() const override { return m_uHwUID; }
		const Color& GetBorderColor() const { return m_borderColor; }
		void  SetBorderColor(const Color& color);
		void  SetShadowCompare(bool enable);
		bool  GetShadowCompare() const { return m_bShadowCompare; }

	public:

		unsigned GetSRGBFormat(unsigned format);
		
	protected:

		uint m_uWidth;
		uint m_uHeight;
		uint m_uDepth;
		uint m_uNumMipmaps;//当前的mipLevel等级 ,在创建的时候决定	
		uint m_requestLevel;   //要請求的mipmap level，下次创建的时候确定mipLevel
		uint m_uAnisotropyLevel;
		bool m_bIsCompress;
		bool m_bIsDisposed;
		HIMAGE m_hImg;
		PixelFormat m_ePixelFormat;
		uint		m_glTexFormat;
		TextureFilterMode  m_eFilterMode;
		TextureAddressMode m_eAddressMode_[MAX_COORDS];
		TextureUsage   m_eUsage;
		std::string m_szName;
		ResoureType m_eType;
		size_t  m_uSize;
		uint m_maxMipLevel;
		unsigned m_skipMips[MAX_TEXTURE_QUALITY_LEVELS];
		//阴影比较模式(ShadowMap)
		bool m_bShadowCompare;
		/// 边界颜色
		Color m_borderColor;
		SharedPtr<GLTexture>  m_backupTex;   //备份纹理，当主纹理无效时，使用的默认替代物
		

		//OpenGL 纹理目标类型  采样器
		//GL_TEXTURE_1D  :  Sampler1D
		//GL_TEXTURE_1D_ARRAY  : Sampler1DArray
		//GL_TEXTURE_2D   : Sampler2D
		//GL_TEXTURE_2D_ARRAY  :Sampler2DArray
		//GL_TEXTURE_3D : Sampler3D
		//GL_TEXTURE_2D_MULTISAMPLE : Simpler2DMS
		//GL_TEXTURE_2D_MULTISAMPLE_ARRAY: Sampler2DMSArray
		//GL_TEXTURE_CUBE_MAP :SamplerCube
		//GL_TEXTURE_RECTANGLE : SamplerRect   //简单纹素矩形， 不支持mipmap
		//GL_TEXTURE_BUFFER :  samplerBuffer   //任意纹素的1维数组，不支持mipmap
		int  m_glTexTarget;
		//是否使用SRGB， 2d纹理SRGB空间， 法线/高光线性空间
		bool m_bSRGB;
		bool m_bParametersDirty;   //纹理参数的脏标志


	};

}

