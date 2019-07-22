#pragma once
#include <Graphics.h>


namespace Sapphire
{

	//�豸�޹ص�Texture�ӿ�
	//ע��������Texture����ֱ��ɾ������TextureManager
	struct ITexture
	{
		virtual uint getWidth() const = 0;
		virtual uint getHeight() const = 0;
		virtual uint getLevelWidth(uint level) const = 0;  //��ȡָ��mipmap�ȼ��µĿ��
		virtual uint getLevelHeight(uint level) const = 0; ////��ȡָ��mipmap�ȼ��µĸ߶�
		virtual uint getDepth() const = 0;
		virtual uint getNumMipmaps() const = 0;
		virtual uint getRequestMipLevel() const = 0;
		virtual void setRequestMipLevel(uint val) = 0;
		virtual PixelFormat getPixelFormat() const = 0;
		virtual void setPixelFormat(PixelFormat val) = 0; //����ͨ�õ����ظ�ʽ����
		virtual Sapphire::TextureFilterMode getFilterMode() const = 0;
		virtual void setFilterMode(Sapphire::TextureFilterMode val) = 0;
		virtual Sapphire::TextureAddressMode getAddressMode(TextureCoordinate crood) = 0;
		virtual void setAddressMode(TextureCoordinate crood, TextureAddressMode mode) = 0;
		virtual uint  GetDataSize() const = 0;  //ȡ�������ܴ�С
		virtual uint  GetRowSize() const = 0;   //ȡ������һ�е����ݴ�С
		virtual std::string getName() const = 0;	//ȡ����Դ��
		virtual void setName(std::string val) = 0;   //������Դ��
		virtual uint  getAnisotropyLevel() const = 0;
		virtual void setAnisotropyLevel(uint val) = 0;
		virtual Sapphire::TextureUsage getUsage() const = 0;
		virtual void setUsage(Sapphire::TextureUsage val) = 0;
		virtual int getTextureType() const = 0;  //OpenGL�Ļ�������OpenGL������Ŀ��GL_TEXTURE_1D,GL_TEXTURE_2D...
		virtual uint getUID() const = 0;
		virtual uint getGPUHandle() const = 0;
		virtual ITexture* getBackupTexture() const = 0;   //��ȡ��������  ��ʹ�õ�Ĭ�Ͽ�����
		virtual void setBackupTexture(ITexture* tex) = 0;    //���ñ�������
		virtual void RenderSurfaceUpdate() = 0;
		virtual bool IsCompressed() const = 0;    //�y���Ƿ񉺿s
		virtual bool GetSRGB() const = 0;
		virtual void SetSRGB(bool enable) = 0;       
		virtual uint GetHWFormat() const = 0;      //��ȡӲ�������ʽ
		virtual void UpdateParameters() = 0;       //���¼y�텢��
		virtual bool GetParametersDirty() const = 0;   //�y�텢���Ƿ��v��

	
	};
}