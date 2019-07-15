#include "GLGraphicDriver.h"
#include <GraphicException.h>
#include "GLDisplayContext.h"
#include "IRenderSystem.h"
#include "Texture2D.h"
#include "TextureMgr.h"
#include "../ImageMgr.h"
#include "ShaderScriptMgr.h"
#include "ShaderMgr.h"
#include "IIndexBuffer.h"
#include "IVertexBuffer.h"
#include "IShaderVariation.h"
#include "GLShaderProgram.h"
#include "VertexBuffer.h"
#include "GLRenderSurface.h"
#include "GLRenderSystem.h"


namespace Sapphire
{
	// 重新OpenGL映射顶点属性,经常蒙皮修正要用到,要避免GLES2 蒙皮bug，gles设备只支持到8
	static const unsigned glVertexAttrIndex[] =
	{
		0, 1, 2, 3, 4, 8, 9, 5, 6, 7, 10, 11, 12
	};



	GLGraphicDriver::GLGraphicDriver(Core* pCore) :IGraphicDriver(pCore)
	{
		m_pTextureMgr = NULL;
		m_pImageMgr = NULL;
		m_nTextureQuality = QUALITY_HIGH;
		m_driverType = GRAPHICDRIVER_OPENGL;
		m_bGL3Support = false;
		m_displayContext = NULL;

	}

	GLGraphicDriver::~GLGraphicDriver()
	{

	}

	void GLGraphicDriver::Init()
	{
		

		m_pTextureMgr = new TextureMgr(m_pCore);
		m_pImageMgr = new ImageMgr();
		m_pShaderScriptMgr = new ShaderScriptMgr();
		m_imagetypeNames.clear();
		m_imagetypeNames[ENUM2STR(ImageType_Bmp_A8R8G8B8)] = ImageType_Bmp_A8R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_Bmp_R8G8B8)] = ImageType_Bmp_R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_Jpeg)] = ImageType_Jpeg;
		m_imagetypeNames[ENUM2STR(ImageType_Png)] = ImageType_Png;
		m_imagetypeNames[ENUM2STR(ImageType_RAW_R8G8B8)] = ImageType_RAW_R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_RAW_R8G8B8A8)] = ImageType_RAW_R8G8B8A8;
		m_imagetypeNames[ENUM2STR(ImageType_Tga32)] = ImageType_Tga32;
		m_imagetypeNames[ENUM2STR(ImageType_Tga24)] = ImageType_Tga24;
		m_displayContext = new GLDisplayContext(); //创建OpenGL窗口显示环境
		m_displayContext->Initialize(); //初始化OpenGL窗口显示环境
		CheckFeature();
		//初始化渲染系統接口
		IRenderSystem* pRenderSys = new GLRenderSystem(m_pCore,this);
		m_pCore->RegisterSubSystem<IRenderSystem>(pRenderSys, ESST_RENDERSYSTEM);

	}

	void GLGraphicDriver::Release()
	{
		safeDelete(m_pImageMgr);
		safeDelete(m_pTextureMgr);
		safeDelete(m_pShaderScriptMgr);

	}



	void GLGraphicDriver::PrepareDraw()
	{

	}

	void GLGraphicDriver::BindTexture(ITexture* pTexture, TextureUnit unit)
	{
		if (m_pTextureMgr)
		{
			m_pTextureMgr->SetTexture(pTexture, unit);
		}
		else
		{
			throw GraphicDriverException("TextureManager is Null!", GraphicDriverException::GDError_NullPointerError);
		}
	}

	void GLGraphicDriver::BindVBO(uint uHwUID)
	{
		if (m_curBoundVBO != uHwUID)
		{
			if (uHwUID)
			{
				glBindBuffer(GL_ARRAY_BUFFER, uHwUID);
				m_curBoundVBO = uHwUID;
			}
		}
	}

	void GLGraphicDriver::BindUBO(uint uHwUID)
	{

	}

	void GLGraphicDriver::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
	{
		if (m_indexBuffer == pIndexBuffer)
			return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer ? pIndexBuffer->GetUID() : 0);  //绑定索引缓冲区
		m_indexBuffer = pIndexBuffer;
	}

	Sapphire::ConstantBuffer* GLGraphicDriver::GetOrCreateConstantBuffer(unsigned bindingIndex, unsigned size)
	{
		unsigned key = (bindingIndex << 16) | size;
		std::unordered_map<unsigned, SharedPtr<ConstantBuffer> >::iterator i = m_constantsBuffers.find(key);
		if (i == m_constantsBuffers.end())
		{
			std::pair<unsigned, SharedPtr<ConstantBuffer>> pair = std::make_pair(key, SharedPtr<ConstantBuffer>(new ConstantBuffer(m_pCore)));
			m_constantsBuffers.insert(pair);
			pair.second->SetSize(size);
			return pair.second.Get();
		}
		else
		{
			return i->second.Get();
		}
	}

	void GLGraphicDriver::ResetRenderTarget(uint index)
	{
		SetRenderTarget(index, (GLRenderSurface*)NULL);
	}

	Sapphire::GLRenderSurface* GLGraphicDriver::GetRenderTarget(uint index) const
	{
		return index < MAX_RENDERTARGETS ? m_renderTargets[index] : 0;
	}

	Sapphire::GLRenderSurface* GLGraphicDriver::GetDepthStencil() const
	{
		return m_depthStencil;
	}

	void GLGraphicDriver::ResetDepthStencil()
	{
		SetDepthStencil((GLRenderSurface*)0);
	}

	void GLGraphicDriver::CleanupRenderSurface(GLRenderSurface* surface)
	{
		if (!surface)
			return;

		//刷新有等待更新的FBO
		PrepareDraw();

		uint curFbo = GetCurrentBoundFBO();
		std::map<ulonglong, FrameBufferObject>::iterator it;
		//遍历所有fbo用到的这个表面
		for (it = m_frameBuffers.begin(); it != m_frameBuffers.end(); ++it)
		{
			for (int i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (it->second.colorAttachments[i] == surface)
				{
					//不是当前在绑定的FBO
					if (curFbo != it->second.fbo)
					{
						//先绑定
						BindFrameBuffer(it->second.fbo);
						curFbo = it->second.fbo;
					}
					BindColorAttachment(i, GL_TEXTURE_2D, 0); //取消绑定
					it->second.colorAttachments[i] = 0;
					it->second.drawBuffers = M_MAX_UNSIGNED;
				}
			}
			if (it->second.depthAttachment == surface)
			{
				if (curFbo != it->second.fbo)
				{
					BindFrameBuffer(it->second.fbo);
					curFbo = it->second.fbo;
				}
				BindDepthAttachment(0, false);  //取消绑定
				BindStencilAttachment(0, false);   //取消绑定
				it->second.depthAttachment = 0;
			}
		}
		// 恢复之前绑定的FBO
		if (curFbo != GetCurrentBoundFBO())
			BindFrameBuffer(GetCurrentBoundFBO());

	}

	bool GLGraphicDriver::IsDeviceLost()
	{
		return false;
	}

	uint GLGraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //查询允许的各向异性数量
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniLevel);
		return (uint)maxAniLevel;
	}

	PixelFormat GLGraphicDriver::GetPixelFormat(ImageType eImgType)
	{
		switch (eImgType)
		{
		case Sapphire::ImageType_Bmp_R8G8B8:
			return PF_R8G8B8;
		case Sapphire::ImageType_Bmp_A8R8G8B8:
			return PF_R8G8B8A8;
		case Sapphire::ImageType_Jpeg:
			return PF_R8G8B8;
		case Sapphire::ImageType_Png:
			return PF_R8G8B8A8;
		case Sapphire::ImageType_Tga32:
			return PF_R8G8B8A8;
		case Sapphire::ImageType_Tga24:
			return PF_R8G8B8;
		case Sapphire::ImageType_RAW_R8G8B8A8:
			return PF_R8G8B8A8;
		case Sapphire::ImageType_RAW_R8G8B8:
			return PF_R8G8B8;
		case Sapphire::ImageType_Unknown:
			return PF_UNDEFINED;
		default:
			break;
		}
		return PF_UNDEFINED;
	}

	PixelFormat GLGraphicDriver::GetPixelFormat(std::string szImageType)
	{
		if (m_imagetypeNames.find(szImageType) != m_imagetypeNames.end())
		{
			return GetPixelFormat(m_imagetypeNames[szImageType]);
		}
		return PF_UNDEFINED;
	}

	uint GLGraphicDriver::GetCurrentBoundFBO() const
	{
		return m_curBoundBO;
	}

	void GLGraphicDriver::BindFrameBuffer(uint fbo)
	{
#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fbo);
		else
#endif
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	}

	Sapphire::IShaderProgram* GLGraphicDriver::GetShaderProgram() const
	{
		return m_shaderProgram;
	}

	Sapphire::IVertexBuffer* GLGraphicDriver::GetVertexBuffer(uint index) const
	{
		return index < MAX_VERTEX_STREAMS ? m_vertexBuffers[index] : 0;
	}

	Sapphire::IShaderVariation* GLGraphicDriver::GetShader(ShaderType type, const std::string& name, const std::string define /*= ""*/) const
	{
		std::string fullName = ShaderMgr::GetFileName(name, type);
		return NULL;
	}

 

	void GLGraphicDriver::SetShaders(IShaderVariation* vs, IShaderVariation* ps)
	{

	}

	Sapphire::IShaderVariation* GLGraphicDriver::GetVertexShader() const
	{
		return NULL;
	}

	Sapphire::IShaderVariation* GLGraphicDriver::GetPixelShader() const
	{
		return NULL;
	}

	void GLGraphicDriver::CleanupShaderPrograms(IShaderVariation* pShaderVariation)
	{

	}

	bool GLGraphicDriver::SetVertexBuffers(const std::vector<IVertexBuffer*>& buffers, const std::vector<uint>& elememtMasks, uint instOffset)
	{
		//同时处理的顶点流不能超过MAX_VERTEX_STREAMS
		if (buffers.size() > MAX_VERTEX_STREAMS)
		{
			SAPPHIRE_LOGERROR("Too many vertex buffers");
			return false;
		}
		//元素掩码数要和顶点流数匹配
		if (buffers.size() != elememtMasks.size())
		{
			SAPPHIRE_LOGERROR("Amount of element masks and vertex buffers does not match");
			return false;
		}

		bool changed = false;//是否有改变
		unsigned newAttributes = 0;

		for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
		{
			IVertexBuffer* buffer = 0;
			unsigned elementMask = 0;

			if (i < buffers.size() && buffers[i])
			{
				buffer = buffers[i];
				if (elememtMasks[i] == MASK_DEFAULT)
					elementMask = buffer->GetElementMask(); //獲取当前缓冲区的元素掩码
				else
					elementMask = buffer->GetElementMask() & elememtMasks[i]; //对当前缓冲区的掩码和设置的掩码进行位与
			}

			//如果当前缓冲区和元素掩码和设置的相同，跳过
			if (buffer == m_vertexBuffers[i] && elementMask == m_elementMasks[i] && instOffset == m_lastInstOffset && !changed)
			{
				newAttributes |= elementMask;
				continue;
			}

			//设置缓冲区
			m_vertexBuffers[i] = buffer;
			//设置缓冲区对应掩码
			m_elementMasks[i] = elementMask;
			changed = true;


			// 小心opengl缓冲区对象的丢失，一个已绑定的非0的缓冲区对象才可以访问顶点数据的CPU内存,无效指针可能导致崩溃
			if (!buffer || (!buffer->GetUID() && glIsBuffer(buffer->GetUID())))
				continue;

			//绑定这个缓冲区
			BindVBO(buffer->GetUID());
			//设置缓冲区对象
			unsigned vertexSize = buffer->GetVertexSize();

			for (unsigned j = 0; j < MAX_VERTEX_ELEMENTS; ++j)
			{
				unsigned attrIndex = glVertexAttrIndex[j];  //取得顶点属性索引
				unsigned elementBit = (unsigned)(1 << j); //依次取得元素位

				if (elementMask & elementBit)  //判断有没有该元素
				{
					newAttributes |= elementBit;   //新增加属性

					// 开启属性
					if ((m_enabledAttributes & elementBit) == 0)
					{
						glEnableVertexAttribArray(attrIndex);  //打开顶点属性数组索引,然后设置顶点属性
						m_enabledAttributes |= elementBit;    //更新当前已打开的属性
					}

					// 设置属性指针， 如果是矩阵的,要添加这个矩阵指针的实例偏移
					unsigned offset = j >= ELEMENT_INSTANCEMATRIX1 ? instOffset * vertexSize : 0;
					glVertexAttribPointer(attrIndex, VertexBuffer::elementComponents[j], VertexBuffer::elementType[j],
						(GLboolean)VertexBuffer::elementNormalize[j], vertexSize,
						reinterpret_cast<const GLvoid*>(buffer->GetElementOffset((VertexElement)j) + offset));
				}
			}
		}

		if (!changed) //有变更
			return true;

		m_lastInstOffset = instOffset;

		// 检查要关掉哪些顶点属性
		unsigned disableAttributes = m_enabledAttributes & (~newAttributes);
		unsigned disableIndex = 0;

		while (disableAttributes)
		{
			if (disableAttributes & 1)
			{
				glDisableVertexAttribArray(glVertexAttrIndex[disableIndex]);
				m_enabledAttributes &= ~(1 << disableIndex);
			}
			disableAttributes >>= 1;
			++disableIndex;
		}

		return true;
	}

	void GLGraphicDriver::SetVertexBuffer(IVertexBuffer* vertexBuffer)
	{
		//注意这里不是多线程安全的
		static std::vector<IVertexBuffer*> vertexBuffers(1);
		static std::vector<unsigned> elementMasks(1);
		vertexBuffers[0] = vertexBuffer;
		elementMasks[0] = MASK_DEFAULT;

		SetVertexBuffers(vertexBuffers, elementMasks);
	}

	void GLGraphicDriver::SetRenderTarget(unsigned index, GLRenderSurface* renderTarget)
	{
		if (index >= MAX_RENDERTARGETS)
			return;

		if (renderTarget != m_renderTargets[index])
		{
			m_renderTargets[index] = renderTarget;

			// 如果渲染目标是一个已经绑定的纹理，用备份纹理暂时替换或者置空
			if (renderTarget)
			{
				ITexture* parentTexture = renderTarget->GetParentTexture();

				for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
				{

					if (m_pTextureMgr->GetTexture((TextureUnit)i) == parentTexture)
						m_pTextureMgr->SetTexture(m_pTextureMgr->GetTexture((TextureUnit)i)->getBackupTexture(), (TextureUnit)i);
				}
			}
			//fbo需要更新
			m_fboDirty = true;
		}
	}

	void GLGraphicDriver::SetRenderTarget(unsigned index, Texture2D* texture)
	{
		/*RenderSurface* renderTarget = 0;
		if (texture)
		renderTarget = texture->GetRenderSurface();

		SetRenderTarget(index, renderTarget);*/
	}

	void GLGraphicDriver::SetDepthStencil(GLRenderSurface* depthStencil)
	{

	}

	void GLGraphicDriver::SetDepthStencil(Texture2D* texture)
	{

	}

	Sapphire::IIndexBuffer* GLGraphicDriver::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}


	void* GLGraphicDriver::ReserveScratchBuffer(ulong size)
	{
		if (!size)
			return NULL;
		if (size > m_maxScratchBufferRequest)
		{
			m_maxScratchBufferRequest = size;
		}
		for (std::vector<ScratchBuffer>::iterator it = m_scratchBuffers.begin(); it != m_scratchBuffers.end(); ++it)
		{
			if (!it->m_bReserved && it->m_size >= size)
			{
				it->m_bReserved = true;
				return it->data_.Get();
			}
		}
		for (std::vector<ScratchBuffer>::iterator it = m_scratchBuffers.begin(); it != m_scratchBuffers.end(); ++it)
		{
			if (!it->m_bReserved)
			{
				//没有找到已分配足够空间的，找到一个未使用的重新分配
				it->data_ = new byte[size];
				it->m_bReserved = true;
				it->m_size = size;
				return it->data_.Get();
			}
		}
		//还是空余的，创建新的
		ScratchBuffer newBuffer;
		newBuffer.data_ = new unsigned char[size];
		newBuffer.m_size = size;
		newBuffer.m_bReserved = true;
		m_scratchBuffers.push_back(newBuffer);
		return newBuffer.data_.Get();
	}

	void GLGraphicDriver::FreeScratchBuffer(void* buffer)
	{
		if (!buffer)
			return;
		for (std::vector<ScratchBuffer>::iterator it = m_scratchBuffers.begin(); it != m_scratchBuffers.end(); ++it)
		{
			if (it->m_bReserved && it->data_.Get() == buffer)
			{
				it->m_bReserved = false;
				return;
			}
		}
		SAPPHIRE_LOGWARNING("FreeScratchBuffer Failed! Scratch Buffer not found!");
	}

	void GLGraphicDriver::CleanScratchBuffers()
	{
		for (std::vector<ScratchBuffer>::iterator it = m_scratchBuffers.begin(); it != m_scratchBuffers.end(); ++it)
		{
			if (!it->m_bReserved && it->m_size > m_maxScratchBufferRequest * 2)
			{
				it->data_ = m_maxScratchBufferRequest > 0 ? new unsigned char[m_maxScratchBufferRequest] : 0;
				it->m_size = m_maxScratchBufferRequest;
			}
		}
	}

	void GLGraphicDriver::CheckFeatureSupport()
	{

	}

	bool GLGraphicDriver::CheckExtension(const char* name)
	{
		std::string extensions = (const char*)glGetString(GL_EXTENSIONS);

		return extensions.find(name) != std::string::npos;
	}

	Sapphire::GraphicDriverType GLGraphicDriver::getDriverType() const
	{
		return m_driverType;
	}

	void* GLGraphicDriver::GetMainWindow()
	{
		return NULL;
	}

	bool GLGraphicDriver::IsInitialized()
	{
		return false;
	}

	Sapphire::IDisplayContext* GLGraphicDriver::GetDisplayContext() const
	{
		throw std::logic_error("The method or operation is not implemented.");
	}

	bool GLGraphicDriver::BeginFrame()
	{
		return true;
	}

	void GLGraphicDriver::EndFrame()
	{
		
	}

	int GLGraphicDriver::GetHWTextureWarpParam(TextureAddressMode mode)
	{
		switch (mode)
		{
		case Sapphire::ADDRESS_REPEAT:
			return GL_REPEAT;
			break;
		case Sapphire::ADDRESS_MIRROR:
			return GL_MIRRORED_REPEAT;
			break;
		case Sapphire::ADDRESS_CLAMP:
			return GL_CLAMP;
			break;
		case Sapphire::ADDRESS_BORDER:
			return GL_CLAMP_TO_BORDER;
			break;
		default:
			return GL_REPEAT;
			break;
		}
	}


	int GLGraphicDriver::GetHWTextureFormat(PixelFormat eFormat)
	{
		switch (eFormat)
		{
		case Sapphire::PF_L8:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_L16:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_A8:
			return GL_ALPHA;
			break;
		case Sapphire::PF_A4L4:
			return GL_LUMINANCE_ALPHA;
			break;
		case Sapphire::PF_R8G8B8A8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8R8G8B8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8B8G8R8:
			return GL_RGBA;
			break;
		case Sapphire::PF_R8G8B8:
			return GL_RGB;
			break;
		case Sapphire::PF_B8G8R8:
			return GL_RGB;
			break;
		case Sapphire::PF_R8:
			return GL_LUMINANCE;
			break;
		default:
			return GL_RGBA;
			break;
		}
	}

	int GLGraphicDriver::GetSWTextureFormat(PixelFormat eFormat)
	{
		switch (eFormat)
		{
		case Sapphire::PF_L8:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_L16:
			return GL_LUMINANCE;
			break;
		case Sapphire::PF_A8:
			return GL_ALPHA;
			break;
		case Sapphire::PF_A4L4:
			return GL_LUMINANCE_ALPHA;
			break;
		case Sapphire::PF_R8G8B8A8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8R8G8B8:
			return GL_RGBA;
			break;
		case Sapphire::PF_A8B8G8R8:
			return GL_RGBA;
			break;
		case Sapphire::PF_R8G8B8:
			return GL_RGB;
			break;
		case Sapphire::PF_B8G8R8:
			return GL_RGB;
			break;
		case Sapphire::PF_R8:
			return GL_LUMINANCE;
			break;
		default:
			return GL_RGBA;
			break;
		}
	}


	uint GLGraphicDriver::GetHWTextureDataType(PixelFormat ePformat)
	{
		uint format = GetHWTextureFormat(ePformat);

#ifndef GL_ES_VERSION_2_0
		if (format == GL_DEPTH24_STENCIL8_EXT)
			return GL_UNSIGNED_INT_24_8_EXT;
		else if (format == GL_RG16 || format == GL_RGBA16)
			return GL_UNSIGNED_SHORT;
		else if (format == GL_RGBA32F_ARB || format == GL_RG32F || format == GL_R32F)
			return GL_FLOAT;
		else if (format == GL_RGBA16F_ARB || format == GL_RG16F || format == GL_R16F)
			return GL_HALF_FLOAT_ARB;
		else
			return GL_UNSIGNED_BYTE;
#else
		if (format == GL_DEPTH_COMPONENT || format == GL_DEPTH_COMPONENT24_OES)
			return GL_UNSIGNED_INT;
		else if (format == GL_DEPTH_COMPONENT16)
			return GL_UNSIGNED_SHORT;
		else
			return GL_UNSIGNED_BYTE;
#endif
	}

	int GLGraphicDriver::GetHWAlphaFormat()
	{
		return GL_ALPHA;
	}

	int GLGraphicDriver::GetHWLuminanceFormat()
	{
		return GL_LUMINANCE;
	}

	int GLGraphicDriver::GetHWLuminanceAlphaFormat()
	{
		return GL_LUMINANCE_ALPHA;
	}

	int GLGraphicDriver::GetHWRGBFormat()
	{
		return GL_RGB;
	}

	int GLGraphicDriver::GetHWRGBAFormat()
	{
		return GL_RGBA;
	}

	void GLGraphicDriver::CheckFeature()
	{
		m_bAnisotropySupport = GLEW_EXT_texture_filter_anisotropic != 0;
		m_nMaxTextureUnits = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;

#ifndef GL_ES_VERSION_2_0
		m_glesHWDepthStencilFormat = GL_DEPTH24_STENCIL8_EXT; //默认24位深度/8位模板
		m_glesHWDepthStencilFormat = 0;
#else
		if (CheckExtension("GL_OES_depth24"))        //OpenGLES 2.0 特殊格式
			m_glesHWDepthStencilFormat = GL_DEPTH_COMPONENT24_OES;
		if (CheckExtension("GL_OES_packed_depth_stencil"))
			m_glesHWDepthStencilFormat = GL_DEPTH24_STENCIL8_OES;
		//#ifdef __EMSCRIPTEN__
#ifdef __WEBGL__
		if (!CheckExtension("WEBGL_depth_texture"))
#else
		if (!CheckExtension("GL_OES_depth_texture"))
#endif
		{
			m_shadowMapFormat = 0;
			m_hireShadowMapFormat = 0;
			m_glesHWReadableDepthFormat = 0;
		}
		else
		{
#ifdef IOS
			// iOS hack: 不支持深度渲染缓冲区，用深度纹理替代
			m_glesHWDepthStencilFormat = GL_DEPTH_COMPONENT;
#endif
			m_shadowMapFormat = GL_DEPTH_COMPONENT;
			m_hireShadowMapFormat = 0;
			// WebGL 的shadow map没有一个附件的dummy color texture渲染的非常慢
			//#ifdef __EMSCRIPTEN__
#ifdef __WEBGL__
			m_dummyColorFormat = GetHWRGBAFormat();
#endif
		}
#endif

	}

	void GLGraphicDriver::BindColorAttachment(uint index, uint target, uint obj)
	{
#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + index, target, obj, 0);
		else
#endif
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, obj, 0);
	}

	void GLGraphicDriver::BindDepthAttachment(uint object, bool isRenderBuffer)
	{
		if (!object)
			isRenderBuffer = false;

#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
		{
			if (!isRenderBuffer)  //是纹理还是渲染缓冲区
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, object, 0);
			else
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object);
		}
		else
#endif
		{
			if (!isRenderBuffer)  //是纹理还是渲染缓冲区
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, object, 0);
			else
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, object);
		}
	}

	void GLGraphicDriver::BindStencilAttachment(uint object, bool isRenderBuffer)
	{
		if (!object)
			isRenderBuffer = false;

#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
		{
			if (!isRenderBuffer)
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_TEXTURE_2D, object, 0);
			else
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object);
		}
		else
#endif
		{
			if (!isRenderBuffer)
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, object, 0);
			else
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, object);
		}
	}

	void GLGraphicDriver::DeleteFrameBuffer(uint fbo)
	{
#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
			glDeleteFramebuffersEXT(1, &fbo);
		else
#endif
			glDeleteFramebuffers(1, &fbo);
	}

	uint GLGraphicDriver::CreateFramebuffer()
	{
		unsigned newFbo = 0;
#ifndef GL_ES_VERSION_2_0
		if (!m_bGL3Support)
			glGenFramebuffersEXT(1, &newFbo);
		else
#endif
			glGenFramebuffers(1, &newFbo);
		return newFbo;
	}

}