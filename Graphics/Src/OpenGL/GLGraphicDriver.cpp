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
	// ����OpenGLӳ�䶥������,������Ƥ����Ҫ�õ�,Ҫ����GLES2 ��Ƥbug��gles�豸ֻ֧�ֵ�8
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
		m_displayContext = new GLDisplayContext(); //����OpenGL������ʾ����
		m_displayContext->Initialize(); //��ʼ��OpenGL������ʾ����
		CheckFeature();
		//��ʼ����Ⱦϵ�y�ӿ�
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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer ? pIndexBuffer->GetUID() : 0);  //������������
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

		//ˢ���еȴ����µ�FBO
		PrepareDraw();

		uint curFbo = GetCurrentBoundFBO();
		std::map<ulonglong, FrameBufferObject>::iterator it;
		//��������fbo�õ����������
		for (it = m_frameBuffers.begin(); it != m_frameBuffers.end(); ++it)
		{
			for (int i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (it->second.colorAttachments[i] == surface)
				{
					//���ǵ�ǰ�ڰ󶨵�FBO
					if (curFbo != it->second.fbo)
					{
						//�Ȱ�
						BindFrameBuffer(it->second.fbo);
						curFbo = it->second.fbo;
					}
					BindColorAttachment(i, GL_TEXTURE_2D, 0); //ȡ����
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
				BindDepthAttachment(0, false);  //ȡ����
				BindStencilAttachment(0, false);   //ȡ����
				it->second.depthAttachment = 0;
			}
		}
		// �ָ�֮ǰ�󶨵�FBO
		if (curFbo != GetCurrentBoundFBO())
			BindFrameBuffer(GetCurrentBoundFBO());

	}

	bool GLGraphicDriver::IsDeviceLost()
	{
		return false;
	}

	uint GLGraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //��ѯ����ĸ�����������
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
		//ͬʱ����Ķ��������ܳ���MAX_VERTEX_STREAMS
		if (buffers.size() > MAX_VERTEX_STREAMS)
		{
			SAPPHIRE_LOGERROR("Too many vertex buffers");
			return false;
		}
		//Ԫ��������Ҫ�Ͷ�������ƥ��
		if (buffers.size() != elememtMasks.size())
		{
			SAPPHIRE_LOGERROR("Amount of element masks and vertex buffers does not match");
			return false;
		}

		bool changed = false;//�Ƿ��иı�
		unsigned newAttributes = 0;

		for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
		{
			IVertexBuffer* buffer = 0;
			unsigned elementMask = 0;

			if (i < buffers.size() && buffers[i])
			{
				buffer = buffers[i];
				if (elememtMasks[i] == MASK_DEFAULT)
					elementMask = buffer->GetElementMask(); //�@ȡ��ǰ��������Ԫ������
				else
					elementMask = buffer->GetElementMask() & elememtMasks[i]; //�Ե�ǰ����������������õ��������λ��
			}

			//�����ǰ��������Ԫ����������õ���ͬ������
			if (buffer == m_vertexBuffers[i] && elementMask == m_elementMasks[i] && instOffset == m_lastInstOffset && !changed)
			{
				newAttributes |= elementMask;
				continue;
			}

			//���û�����
			m_vertexBuffers[i] = buffer;
			//���û�������Ӧ����
			m_elementMasks[i] = elementMask;
			changed = true;


			// С��opengl����������Ķ�ʧ��һ���Ѱ󶨵ķ�0�Ļ���������ſ��Է��ʶ������ݵ�CPU�ڴ�,��Чָ����ܵ��±���
			if (!buffer || (!buffer->GetUID() && glIsBuffer(buffer->GetUID())))
				continue;

			//�����������
			BindVBO(buffer->GetUID());
			//���û���������
			unsigned vertexSize = buffer->GetVertexSize();

			for (unsigned j = 0; j < MAX_VERTEX_ELEMENTS; ++j)
			{
				unsigned attrIndex = glVertexAttrIndex[j];  //ȡ�ö�����������
				unsigned elementBit = (unsigned)(1 << j); //����ȡ��Ԫ��λ

				if (elementMask & elementBit)  //�ж���û�и�Ԫ��
				{
					newAttributes |= elementBit;   //����������

					// ��������
					if ((m_enabledAttributes & elementBit) == 0)
					{
						glEnableVertexAttribArray(attrIndex);  //�򿪶���������������,Ȼ�����ö�������
						m_enabledAttributes |= elementBit;    //���µ�ǰ�Ѵ򿪵�����
					}

					// ��������ָ�룬 ����Ǿ����,Ҫ����������ָ���ʵ��ƫ��
					unsigned offset = j >= ELEMENT_INSTANCEMATRIX1 ? instOffset * vertexSize : 0;
					glVertexAttribPointer(attrIndex, VertexBuffer::elementComponents[j], VertexBuffer::elementType[j],
						(GLboolean)VertexBuffer::elementNormalize[j], vertexSize,
						reinterpret_cast<const GLvoid*>(buffer->GetElementOffset((VertexElement)j) + offset));
				}
			}
		}

		if (!changed) //�б��
			return true;

		m_lastInstOffset = instOffset;

		// ���Ҫ�ص���Щ��������
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
		//ע�����ﲻ�Ƕ��̰߳�ȫ��
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

			// �����ȾĿ����һ���Ѿ��󶨵������ñ���������ʱ�滻�����ÿ�
			if (renderTarget)
			{
				ITexture* parentTexture = renderTarget->GetParentTexture();

				for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
				{

					if (m_pTextureMgr->GetTexture((TextureUnit)i) == parentTexture)
						m_pTextureMgr->SetTexture(m_pTextureMgr->GetTexture((TextureUnit)i)->getBackupTexture(), (TextureUnit)i);
				}
			}
			//fbo��Ҫ����
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
				//û���ҵ��ѷ����㹻�ռ�ģ��ҵ�һ��δʹ�õ����·���
				it->data_ = new byte[size];
				it->m_bReserved = true;
				it->m_size = size;
				return it->data_.Get();
			}
		}
		//���ǿ���ģ������µ�
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
		m_glesHWDepthStencilFormat = GL_DEPTH24_STENCIL8_EXT; //Ĭ��24λ���/8λģ��
		m_glesHWDepthStencilFormat = 0;
#else
		if (CheckExtension("GL_OES_depth24"))        //OpenGLES 2.0 �����ʽ
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
			// iOS hack: ��֧�������Ⱦ��������������������
			m_glesHWDepthStencilFormat = GL_DEPTH_COMPONENT;
#endif
			m_shadowMapFormat = GL_DEPTH_COMPONENT;
			m_hireShadowMapFormat = 0;
			// WebGL ��shadow mapû��һ��������dummy color texture��Ⱦ�ķǳ���
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
			if (!isRenderBuffer)  //����������Ⱦ������
				glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, object, 0);
			else
				glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, object);
		}
		else
#endif
		{
			if (!isRenderBuffer)  //����������Ⱦ������
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