#pragma once
#include <Graphics.h>
#include "IGraphicDriver.h"

namespace Sapphire
{
	struct ITextureMgr;
	struct IImageMgr;
	struct ITexture;
	struct IVertexBuffer;
	struct IIndexBuffer;
	struct IShaderVariation;
	class RenderSurface;
	class Texture2D;
	class GLRenderSystem;
	class ShaderMgr;
	class ShaderScriptMgr;

	///���㶥����µ�CPU�˻�����
	struct ScratchBuffer
	{
		ScratchBuffer() :
			m_size(0),
			m_bReserved(false)
		{
		}

		SharedArrayPtr<unsigned char> data_;
		unsigned m_size;
		bool m_bReserved;
	};

	/// FBO����
	struct FrameBufferObject
	{
		FrameBufferObject() :
			fbo(0),
			depthAttachment(0),
			readBuffers(M_MAX_UNSIGNED),
			drawBuffers(M_MAX_UNSIGNED)
		{
			for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
				colorAttachments[i] = 0;
		}

		/// FBO������
		uint  fbo;
		/// �󶨵���ɫ��������
		RenderSurface* colorAttachments[MAX_RENDERTARGETS];
		/// �󶨵����ģ�帽��
		RenderSurface* depthAttachment;
		/// ��ȡ������λ��
		uint readBuffers;
		/// ���ƻ�����λ��
		uint drawBuffers;
	};


	//OpenGL ��Ӳ������������ࡣ  
	//����IGraphicDriver�ӿ�, �޹�ͼ��API
	class GLGraphicDriver : public IGraphicDriver
	{
	public:

		typedef std::unordered_map<std::string, ImageType>   ImageTypeNameMap;

		GLGraphicDriver(Core* pCore);
		virtual ~GLGraphicDriver();

		virtual void Init();

		virtual void Release();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		Sapphire::ShaderMgr* getShaderMgr() const { return m_pShaderMgr; }

		Sapphire::ShaderScriptMgr*  getShaderScriptMgr() const { return m_pShaderScriptMgr; }
		//����ǰ��׼������
		virtual void PrepareDraw();

		//��һ������ָ��������Ԫ
		//�ȼ����Ӧ������Ԫ
		//Ȼ����������
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //��VBO����

		//��UBO����
		void BindUBO(uint uHwUID);  //��UBO

		bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

		PixelFormat GetPixelFormat(ImageType eImgType);

		PixelFormat GetPixelFormat(std::string szImageType);

		//ȡ�õ�ǰ�󶨵�FBO
		virtual uint GetCurrentBoundFBO() const;
		//��FBO����
		virtual void BindFrameBuffer(uint fbo);
		//����ɫ����
		virtual void BindColorAttachment(uint index, uint target, uint obj);
		//����ȸ���
		virtual void BindDepthAttachment(uint object, bool isRenderBuffer);
		//��ģ�帽��
		virtual void BindStencilAttachment(uint object, bool isRenderBuffer);

		virtual void DeleteFrameBuffer(uint fbo);
		//����֡������
		virtual uint CreateFramebuffer();


		int getTextureQuality() const { return m_nTextureQuality; }

		void setTextureQuality(int val) { m_nTextureQuality = val; }

		virtual IShaderProgram* GetShaderProgram() const;

		virtual IVertexBuffer* GetVertexBuffer(uint index) const;

		virtual IShaderVariation*  GetShader(ShaderType type, const std::string& name, const std::string define = "") const;

		virtual void SetShaders(IShaderVariation* vs, IShaderVariation* ps);

		virtual IShaderVariation* GetVertexShader() const;
		virtual IShaderVariation* GetPixelShader() const;

		virtual void CleanupShaderPrograms(IShaderVariation* pShaderVariation);

		//���ö�����㻺����
		virtual bool SetVertexBuffers(const std::vector<IVertexBuffer*>&  buffers, const std::vector<uint>& elememtMasks, uint instOffset = 0);

		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer);
		//������ȾĿ��
		virtual void SetRenderTarget(unsigned index, RenderSurface* renderTarget);

		virtual void SetRenderTarget(unsigned index, Texture2D* texture);

		virtual void SetDepthStencil(RenderSurface* depthStencil);

		virtual void SetDepthStencil(Texture2D* texture);

		virtual IIndexBuffer* GetIndexBuffer() const;

		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer); //����������������ͬʱ����

		virtual void ResetRenderTarget(uint index);

		virtual RenderSurface* GetRenderTarget(uint index) const;  //ȡ��ָ������ȾĿ��

		virtual RenderSurface* GetDepthStencil() const;     //ȡ�����ģ�����

		virtual void ResetDepthStencil();

		virtual void CleanupRenderSurface(RenderSurface* surface); //������FBO������һ����Ⱦ��

		//����һ��ScratchBuffer
		virtual void* ReserveScratchBuffer(ulong size);
		//�ͷ�һ��ScratchBuffer
		virtual void FreeScratchBuffer(void* buffer);
		//����̫��ScratchBuffer
		virtual void CleanScratchBuffers();
		//��ȡGLES�����ģ���ʽ
		virtual uint GetHWDepthStencilFormat() const { return m_glesHWDepthStencilFormat; }
		//�������֧��
		virtual void CheckFeatureSupport();

		//���OpenGL��չ
		virtual bool CheckExtension(const char* name);
		//����shadowMap ��������ʽ�� ���λ0��֧��
		virtual uint GetHWShadowMapFormat() const { return m_shadowMapFormat; }

		virtual GraphicDriverType getDriverType() const override;

	public:

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);

		static uint GetHWTextureDataType(PixelFormat format);
		//Ӳ��Alpha��ʽ
		static int GetHWAlphaFormat();
		//Ӳ�����ȸ�ʽ
		static int GetHWLuminanceFormat();
		//Ӳ�����Ⱥ�alpha��ʽ
		static int GetHWLuminanceAlphaFormat();
		//Ӳ��RGB��ʽ
		static int GetHWRGBFormat();
		//Ӳ��RGBA��ʽ
		static int GetHWRGBAFormat();




	private:

		void CheckFeature();

		GraphicDriverType  m_driverType;
		ITextureMgr*   m_pTextureMgr;
		IImageMgr*     m_pImageMgr;
		ShaderMgr*     m_pShaderMgr;
		ShaderScriptMgr*   m_pShaderScriptMgr;
		GLRenderSystem*  m_renderSys;

		int            m_nTextureQuality;
		bool		   m_bAnisotropySupport;
		uint           m_nMaxTextureUnits;
		ImageTypeNameMap   m_imagetypeNames;

		bool m_bGL3Support;

		uint m_curBoundVBO;  //��ǰ��VBO
		uint m_curBoundUBO;  //��ǰUBO
		uint m_curBoundBO;  //��ǰ��FBO
		uint m_sysFBO;      //ϵͳFBO

		/// �������õ���ȾĿ��
		RenderSurface* m_renderTargets[MAX_RENDERTARGETS];
		/// ��ǰ��Ŀ�껺����
		RenderSurface* m_depthStencil;

		//���õĶ��㻺������
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];
		//ʹ�õ�Ԫ������
		uint m_elementMasks[MAX_VERTEX_STREAMS];
		//��һ��ʹ�õ�ʵ������ƫ��
		uint m_lastInstOffset;

		bool m_fboDirty;  //FBO��Ҫ���±�־

		//ÿһ���ֱ��ʺ͸�ʽ��FBO��
		std::map<ulonglong, FrameBufferObject>  m_frameBuffers;  

		//��ǰʹ�õĶ�����������
		unsigned m_enabledAttributes;

		///��ǰʹ��indexBuffer
		IIndexBuffer* m_indexBuffer;

		//ScratchBuffers
		std::vector<ScratchBuffer> m_scratchBuffers;
		ulong m_maxScratchBufferRequest; //��ǰ֡�����ScratchBuffer���ֵ

		//glesӲ�����ģ���ʽ
		uint m_glesHWDepthStencilFormat;
		//glesӲ����ȡ��ȸ�ʽ
		uint m_glesHWReadableDepthFormat;
		//ShadowMap ��ȸ�ʽ
		uint m_shadowMapFormat;
		//ShadowMap 24λ��ȸ�ʽ
		uint m_hireShadowMapFormat;
		uint m_dummyColorFormat;

		//��ǰ���õ�shader
		GLShaderVariation* m_vertexShader;
		GLShaderVariation* m_pixelShader;
		GLShaderVariation* m_geometryShader;
		GLShaderVariation* m_computeShader;
		GLShaderProgram* m_shaderProgram;


	};
}