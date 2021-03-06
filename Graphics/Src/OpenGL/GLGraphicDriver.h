#pragma once
#include "Sapphire.h"
#include <Graphics.h>
#include "IGraphicDriver.h"
#include "Thread.h"
#include "Math/Rect.h"

#ifndef GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83f1
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83f2
#endif
#ifndef GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83f3
#endif
#ifndef GL_ETC1_RGB8_OES
#define GL_ETC1_RGB8_OES 0x8d64
#endif
#ifndef COMPRESSED_RGB_PVRTC_4BPPV1_IMG
#define COMPRESSED_RGB_PVRTC_4BPPV1_IMG 0x8c00
#endif
#ifndef COMPRESSED_RGB_PVRTC_2BPPV1_IMG
#define COMPRESSED_RGB_PVRTC_2BPPV1_IMG 0x8c01
#endif
#ifndef COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
#define COMPRESSED_RGBA_PVRTC_4BPPV1_IMG 0x8c02
#endif
#ifndef COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
#define COMPRESSED_RGBA_PVRTC_2BPPV1_IMG 0x8c03
#endif


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
	class GLShaderManager;
	class IRenderer;


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
		/// 当前FBO用到的读取缓冲区位数
		uint readBuffers;
		/// 当前FBO用到的绘制缓冲区位数
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


		

		virtual SharedPtr<IRenderer> GetRenderer(const StringHash& key) const;

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


		virtual IShaderVariation* GetShader(ShaderType type, const std::string& name, const std::string& defines = "") const;

		virtual IShaderVariation* GetShader(ShaderType type, const char* name, const char* defines) const;

		//獲取Shader，如果緩存裡沒有，同步加載
		virtual IShaderVariation*  GetShader(ShaderType type, const std::string& name, const std::string define = "") const;

		//设置当前渲染使用的Shader
		virtual void SetShaders(IShaderVariation* vs, IShaderVariation* ps, IShaderVariation* gs = NULL, IShaderVariation* cs = NULL);

		virtual IShaderVariation* GetVertexShader() const;
		virtual IShaderVariation* GetPixelShader() const;

		virtual const Path& GetShaderPath() const { return m_shaderResPath; }

		virtual void    SetShaderPath(std::string path);


		//设置一个float
		virtual void SetShaderParameter(StringHash param, float value);
		//设置一个float缓冲区
		virtual void SetShaderParameter(StringHash param, const float* data, unsigned count);
		//设置一个Vector2
		virtual void SetShaderParameter(StringHash param, const Vector2& vector);
		//设置一个Vector4
		virtual void SetShaderParameter(StringHash param, const Vector4& vector);
		//设置一个Vector3
		virtual void SetShaderParameter(StringHash param, const Vector3& vector);
		///设置一个3x3矩阵
		virtual void SetShaderParameter(StringHash param, const Matrix3x3& matrix);
		///设置一个4x3矩阵
		virtual void SetShaderParameter(StringHash param, const Matrix3x4& matrix);
		//设置一个4x4矩阵
		virtual void SetShaderParameter(StringHash param, const Matrix4x4& matrix);
		///设置一个Color
		virtual void SetShaderParameter(StringHash param, const Color& color);
		//是否有参数
		virtual bool HasShaderParameter(StringHash param);
		//检查参数组是否需要更新，如果不存在不做检查
		virtual bool NeedParameterUpdate(ShaderParameterGroup group, const void* source);
		//清理Shader参数源
		virtual void ClearParameterSources();
		///清理变换Shader参数源
		virtual void ClearTransformSources();
		///清理指定的shader参数源组
		virtual void ClearParameterSource(ShaderParameterGroup group);


		//绘制前的准备工作,渲染前调用，更新UBO和FBO
		virtual void PrepareDraw();
		///绘制非索引化的的几何体
		virtual void Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount);
		///绘制索引化的几何体
		virtual void Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);

		///重设缓存的渲染状态
		void ResetCachedState();

		//绑定一个纹理到指定的纹理单元
		//先激活对应的纹理单元
		//然后绑定纹理对象
		virtual void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //绑定VBO对象

		//绑定UBO对象
		void BindUBO(uint uHwUID);  //绑定UBO

		virtual bool  IsDeviceLost();

		virtual bool GetAnisotropySupport() const { return m_bAnisotropySupport; }
		//是否支持SRGB空间
		virtual bool GetSRGBSupport() const { return m_bsRGBSupport;}

		void MarkFBODirty();

		uint GetMaxAnisotropyLevels();

		static uint GetMaxBones();

		IntVector2 GetRenderTargetDimensions() const;

		virtual PixelFormat GetPixelFormat(ImageType eImgType);

		virtual PixelFormat GetPixelFormat(std::string szImageType);

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


		virtual int getTextureQuality() const { return m_nTextureQuality; }

		void setTextureQuality(int val) { m_nTextureQuality = val; }

		virtual IShaderProgram* GetShaderProgram() const;

		virtual IVertexBuffer* GetVertexBuffer(uint index) const;

		

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
		virtual  void SetColorWrite(bool enable);
		///  设置剔除模式
		virtual void SetCullMode(CullMode mode);
		///  设置填充模式
		virtual void SetFillMode(FillMode mode);
		///  设置深度测试
		virtual  void SetDepthTest(CompareMode mode);
		///	设置深度写入
		virtual void SetDepthWrite(bool enable);
		//	设置模板测试
		virtual  void SetStencilTest(bool enable, CompareMode mode = CMP_ALWAYS, StencilOp pass = OP_KEEP, StencilOp fail = OP_KEEP, StencilOp zFail = OP_KEEP,
			unsigned stencilRef = 0, unsigned compareMask = M_MAX_UNSIGNED, unsigned writeMask = M_MAX_UNSIGNED);
		///  设置Alpha混合模式
		virtual void SetBlendMode(BlendMode mode);
		///  设置剪裁测试
		virtual void SetScissorTest(bool enable, const IntRect& rect);
		///  设置剪裁测试
		virtual void SetScissorTest(bool enable, const Rect& rect = Rect::FULL, bool borderInclusive = true);

		



		ConstantBuffer* GetOrCreateConstantBuffer(unsigned bindingIndex, unsigned size);  //获取或者创建一个ConstantsBuffer对象

		/// 重置所有渲染目标，深度缓冲区表面和视口
		virtual void ResetRenderTargets();

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

		//默认的各项异性过滤dengj
		virtual unsigned GetTextureAnisotropy() const { return m_textureAnisotropy; }
		//返回默认纹理过滤模式
		virtual  TextureFilterMode GetDefaultTextureFilterMode() const { return m_eDefaultTextureFilterMode; }

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

		//获取当前帧图元数
		unsigned	GetNumPrimitives() const { return m_uNumPrimitives; }
		//获取当前帧的批次
		unsigned	GetNumBatches() const { return m_uNumBatches; }
		
	public:

		static int GetHWTextureWarpParam(TextureAddressMode mode);

		static int GetHWTextureFormat(PixelFormat eFormat);

		static int GetSWTextureFormat(PixelFormat eFormat);

		static uint GetHWTextureDataType(PixelFormat format);

		static GLenum  GetWarpMode(TextureAddressMode mode);


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

		void AddRenderer(const StringHash& key, SharedPtr<IRenderer> renderer);

		void CheckFeature();

		void Release(bool clearGpuObjects, bool closeWindow); //释放清理GPU资源和窗口


		GraphicDriverType  m_driverType;
		ITextureMgr*			m_pTextureMgr;
		IImageMgr*				m_pImageMgr;
		ShaderMgr*				m_pShaderMgr;
		GLShaderManager*		m_pGLShaderMgr;
		ShaderScriptMgr*   m_pShaderScriptMgr;
		GLRenderSystem*  m_renderSys;
		GLDisplayContext*  m_displayContext;

		std::unordered_map<StringHash, SharedPtr<IRenderer>, StringHashFunc, StringHashCMP> m_glRenderers;

		static  bool   m_gl3Support;


		int            m_nTextureQuality;
		bool		   m_bAnisotropySupport;
		int            m_textureAnisotropy;

		TextureFilterMode m_eDefaultTextureFilterMode;
		uint           m_nMaxTextureUnits;
		ImageTypeNameMap   m_imagetypeNames;

		bool m_bGL3Support;  //是否支持OpenGL3

		/////是否支持sRGB读
		bool m_bsRGBReadSupport;
		bool m_bsRGBWriteSupport;   //是否支持sRGB写
		bool m_bsRGBSupport;

		/// SRGB写标志
		bool m_bSRGBWrite;

		uint m_curBoundVBO;  //当前绑定VBO
		uint m_curBoundUBO;  //当前UBO
		uint m_curBoundFBO;  //当前绑定FBO
		uint m_sysFBO;      //系统FBO
		uint m_VAO;          // VAO

		MutexEx  m_gpuObjMutex; //GPU对象互斥锁
		//gpu 对象表
		std::unordered_map<std::string, GPUObject*>   m_gpuObjects;
		//已链接的shaderProgames
		std::unordered_map<std::string, SharedPtr<GLShaderProgram>>    m_shaderProgramDict;

		
		
		/// 当前在用的渲染目标   
		GLRenderSurface* m_renderTargets[MAX_RENDERTARGETS];
		/// 当前的目标缓冲区
		GLRenderSurface* m_depthStencil;
		// 深度纹理, 用来模拟dx9的功能混合渲染纹理和后备缓冲区
		std::unordered_map<int,Texture2D*>   m_depthTextures;

		//可用的顶点缓冲区数
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];
		//UBO缓冲区集合
		std::unordered_map<unsigned, SharedPtr<ConstantBuffer> > m_constantsBuffers;
		/// 当前绑定的constantBuffer,VertexShader和PixelShader各占一段，所以是两倍
		ConstantBuffer* m_currentConstantBuffers[MAX_SHADER_PARAMETER_GROUPS * 2];

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

		/// 脏的Constant buffers.
		std::vector<ConstantBuffer*> m_dirtyConstantBuffers;

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
		uint			m_stencilRefVal;    //模板测试的参考值
		CompareMode		m_stencilTestMode;	//模板测试模式
		uint			m_stencilCompareMask;    //模板比较的掩码
		StencilOp		m_stencilPassOp;		//模板测试通过的操作  
		StencilOp		m_stencilFailOp;		//模板测试失败的操作
		StencilOp		m_stencilZFailOp;		//通过模板测试，没通过深度测试的操作
		StencilOp		m_stencilZPassOp;		//通过深度测试或者没开启深度测试执行的操作

		/// 剪裁测试区域
		IntRect			m_scissorRect;    
		IntRect			m_viewport;   //视口区域
		///  
		///自定义剪裁平面
		bool			m_useCustomClipPlane;
		///当前自定义剪裁平面
		Vector4			m_customclipPlane;

		/// 当前帧的图元数
		unsigned m_uNumPrimitives;
		/// 当前帧的批次数
		unsigned m_uNumBatches;
		
		//当前再用的shader
		GLShaderVariation* m_vertexShader;
		GLShaderVariation* m_pixelShader;
		GLShaderVariation* m_geometryShader;
		GLShaderVariation* m_computeShader;
		GLShaderProgram* m_shaderProgram;
		Path		 m_shaderResPath;


		bool  m_bIsInitialized;

	};
}