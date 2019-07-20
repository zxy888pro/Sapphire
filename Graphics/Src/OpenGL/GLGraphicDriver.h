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


	///方便顶点更新的CPU端缓冲区
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

	/// FBO对象
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

		/// FBO对象句柄
		uint  fbo;
		/// 绑定的颜色附件纹理
		GLRenderSurface* colorAttachments[MAX_RENDERTARGETS];
		/// 绑定的深度模板附件
		GLRenderSurface* depthAttachment;
		/// 读取缓冲区位数
		uint readBuffers;
		/// 绘制缓冲区位数
		uint drawBuffers;
	};


	//OpenGL 与硬件层访问驱动类。  
	//需抽出IGraphicDriver接口, 无关图形API
	class SAPPHIRE_CLASS GLGraphicDriver : public IGraphicDriver
	{
		SAPPHIRE_OBJECT(GLGraphicDriver, IGraphicDriver)

	public:

		typedef std::unordered_map<std::string, ImageType>   ImageTypeNameMap;

		GLGraphicDriver(Core* pCore);
		virtual ~GLGraphicDriver();

		virtual void Init();

		virtual void Release();

		//设置显示模式，改变窗口或创建新窗口
		virtual bool SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable);

		/// 恢复GPU对象并重新初始化，需要已经打开的窗口
		void Restore();
		/// 窗口最大化
		void Maximize();
		/// 最小化窗口
		void Minimize();

		Sapphire::ITextureMgr* getTextureMgr() const { return m_pTextureMgr; }

		Sapphire::IImageMgr* getImageMgr() const { return m_pImageMgr; }

		Sapphire::ShaderMgr* getShaderMgr() const { return m_pShaderMgr; }

		Sapphire::ShaderScriptMgr*  getShaderScriptMgr() const { return m_pShaderScriptMgr; }
		//绘制前的准备工作,渲染前调用，更新UBO和FBO
		virtual void PrepareDraw();

		//绑定一个纹理到指定的纹理单元
		//先激活对应的纹理单元
		//然后绑定纹理对象
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //绑定VBO对象

		//绑定UBO对象
		void BindUBO(uint uHwUID);  //绑定UBO

		virtual bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

		IntVector2 GetRenderTargetDimensions() const;

		PixelFormat GetPixelFormat(ImageType eImgType);

		PixelFormat GetPixelFormat(std::string szImageType);

		//取得当前绑定的FBO
		virtual uint GetCurrentBoundFBO() const;
		//绑定FBO对象
		virtual void BindFrameBuffer(uint fbo);
		//绑定颜色附件
		virtual void BindColorAttachment(uint index, uint target, uint obj);
		//绑定深度附件
		virtual void BindDepthAttachment(uint object, bool isRenderBuffer);
		//绑定模板附件
		virtual void BindStencilAttachment(uint object, bool isRenderBuffer);

		virtual void DeleteFrameBuffer(uint fbo);
		//创建帧缓冲区
		virtual uint CreateFramebuffer();
		/// 清理所有的帧缓冲区，在销毁DisplayContext之前调用
		virtual void CleanFrameBuffers();
		/// 清理所有的渲染目标，深度缓冲区和模板缓冲区
		virtual void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);

		/// 添加一个跟踪的GPUObject对象，由GPUObject调用
		virtual void AddGPUObject(GPUObject*  gpuObj);
		/// 移除一个GPUObject对象，由GPUObject调用
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

		//设置多个顶点缓冲区
		virtual bool SetVertexBuffers(const std::vector<IVertexBuffer*>&  buffers, const std::vector<uint>& elememtMasks, uint instOffset = 0);
		//设置顶点缓冲区
		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer);
		//设置渲染目标
		virtual void SetRenderTarget(unsigned index, GLRenderSurface* renderTarget);
		//设置渲染目标
		virtual void SetRenderTarget(unsigned index, Texture2D* texture);
		//设置深度模板表面
		virtual void SetDepthStencil(GLRenderSurface* depthStencil);
		//设置深度模板表面
		virtual void SetDepthStencil(Texture2D* texture);

		virtual IIndexBuffer* GetIndexBuffer() const;

		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer); //设置索引缓冲区，同时绑定它

		virtual void SetViewport(const IntRect& rect);  //设置视口

		/// 设置颜色写入
		void SetColorWrite(bool enable);
		///  设置剔除模式
		virtual void SetCullMode(CullMode mode);
		///  设置填充模式
		virtual void SetFillMode(FillMode mode);
		///  设置深度测试
		void SetDepthTest(CompareMode mode);
		///	设置深度写入
		void SetDepthWrite(bool enable);
		///  设置Alpha混合模式
		void SetBlendMode(BlendMode mode);
		///  设置剪裁测试
		void SetScissorTest(bool enable, const IntRect& rect);
		///  设置剪裁测试
		void SetScissorTest(bool enable, const Rect& rect = Rect::FULL, bool borderInclusive = true);


		ConstantBuffer* GetOrCreateConstantBuffer(unsigned bindingIndex, unsigned size);  //获取或者创建一个ConstantsBuffer对象

		/// 重置所有渲染目标，深度缓冲区表面和视口
		void ResetRenderTargets();

		virtual void ResetRenderTarget(uint index);

		virtual GLRenderSurface* GetRenderTarget(uint index) const;  //取得指定的渲染目标

		virtual GLRenderSurface* GetDepthStencil() const;     //取得深度模板表面

		virtual void ResetDepthStencil();

		virtual void CleanupRenderSurface(GLRenderSurface* surface); //从所有FBO种清理一个渲染面

		//申请一块ScratchBuffer
		virtual void* ReserveScratchBuffer(ulong size);
		//释放一块ScratchBuffer
		virtual void FreeScratchBuffer(void* buffer);
		//清理太大ScratchBuffer
		virtual void CleanScratchBuffers();
		//获取GLES的深度模板格式
		virtual uint GetHWDepthStencilFormat() const { return m_glesHWDepthStencilFormat; }
		//检查特性支持
		virtual void CheckFeatureSupport();

		//检查OpenGL扩展
		virtual bool CheckExtension(const char* name);
		//返回shadowMap 深度纹理格式， 如果位0则不支持
		virtual uint GetHWShadowMapFormat() const { return m_shadowMapFormat; }

		virtual GraphicDriverType getDriverType() const override;

		//返回OpenGL主窗口
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
		//硬件Alpha格式
		static int GetHWAlphaFormat();
		//硬件明度格式
		static int GetHWLuminanceFormat();
		//硬件明度和alpha格式
		static int GetHWLuminanceAlphaFormat();
		//硬件RGB格式
		static int GetHWRGBFormat();
		//硬件RGBA格式
		static int GetHWRGBAFormat();
		//是否支持OpenGL3以上
		static bool GetGL3Support()
		{
			return m_gl3Support;
		}




	private:

		void CheckFeature();

		void Release(bool clearGpuObjects, bool closeWindow); //释放清理GPU资源和窗口


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

		bool m_bGL3Support;  //是否支持OpenGL3

		uint m_curBoundVBO;  //当前绑定VBO
		uint m_curBoundUBO;  //当前UBO
		uint m_curBoundFBO;  //当前绑定FBO
		uint m_sysFBO;      //系统FBO
		uint m_VAO;          // VAO

		MutexEx  m_gpuObjMutex; //GPU对象互斥锁
		//gpu 对象表
		std::unordered_map<std::string, GPUObject*>   m_gpuObjects;
		//已链接的shaderProgames
		std::unordered_map<std::string, GLShaderProgram*>    m_shaderProgramDict;


		
		/// 所有在用的渲染目标
		GLRenderSurface* m_renderTargets[MAX_RENDERTARGETS];
		/// 当前的目标缓冲区
		GLRenderSurface* m_depthStencil;
		// 深度纹理, 用来模拟dx9的功能混合渲染纹理和后备缓冲区
		std::unordered_map<int,Texture2D*>   m_depthTextures;

		//可用的顶点缓冲区数
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];
		//UBO缓冲区集合
		std::unordered_map<unsigned, SharedPtr<ConstantBuffer> > m_constantsBuffers;
		//使用的元素掩码
		uint m_elementMasks[MAX_VERTEX_STREAMS];
		//上一次使用的实例数据偏移
		uint m_lastInstOffset;

		bool m_fboDirty;  //FBO需要更新标志

		//每一个分辨率和格式的FBO表
		std::map<ulonglong, FrameBufferObject>  m_frameBuffers;  

		//当前使用的顶点属性掩码
		unsigned m_enabledAttributes;

		///当前使用indexBuffer
		IIndexBuffer* m_indexBuffer;

		//ScratchBuffers
		std::vector<ScratchBuffer> m_scratchBuffers;
		ulong m_maxScratchBufferRequest; //当前帧请求的ScratchBuffer最大值

		//gles硬件深度模板格式
		uint m_glesHWDepthStencilFormat;
		//gles硬件读取深度格式
		uint m_glesHWReadableDepthFormat;
		//ShadowMap 深度格式
		uint m_shadowMapFormat;
		//ShadowMap 24位深度格式
		uint m_hireShadowMapFormat;
		uint m_dummyColorFormat;


		bool	 m_bColorWrite;        //颜色可写入
		bool	 m_bDepthWrite;		//深度可写入
		uint	 m_stencilWriteMask; //模板写入位掩码
		bool	 m_bScissorTest;      //剪裁测试
		bool	 m_bStencilTest;     //模板测试开启标志

		CullMode		m_cullmode;         //剔除模式
		FillMode		m_fillmode;		//填充模式
		CompareMode     m_depthTestMode;   //深度测试模式
		BlendMode		m_blendMode;		//alpha混合模式

		/// 剪裁测试区域
		IntRect			m_scissorRect;    
		IntRect			m_viewport;   //视口区域
		///  
		
		//当前再用的shader
		GLShaderVariation* m_vertexShader;
		GLShaderVariation* m_pixelShader;
		GLShaderVariation* m_geometryShader;
		GLShaderVariation* m_computeShader;
		GLShaderProgram* m_shaderProgram;

		bool  m_bIsInitialized;

	};
}