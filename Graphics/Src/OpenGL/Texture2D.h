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
	//OpenGL Texture2D
	class Texture2D : public GPUObject, public BaseResource, public ITexture
	{
		SAPPHIRE_OBJECT(Texture2D, BaseResource)
	public:

		friend class TextureMgr;

		Texture2D(Core* pCore, IGraphicDriver* pDriver, const char* name = "");
		Texture2D(Core* pCore, IGraphicDriver* pDriver, const char* name, uint width, uint height, uint depth, PixelFormat pf = PF_R8G8B8A8, uint NumMipmaps = 1, int glTargerType = GL_TEXTURE_2D,
			TextureUsage eUsage = TextureUsage::TEXTURE_STATIC,TextureAddressMode s = TextureAddressMode::ADDRESS_REPEAT,
			TextureAddressMode t = TextureAddressMode::ADDRESS_REPEAT,TextureFilterMode filterMode = TextureFilterMode::FILTER_BILINEAR);

		virtual ~Texture2D();
		virtual void Release();//�ͷŵ��������
		virtual bool Recreate();//���´����������
		virtual void Dispose();
		virtual bool  IsDisposed();
		virtual size_t GetSize();
		virtual uint GetDataSize() const override;
		virtual uint  GetRowSize() const override;
		virtual void Load(HIMAGE himg);  //ͨ��ͼ������������
		virtual void ReRequest();  //ͨ����������ȥ��������
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual bool Create() override; //�����������,��ɼy�팦��ĳ�ʼ�O��

		virtual bool SetSize(int width, int height, PixelFormat eformat, TextureUsage usage = TEXTURE_STATIC);  //��׃�y�팦��Ĵ�С
		virtual bool SetData(uint level, int x, int y, int width, int height, const void* data); //������������ 
		virtual bool SetData(HIMAGE himg, bool useAlpha);  //������������,-����mipmap ,  ��ֻ��һ��ͨ���ĕr��useAlpha��ʾ��alpha߀������
		virtual uint getLevelWidth(uint level) const override;
		virtual uint getLevelHeight(uint level) const override;
		virtual ITexture* getBackupTexture() const override;
		virtual uint getRequestMipLevel() const override;
		virtual void setRequestMipLevel(uint level) override;
		virtual void setBackupTexture(ITexture* tex) override;
		virtual bool IsCompressed() const override;

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
		virtual GLRenderSurface* getRenderSurface() const { return m_renderSurface; }
		virtual void RenderSurfaceUpdate();
	

	private:

		uint m_uWidth;
		uint m_uHeight;
		uint m_uDepth;
		uint m_uNumMipmaps;
		uint m_mipLevel;
		uint m_requestLevel;   //ҪՈ���mipmap level
		uint m_uAnisotropyLevel;
		bool m_bIsCompress;
		bool m_bIsDisposed;
		HIMAGE m_hImg;
		PixelFormat m_ePixelFormat;
		TextureFilterMode  m_eFilterMode;
		TextureAddressMode m_eAddressMode_[MAX_COORDS];
		TextureUsage   m_eUsage;
		std::string m_szName;
		ResoureType m_eType;
		size_t  m_uSize;
		uint m_maxMipLevel;
		unsigned m_skipMips[MAX_TEXTURE_QUALITY_LEVELS];

		SharedPtr<Texture2D>  m_backupTex;   //������������������Чʱ��ʹ�õ�Ĭ�������
		SharedPtr<GLRenderSurface>  m_renderSurface;     //���õ���ȾĿ��

		//OpenGL ����Ŀ������  ������
		//GL_TEXTURE_1D  :  Sampler1D
		//GL_TEXTURE_1D_ARRAY  : Sampler1DArray
		//GL_TEXTURE_2D   : Sampler2D
		//GL_TEXTURE_2D_ARRAY  :Sampler2DArray
		//GL_TEXTURE_3D : Sampler3D
		//GL_TEXTURE_2D_MULTISAMPLE : Simpler2DMS
		//GL_TEXTURE_2D_MULTISAMPLE_ARRAY: Sampler2DMSArray
		//GL_TEXTURE_CUBE_MAP :SamplerCube
		//GL_TEXTURE_RECTANGLE : SamplerRect   //�����ؾ��Σ� ��֧��mipmap
		//GL_TEXTURE_BUFFER :  samplerBuffer   //�������ص�1ά���飬��֧��mipmap
		int  m_glType;

		GLGraphicDriver*  m_pGraphicDriver;
	};

}


