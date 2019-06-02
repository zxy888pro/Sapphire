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
		RenderSurface* colorAttachments[MAX_RENDERTARGETS];
		/// 绑定的深度模板附件
		RenderSurface* depthAttachment;
		/// 读取缓冲区位数
		uint readBuffers;
		/// 绘制缓冲区位数
		uint drawBuffers;
	};


	//OpenGL 与硬件层访问驱动类。  
	//需抽出IGraphicDriver接口, 无关图形API
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
		//绘制前的准备工作
		virtual void PrepareDraw();

		//绑定一个纹理到指定的纹理单元
		//先激活对应的纹理单元
		//然后绑定纹理对象
		void BindTexture(ITexture* pTexture, TextureUnit unit);

		void BindVBO(uint uHwUID);  //绑定VBO对象

		//绑定UBO对象
		void BindUBO(uint uHwUID);  //绑定UBO

		bool  IsDeviceLost();

		bool GetAnisotropySupport(){ return m_bAnisotropySupport; }

		uint GetMaxAnisotropyLevels();

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

		virtual void  SetVertexBuffer(IVertexBuffer* vertexBuffer);
		//设置渲染目标
		virtual void SetRenderTarget(unsigned index, RenderSurface* renderTarget);

		virtual void SetRenderTarget(unsigned index, Texture2D* texture);

		virtual void SetDepthStencil(RenderSurface* depthStencil);

		virtual void SetDepthStencil(Texture2D* texture);

		virtual IIndexBuffer* GetIndexBuffer() const;

		virtual void SetIndexBuffer(IIndexBuffer* pIndexBuffer); //设置索引缓冲区，同时绑定它

		virtual void ResetRenderTarget(uint index);

		virtual RenderSurface* GetRenderTarget(uint index) const;  //取得指定的渲染目标

		virtual RenderSurface* GetDepthStencil() const;     //取得深度模板表面

		virtual void ResetDepthStencil();

		virtual void CleanupRenderSurface(RenderSurface* surface); //从所有FBO种清理一个渲染面

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

		uint m_curBoundVBO;  //当前绑定VBO
		uint m_curBoundUBO;  //当前UBO
		uint m_curBoundBO;  //当前绑定FBO
		uint m_sysFBO;      //系统FBO

		/// 所有在用的渲染目标
		RenderSurface* m_renderTargets[MAX_RENDERTARGETS];
		/// 当前的目标缓冲区
		RenderSurface* m_depthStencil;

		//可用的顶点缓冲区数
		IVertexBuffer* m_vertexBuffers[MAX_VERTEX_STREAMS];
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

		//当前再用的shader
		GLShaderVariation* m_vertexShader;
		GLShaderVariation* m_pixelShader;
		GLShaderVariation* m_geometryShader;
		GLShaderVariation* m_computeShader;
		GLShaderProgram* m_shaderProgram;


	};
}