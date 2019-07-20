#pragma once
#include "Sapphire.h"
#include <Graphics.h>
#include "IGraphicDriver.h"
#include "Thread.h"
#include "Math/Rect.h"


namespace Sapphire
{
	struct ITextureMgr;
	struct IImageMgr;
	struct ITexture;
	struct IVertexBuffer;
	struct IIndexBuffer;
	struct IShaderProgram;
	struct IShaderVariation;
	class GLRenderSurface;
	class Texture2D;
	class GLRenderSystem;
	class ShaderMgr;
	class ShaderScriptMgr;
	class ConstantBuffer;
	class GLShaderVariation;
	class GLShaderProgram;
	class GLDisplayContext;
	class GPUObject;


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
		GLRenderSurface* colorAttachments[MAX_RENDERTARGETS];
		/// �󶨵����ģ�帽��
		GLRenderSurface* depthAttachment;
		/// ��ȡ������λ��
		uint readBuffers;
		/// ���ƻ�����λ��
		uint drawBuffers;
	};


	//OpenGL ��Ӳ������������ࡣ  
	//����IGraphicDriver�ӿ�, �޹�ͼ��API
	class SAPPHIRE_CLASS GLGraphicDriver : public IGraphicDriver
	{
		SAPPHIRE_OBJECT(GLGraphicDriver, IGraphicDriver)

	public:

		typedef std::unordered_map<std::string, ImageType>   ImageTypeNameMap;

		GLGraphicDriver(Core* pCore);
		virtual ~GLGraphicDriver();

		virtual void Init();

		virtual void Release();

		//������ʾģʽ���ı䴰�ڻ򴴽��´���
		virtual bool SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable);

		/// �ָ�GPU�������³�ʼ������Ҫ�Ѿ��򿪵Ĵ���
		void Restore();
		/// �������
		void Maximize();
		/// ��С������
		void Minimize();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		Sapphire::ShaderMgr* getShaderMgr() const { return m_pShaderMgr; }

		Sapphire::ShaderScriptMgr*  getShaderScriptMgr() const { return m_pShaderScriptMgr; }
		//����ǰ��׼������,��Ⱦǰ���ã�����UBO��FBO
		virtual void PrepareDraw();

		//��һ������ָ��������Ԫ
		//�ȼ����Ӧ������Ԫ
		//Ȼ����������
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //��VBO����

		//��UBO����
		void BindUBO(uint uHwUID);  //��UBO

		virtual bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

		IntVector2 GetRenderTargetDimensions() const;

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
		/// �������е�֡��������������DisplayContext֮ǰ����
		virtual void CleanFrameBuffers();
		/// �������е���ȾĿ�꣬��Ȼ�������ģ�建����
		virtual void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);

		/// ���һ�����ٵ�GPUObject������GPUObject����
		virtual void AddGPUObject(GPUObject*  gpuObj);
		/// �Ƴ�һ��GPUObject������GPUObject����
		virtual void RemoveGPUObject(GPUObject* gpuObj);


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
		//���ö��㻺����
		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer);
		//������ȾĿ��
		virtual void SetRenderTarget(unsigned index, GLRenderSurface* renderTarget);
		//������ȾĿ��
		virtual void SetRenderTarget(unsigned index, Texture2D* texture);
		//�������ģ�����
		virtual void SetDepthStencil(GLRenderSurface* depthStencil);
		//�������ģ�����
		virtual void SetDepthStencil(Texture2D* texture);

		virtual IIndexBuffer* GetIndexBuffer() const;

		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer); //����������������ͬʱ����

		virtual void SetViewport(const IntRect& rect);  //�����ӿ�

		/// ������ɫд��
		void SetColorWrite(bool enable);
		///  �����޳�ģʽ
		virtual void SetCullMode(CullMode mode);
		///  �������ģʽ
		virtual void SetFillMode(FillMode mode);
		///  ������Ȳ���
		void SetDepthTest(CompareMode mode);
		///	�������д��
		void SetDepthWrite(bool enable);
		///  ����Alpha���ģʽ
		void SetBlendMode(BlendMode mode);
		///  ���ü��ò���
		void SetScissorTest(bool enable, const IntRect& rect);
		///  ���ü��ò���
		void SetScissorTest(bool enable, const Rect& rect = Rect::FULL, bool borderInclusive = true);


		ConstantBuffer* GetOrCreateConstantBuffer(unsigned bindingIndex, unsigned size);  //��ȡ���ߴ���һ��ConstantsBuffer����

		/// ����������ȾĿ�꣬��Ȼ�����������ӿ�
		void ResetRenderTargets();

		virtual void ResetRenderTarget(uint index);

		virtual GLRenderSurface* GetRenderTarget(uint index) const;  //ȡ��ָ������ȾĿ��

		virtual GLRenderSurface* GetDepthStencil() const;     //ȡ�����ģ�����

		virtual void ResetDepthStencil();

		virtual void CleanupRenderSurface(GLRenderSurface* surface); //������FBO������һ����Ⱦ��

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

		//����OpenGL������
		virtual void* GetMainWindow() override;

		virtual bool IsInitialized() override;

		virtual IDisplayContext* GetDisplayContext() const override;


		virtual bool BeginFrame() override;

		virtual void EndFrame() override;


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
		//�Ƿ�֧��OpenGL3����
		static bool GetGL3Support()
		{
			return m_gl3Support;
		}




	private:

		void CheckFeature();

		void Release(bool clearGpuObjects, bool closeWindow); //�ͷ�����GPU��Դ�ʹ���


		GraphicDriverType  m_driverType;
		ITextureMgr*   m_pTextureMgr;
		IImageMgr*     m_pImageMgr;
		ShaderMgr*     m_pShaderMgr;
		ShaderScriptMgr*   m_pShaderScriptMgr;
		GLRenderSystem*  m_renderSys;
		GLDisplayContext*  m_displayContext;

		static  bool   m_gl3Support;


		int            m_nTextureQuality;
		bool		   m_bAnisotropySupport;
		uint           m_nMaxTextureUnits;
		ImageTypeNameMap   m_imagetypeNames;

		bool m_bGL3Support;  //�Ƿ�֧��OpenGL3

		uint m_curBoundVBO;  //��ǰ��VBO
		uint m_curBoundUBO;  //��ǰUBO
		uint m_curBoundFBO;  //��ǰ��FBO
		uint m_sysFBO;      //ϵͳFBO
		uint m_VAO;          // VAO

		MutexEx  m_gpuObjMutex; //GPU���󻥳���
		//gpu �����
		std::unordered_map<std::string, GPUObject*>   m_gpuObjects;
		//�����ӵ�shaderProgames
		std::unordered_map<std::string, GLShaderProgram*>    m_shaderProgramDict;


		
		/// �������õ���ȾĿ��
		GLRenderSurface* m_renderTargets[MAX_RENDERTARGETS];
		/// ��ǰ��Ŀ�껺����
		GLRenderSurface* m_depthStencil;
		// �������, ����ģ��dx9�Ĺ��ܻ����Ⱦ����ͺ󱸻�����
		std::unordered_map<int,Texture2D*>   m_depthTextures;

		//���õĶ��㻺������
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];
		//UBO����������
		std::unordered_map<unsigned, SharedPtr<ConstantBuffer> > m_constantsBuffers;
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


		bool	 m_bColorWrite;        //��ɫ��д��
		bool	 m_bDepthWrite;		//��ȿ�д��
		uint	 m_stencilWriteMask; //ģ��д��λ����
		bool	 m_bScissorTest;      //���ò���
		bool	 m_bStencilTest;     //ģ����Կ�����־

		CullMode		m_cullmode;         //�޳�ģʽ
		FillMode		m_fillmode;		//���ģʽ
		CompareMode     m_depthTestMode;   //��Ȳ���ģʽ
		BlendMode		m_blendMode;		//alpha���ģʽ

		/// ���ò�������
		IntRect			m_scissorRect;    
		IntRect			m_viewport;   //�ӿ�����
		///  
		
		//��ǰ���õ�shader
		GLShaderVariation* m_vertexShader;
		GLShaderVariation* m_pixelShader;
		GLShaderVariation* m_geometryShader;
		GLShaderVariation* m_computeShader;
		GLShaderProgram* m_shaderProgram;

		bool  m_bIsInitialized;

	};
}