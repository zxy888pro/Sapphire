#pragma once
#include <GraphicsDefs.h>
#include <Graphics.h>
#include <GraphicDriver.h>
#include <ICubeTexture.h>
#include "GPUObject.h"
#include <Image.h>

namespace Sapphire
{
	class CubeTexture : public GPUObject, public BaseResource, public ICubeTexture
	{
	public:
		CubeTexture();
		CubeTexture(uint size, uint depth, PixelFormat pf = PF_R8G8B8A8, uint NumMipmaps = 1, int glTargerType = GL_TEXTURE_2D,
			TextureUsage eUsage = TextureUsage::TEXTURE_STATIC, TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT,
			TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT, TextureAddressMode r = TextureAddressMode::ADDRESS_REPEAT, 
			TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR);
		virtual ~CubeTexture();

		virtual void Release();//释放掉纹理对象
		virtual bool Recreate();//重新创建纹理对象
		virtual void Dispose();
		virtual bool  IsDisposed();
		virtual size_t GetSize();
		virtual uint GetDataSize() const override;
		virtual uint  GetRowSize() const override;
		virtual void Load(HIMAGE himg, CubeMapFace face);  //通过图像句柄加载纹理(6张贴图)
		virtual void Load(HIMAGE himg); //通过图像句柄加载纹理(一张贴图)
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual bool Create() override; //创建纹理对象

		virtual bool SetSize(int width, int height, PixelFormat eformat, TextureUsage usage = TEXTURE_STATIC);
		//设置更新某个面的数据
		virtual bool SetData(CubeMapFace face, uint level, int x, int y, int width, int height, const void* data);
		//获取某个面的数据
		virtual bool GetData(CubeMapFace face, unsigned level, void* dest) const;

	public:
		 
		virtual uint getWidth() const { return m_uWidth; }
		virtual uint getHeight() const { return m_uHeight; }
		virtual uint getDepth() const { return m_uDepth; }
		virtual void setDepth(uint val) { m_uDepth = val; }
		virtual uint getNumMipmaps() const { return m_uNumMipmaps; }
		virtual void setNumMipmaps(uint val) { m_uNumMipmaps = val; }
		virtual uint getMipLevel() const { return m_mipLevel; }
		virtual void setMipLevel(uint val) { m_mipLevel = val; }
		virtual bool getIsCompress() const { return m_bIsCompress; }
		virtual void setIsCompress(bool val) { m_bIsCompress = val; }
		virtual PixelFormat getPixelFormat() const { return m_ePixelFormat; }
		virtual void setPixelFormat(PixelFormat val) { m_ePixelFormat = val; }
		virtual Sapphire::TextureFilterMode getFilterMode() const { return m_eFilterMode; }
		virtual void setFilterMode(Sapphire::TextureFilterMode val) { m_eFilterMode = val; }
		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood){ return m_eAddressMode_[crood]; }
		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode){ m_eAddressMode_[crood] = mode; }
		virtual std::string getName() const { return m_szName; }
		virtual void setName(std::string val) { m_szName = val; }
		virtual uint  getAnisotropyLevel() const { return m_uAnisotropyLevel; }
		virtual void setAnisotropyLevel(uint val) { m_uAnisotropyLevel = val; }
		virtual Sapphire::TextureUsage getUsage() const { return m_eUsage; }
		virtual void setUsage(Sapphire::TextureUsage val) { m_eUsage = val; }
		virtual int getTextureType() const { return m_glType; }
		virtual uint getUID() const override;

	protected:

		uint m_uWidth;
		uint m_uHeight;
		uint m_uDepth;
		uint m_uNumMipmaps;
		uint m_mipLevel;
		uint m_uAnisotropyLevel;
		bool m_bIsCompress;
		bool m_bIsDisposed;
		HIMAGE m_hImg[6];    //6张贴图
		PixelFormat m_ePixelFormat;
		TextureFilterMode  m_eFilterMode;
		TextureAddressMode m_eAddressMode_[MAX_COORDS];
		TextureUsage   m_eUsage;
		std::string m_szName;
		ResoureType m_eType;
		size_t  m_uSize;
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
		int  m_glType;

		GraphicDriver*  m_pGraphicDriver;
	private:

	};
}