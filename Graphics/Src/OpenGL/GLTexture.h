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

		virtual void Release();//�ͷŵ��������
		virtual bool Recreate();//���´����������
		virtual void Dispose();
		virtual bool  IsDisposed();
		virtual size_t GetSize(); //��ȡ�ڴ�ռ�ô�С
		virtual uint GetDataSize() const override;
		virtual uint  GetRowSize() const override;
		virtual void ReRequest();  //ͨ����������ȥ��������
		virtual void OnDeviceLost() override;
		virtual void OnDeviceReset() override;
		virtual bool Create() override; //�����������,��ɼy�팦��ĳ�ʼ�O��

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
		virtual void SetParameters(XMLFile* xmlFile); //����xml���ò���
		virtual void SetParameter(const XMLElement& xmlElem); //����xmlԪ��
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
		uint m_uNumMipmaps;//��ǰ��mipLevel�ȼ� ,�ڴ�����ʱ�����	
		uint m_requestLevel;   //ҪՈ���mipmap level���´δ�����ʱ��ȷ��mipLevel
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
		//��Ӱ�Ƚ�ģʽ(ShadowMap)
		bool m_bShadowCompare;
		/// �߽���ɫ
		Color m_borderColor;
		SharedPtr<GLTexture>  m_backupTex;   //������������������Чʱ��ʹ�õ�Ĭ�������
		

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
		int  m_glTexTarget;
		//�Ƿ�ʹ��SRGB�� 2d����SRGB�ռ䣬 ����/�߹����Կռ�
		bool m_bSRGB;
		bool m_bParametersDirty;   //������������־


	};

}

