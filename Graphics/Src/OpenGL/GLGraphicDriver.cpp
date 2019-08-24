#include "Variant.h"
#include "Math/Rect.h"
#include "ResourceCache.h"
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
#include "GLShader.h"
#include "GLShaderManager.h"
#include "GLUIRenderer.h"




namespace Sapphire
{
	// ����OpenGLӳ�䶥������,������Ƥ����Ҫ�õ�,Ҫ����GLES2 ��Ƥbug��gles�豸ֻ֧�ֵ�8
	static const unsigned glVertexAttrIndex[] =
	{
		0, 1, 2, 3, 4, 8, 9, 5, 6, 7, 10, 11, 12
	};

	static const unsigned glCmpFunc[] =
	{
		GL_ALWAYS,
		GL_EQUAL,
		GL_NOTEQUAL,
		GL_LESS,
		GL_LEQUAL,
		GL_GREATER,
		GL_GEQUAL
	};

	static const unsigned glSrcBlend[] =
	{
		GL_ONE,
		GL_ONE,
		GL_DST_COLOR,
		GL_SRC_ALPHA,
		GL_SRC_ALPHA,
		GL_ONE,
		GL_ONE_MINUS_DST_ALPHA,
		GL_ONE,
		GL_SRC_ALPHA
	};

	static const unsigned glDestBlend[] =
	{
		GL_ZERO,
		GL_ONE,
		GL_ZERO,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_ONE,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE,
		GL_ONE
	};

	static const unsigned glBlendOp[] =
	{
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_ADD,
		GL_FUNC_REVERSE_SUBTRACT,
		GL_FUNC_REVERSE_SUBTRACT
	};

#ifndef GL_ES_VERSION_2_0

	static const unsigned glFillMode[] =
	{
		GL_FILL,
		GL_LINE,
		GL_POINT
	};

	static const unsigned glStencilOps[] =
	{
		GL_KEEP,
		GL_ZERO,
		GL_REPLACE,
		GL_INCR_WRAP,
		GL_DECR_WRAP
	};

#endif // !GL_ES_VERSION_2_0


	static GLenum glWrapModes[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE,
#ifndef GL_ES_VERSION_2_0
		GL_CLAMP
#else
		GL_CLAMP_TO_EDGE
#endif
	};

#ifndef GL_ES_VERSION_2_0
	static GLenum gl3WrapModes[] =
	{
		GL_REPEAT,
		GL_MIRRORED_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER
	};
#endif


	//ȡ�Ķ�ӦOpenGL��ͼԪ����ͼԪ����
	void GetGLPrimitiveType(unsigned elementCount, PrimitiveType type, unsigned& primitiveCount, GLenum& glPrimitiveType)
	{
		switch (type)
		{
		case TRIANGLE_LIST:
			primitiveCount = elementCount / 3;   //������ ÿ��������һ��������
			glPrimitiveType = GL_TRIANGLES;
			break;

		case LINE_LIST:
			primitiveCount = elementCount / 2;     //�߶�  ÿ2������һ���߶�
			glPrimitiveType = GL_LINES;
			break;

		case POINT_LIST:
			primitiveCount = elementCount;    //��   ÿ������һ����
			glPrimitiveType = GL_POINTS;
			break;

		case TRIANGLE_STRIP:                 //�����δ�     ��������=������-2
			primitiveCount = elementCount - 2;
			glPrimitiveType = GL_TRIANGLE_STRIP;
			break;

		case LINE_STRIP:
			primitiveCount = elementCount - 1;
			glPrimitiveType = GL_LINE_STRIP;
			break;

		case TRIANGLE_FAN:
			primitiveCount = elementCount - 2;
			glPrimitiveType = GL_TRIANGLE_FAN;
			break;
		}
	}

	GLGraphicDriver::GLGraphicDriver(Core* pCore) :IGraphicDriver(pCore)
	{
		m_bIsInitialized = false;
		m_pTextureMgr = NULL;
		m_pImageMgr = NULL;
		m_nTextureQuality = QUALITY_HIGH;
		m_driverType = GRAPHICDRIVER_OPENGL;
		m_bGL3Support = false;
		m_displayContext = NULL;
		m_pTextureMgr = new TextureMgr(m_pCore);
		m_pImageMgr = new ImageMgr();
		m_pShaderScriptMgr = new ShaderScriptMgr();
		m_pGLShaderMgr = new GLShaderManager(pCore, this);
		m_displayContext = new GLDisplayContext(); //����OpenGL������ʾ����
		m_textureAnisotropy = 0;
		m_eDefaultTextureFilterMode = FILTER_BILINEAR;	
		m_shaderProgram = NULL;
		m_curBoundFBO = 0;
		m_curBoundVBO = 0;
		m_curBoundUBO = 0;
		m_vertexShader = NULL;
		m_pixelShader = NULL;
		m_VAO = 0;
		m_indexBuffer = NULL;
		m_viewport = IntRect(0, 0, 0, 0);
		m_blendMode = BLEND_REPLACE;
		m_cullmode = CULL_NONE;
		m_stencilFailOp = OP_KEEP;
		m_stencilPassOp = OP_KEEP;
		m_stencilRefVal = 0;
		m_stencilTestMode = CMP_ALWAYS;
		m_stencilZFailOp = OP_KEEP;
		m_stencilZPassOp = OP_KEEP;
		m_stencilCompareMask = M_MAX_UNSIGNED;
		m_stencilWriteMask = M_MAX_UNSIGNED;
		m_useCustomClipPlane = false;
		m_bsRGBReadSupport = false;
		m_bsRGBWriteSupport = false;
		m_bsRGBSupport = false;
		m_driverType = GRAPHICDRIVER_OPENGL;
		m_depthTestMode = CMP_ALWAYS;
		m_bDepthWrite = false;
		m_fillmode = FillMode::FILL_SOLID;
		m_scissorRect = IntRect::ZERO;
		m_bScissorTest = false;
		m_bColorWrite = true;
		m_depthStencil = 0;
		for (unsigned i = 0; i < MAX_VERTEX_STREAMS; ++i)
		{
			m_vertexBuffers[i] = 0;
			m_elementMasks[i] = 0;
			 
		}
		for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			m_renderTargets[i] = 0;

		for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS * 2; ++i)
			m_currentConstantBuffers[i] = 0;

	}

	GLGraphicDriver::~GLGraphicDriver()
	{
		Release();
		safeDelete(m_pImageMgr);
		safeDelete(m_pTextureMgr);
		safeDelete(m_pShaderScriptMgr);
	}

	void GLGraphicDriver::AddRenderer(const StringHash& key, SharedPtr<IRenderer> renderer)
	{
		if (renderer.NotNull())
			m_glRenderers[key] = renderer;
	}

	Sapphire::SharedPtr<Sapphire::IRenderer> GLGraphicDriver::GetRenderer(const StringHash& key) const
	{
		auto it = m_glRenderers.find(key);
		if (it != m_glRenderers.end())
		{
			return it->second;
		}
		return SharedPtr<IRenderer>();
	}

	void GLGraphicDriver::Init()
	{
		
		m_imagetypeNames.clear();
		m_imagetypeNames[ENUM2STR(ImageType_Bmp_A8R8G8B8)] = ImageType_Bmp_A8R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_Bmp_R8G8B8)] = ImageType_Bmp_R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_Jpeg)] = ImageType_Jpeg;
		m_imagetypeNames[ENUM2STR(ImageType_Png)] = ImageType_Png;
		m_imagetypeNames[ENUM2STR(ImageType_RAW_R8G8B8)] = ImageType_RAW_R8G8B8;
		m_imagetypeNames[ENUM2STR(ImageType_RAW_R8G8B8A8)] = ImageType_RAW_R8G8B8A8;
		m_imagetypeNames[ENUM2STR(ImageType_Tga32)] = ImageType_Tga32;
		m_imagetypeNames[ENUM2STR(ImageType_Tga24)] = ImageType_Tga24;
		m_displayContext->Initialize(); //��ʼ��OpenGL������ʾ����
		CheckFeature();
		//��ʼ����Ⱦϵ�y�ӿ�
		IRenderSystem* pRenderSys = new GLRenderSystem(m_pCore,this);
		pRenderSys->Initialize();
		m_pCore->RegisterSubSystem<IRenderSystem>(pRenderSys, ESST_RENDERSYSTEM);

		//����UI��Ⱦ��
		SharedPtr<IRenderer> renderer = SharedPtr<IRenderer>(new GLUIRenderer(m_pCore,this));
		AddRenderer(RENDERER_UI, renderer);

		m_bIsInitialized = true;

	}

	void GLGraphicDriver::Release()
	{
		if (!m_bIsInitialized)
			return;

		Release(true, true);

	}



	void GLGraphicDriver::Release(bool clearGpuObjects, bool closeWindow)
	{
		if (!GetMainWindow())
			return;

		{
			ResGuard<MutexEx> resGuard(m_gpuObjMutex);

			if (clearGpuObjects)
			{
				//�ͷ����еĴ��ڵ�GPU����
				//ShaderProgramҲ��Ϊһ��GPU���������������Ǳ��������ʱ���б��޸�
				m_shaderProgramDict.clear();

				std::unordered_map<std::string, GPUObject*>::iterator it = m_gpuObjects.begin();
				for (; it != m_gpuObjects.end(); ++it)
				{
					it->second->Release();
				}
				m_gpuObjects.clear();
			}
			else
			{
				//Ҫ���´���������ʹ��GpuObject����ʧ
				std::unordered_map<std::string, GPUObject*>::iterator it = m_gpuObjects.begin();
				for (; it != m_gpuObjects.end(); ++it)
				{
					it->second->OnDeviceLost();
				}
				//�������shaderProgram
				m_shaderProgramDict.clear();
				

				FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_DEVICELOST, NULL);
			}
			
		}

		CleanFrameBuffers();
		m_depthTextures.clear();

		if (m_displayContext)
		{
			//��ֹ��ʾ����
			m_displayContext->Terminate();
		}
		//���ٴ���
		if (closeWindow)
		{
			m_displayContext->CloseWindow();
		}
		
	}

	bool GLGraphicDriver::m_gl3Support;


	bool GLGraphicDriver::SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable)
	{

		if (!width || !height) //���������зǷ�
		{
			if (bFullScreen) //ȫ��Ļ�Ļ�����ȡ�õ�ǰ��ʾ���ķֱ���
			{
				DisplayMode mode;
				m_displayContext->GetCurrentDisplayMode(mode);
				width = mode.width;
				height = mode.height;
			}
			else
			{
				width = 1024;
				height = 768;
			}
		}
		//�Ѳ����������Ƶ�1~16֮��
		multiSample = MathHelper::Clamp(multiSample, 1, 16);

		{
			//��ΪǶ�봰��ʱ���ܹر�
			if (!m_displayContext->GetExternalWindow())
			{
				//DisplayContext�رմ��ڵĴ��ڣ������GpuObject Lost
				Release(false, true);
				//�����ٴ�������
				m_displayContext->CreateRenderWindow(x, y, width, height, bFullScreen, multiSample,false, bVsync);
				 
			}
			if (!m_displayContext->GetWindow())
				return false;
		}
		
		ResetRenderTargets();
		//m_displayContext->Clear()


		Restore();//�ָ�GPU Objects�����ó�ʼ״̬
		Clear(CLEAR_COLOR);
		m_displayContext->SwapBuffers(); //����������


		CheckFeatureSupport();
		//����������ʾģʽ���¼�
		VariantMap eventData;
		eventData["Width"] = width;
		eventData["Height"] = height;
		eventData["FullScreen"] = bFullScreen;
		eventData["Resizable"] = resizable;
		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_ENDRENDERING, &eventData);
		return true;
	}

	
	void GLGraphicDriver::Restore()
	{
		//�ظ�GPUObjects��״̬
		if (m_displayContext->GetWindow())
			return;

#ifdef SAPPHIRE_WIN


#elif defined(SAPPHIRE_ANDROID)


#endif

		GLenum err = glewInit();
		if (GLEW_OK != err)
		{
			SAPPHIRE_LOGERROR(StringFormatA("glewInit error! cause: %s", glewGetErrorString(err)));
			return;
		}
#ifdef GLEW_VERSION_2_0

		//����һ��һֱʹ�õ�VAO����
		if(glIsBuffer(m_VAO))
		{
			glBindVertexArray(m_VAO);
			glDeleteVertexArrays(1, &m_VAO);
		}
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

#else
		//OPENGLES 2.0
		//OpenGL Es2��֧��VAO VBO

#endif

		// ���������д1�ֽڶ���, ���ϴ��κ���������ǰ�Ƿǳ���Ҫ
		glPixelStorei(GL_PACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


		m_gpuObjMutex.Lock();
		std::unordered_map<std::string, GPUObject*>::iterator it = m_gpuObjects.begin();
		for (; it != m_gpuObjects.end(); ++it)
		{
			it->second->OnDeviceReset();
		}
		m_gpuObjMutex.Unlock();

		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_DEVICERESET, NULL);
	}

	void GLGraphicDriver::Maximize()
	{

	}

	void GLGraphicDriver::Minimize()
	{

	}

	void GLGraphicDriver::PrepareDraw()
	{
#ifndef GL_ES_VERSION_2_0

		if (m_gl3Support)
		{
			for (std::vector<ConstantBuffer*>::iterator i = m_dirtyConstantBuffers.begin(); i != m_dirtyConstantBuffers.end(); ++i)
				(*i)->Apply();
			m_dirtyConstantBuffers.clear();
		}
#endif

		if (m_fboDirty)
		{
			m_fboDirty = false;
		}
		//�ȼ����û��fbo��������ʹ��
		bool noFbo = !m_depthStencil;
		if (noFbo)
		{
			for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			{
				if (m_renderTargets[i])
				{
					noFbo = false;
					break;
				}
			}
		}


		if (noFbo)
		{
			//���°󶨻�ϵͳfbo
			if (m_curBoundFBO != m_sysFBO)
			{
				BindFrameBuffer(m_sysFBO);
				m_curBoundFBO = m_sysFBO;
			}
#ifndef GL_ES_VERSION_2_0
			// �Ƿ���sRGBд
			if (m_bsRGBWriteSupport)
			{
				bool sRGBWrite = m_renderTargets[0] ? m_renderTargets[0]->GetParentTexture()->GetSRGB(): m_bSRGBWrite;
				if (sRGBWrite != m_bSRGBWrite) //�Ƿ���ڵ�ǰ��srgbд��־
				{
					if (sRGBWrite)
						glEnable(GL_FRAMEBUFFER_SRGB_EXT);
					else
						glDisable(GL_FRAMEBUFFER_SRGB_EXT);
				}
			}
#endif
			return;
		}
		// ��Ѱһ�����������ʽ�ʹ�С���µ�֡�����������ߴ���һ���µ�
		IntVector2 rtSize = GetRenderTargetDimensions();
		unsigned texFormat = 0;
		if (m_renderTargets[0])
			texFormat = m_renderTargets[0]->GetParentTexture()->GetHWFormat();
		else if (m_depthStencil)
			texFormat = m_depthStencil->GetParentTexture()->GetHWFormat();

		//framebuffer���key�ͷֱ������  ��ʽ|x|y
		unsigned long long fboKey = (rtSize.x_ << 16 | rtSize.y_) | (((unsigned long long)texFormat) << 32);
		std::map<ulonglong, FrameBufferObject>::iterator it = m_frameBuffers.find(fboKey);

		if(it == m_frameBuffers.end()) //û���ҵ�
		{
			FrameBufferObject newFbo; //���´���һ��
			newFbo.fbo = CreateFramebuffer();
			std::pair <std::map<ulonglong, FrameBufferObject>::iterator, bool> pair = m_frameBuffers.insert(std::make_pair(fboKey, newFbo));
			it = pair.first;
		}

		//����fbo����ǰ�ҵ�ƥ���FBO
		if (m_curBoundFBO != it->second.fbo)
		{
			BindFrameBuffer(it->second.fbo);
			m_curBoundFBO = it->second.fbo;
		}

		

		/////֡��������MRT������ȾĿ�������
#ifndef GL_ES_VERSION_2_0
		// �����Ҫ��Ԓ,�O���xȡ���L�u����^
		if (it->second.readBuffers != GL_NONE)
		{
			glReadBuffer(GL_NONE); 
			it->second.readBuffers = GL_NONE;
		}
		//��鵱ǰ����ЩrenderTarget���ã��ϲ���newDrawBuffers��
		unsigned newDrawBuffers = 0;
		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (m_renderTargets[j])
				newDrawBuffers |= 1 << j;
		}

		//��ǰ�õ�renderTargets��FBO��drawBuffers�Բ���,���µ�ǰFBO��drawBuffer
		if (newDrawBuffers != it->second.drawBuffers)
		{
			// ������ɫ��ȾĿ�꣬��ֻ�������Ⱦ��
			if (!newDrawBuffers)
				glDrawBuffer(GL_NONE); //��λ
			else
			{
				int drawBufferIds[MAX_RENDERTARGETS];
				unsigned drawBufferCount = 0;

				for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
				{
					if (m_renderTargets[j])
					{
						if (!m_gl3Support)
							drawBufferIds[drawBufferCount++] = GL_COLOR_ATTACHMENT0_EXT + j;
						else
							drawBufferIds[drawBufferCount++] = GL_COLOR_ATTACHMENT0 + j;
					}
				}
				glDrawBuffers(drawBufferCount, (const GLenum*)drawBufferIds); //���ú�drawBuffer����ɫ������
			}
			it->second.drawBuffers = newDrawBuffers;
		}
#else

#endif
		 //����Ҫ���ӵ�fbo��renderTarget���������
		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (m_renderTargets[j])
			{
				ITexture* texture = m_renderTargets[j]->GetParentTexture();

				// ����y�텢���v�ˣ����N�ڸ��ӵ�����^����ǰ�ڣ�������
				if (texture->GetParametersDirty())
				{
					m_pTextureMgr->SetTextureForUpdate(texture);//���ú�Ҫ���µ�����
					texture->UpdateParameters();
					BindTexture(0, TextureUnit::TU_DIFFUSE);//�������ˣ��ǵûָ�����Ĭ��ֵ
				}
				//�������ˣ�λFBO�����ɫ����
				if (it->second.colorAttachments[j] != m_renderTargets[j])
				{
					BindColorAttachment(j, m_renderTargets[j]->GetTarget(), texture->getUID());
					it->second.colorAttachments[j] = m_renderTargets[j];
				}
			}
			else
			{
				//û��renderTarget���ÿ�
				if (it->second.colorAttachments[j])
				{
					BindColorAttachment(j, GL_TEXTURE_2D, 0);
					it->second.colorAttachments[j] = 0;
				}
			}
		}


		//�������ģ�建����
		if (m_depthStencil)
		{
			ITexture* texture = m_depthStencil->GetParentTexture();

#ifndef GL_ES_VERSION_2_0
			bool hasStencil = texture->GetHWFormat() == GL_DEPTH24_STENCIL8_EXT;
#else
			bool hasStencil = texture->GetHWFormat() == GL_DEPTH24_STENCIL8_OES;
#endif
			//��ȡrenderBuffer����
			unsigned renderBufferID = m_depthStencil->GetRenderBuffer();
			if (!renderBufferID)
			{
				//û��renderBuffer����,���������renderBuffer
				if (texture->GetParametersDirty())
				{
					m_pTextureMgr->SetTextureForUpdate(texture);//���ú�Ҫ���µ�����
					texture->UpdateParameters();
					BindTexture(0, TextureUnit::TU_DIFFUSE);//�������ˣ��ǵûָ�����Ĭ��ֵ
				}

				if (it->second.depthAttachment != m_depthStencil)
				{
					BindDepthAttachment(texture->getUID(), false);
					BindStencilAttachment(hasStencil ? texture->getUID() : 0, false);
					it->second.depthAttachment = m_depthStencil;
				}
			}
			else
			{
				
				if (it->second.depthAttachment != m_depthStencil)
				{
					BindDepthAttachment(renderBufferID, true);
					BindStencilAttachment(hasStencil ? renderBufferID : 0, true);
					it->second.depthAttachment = m_depthStencil;
				}
			}

		}
		else
		{
			//û��ģ����Ȼ�����
			if (it->second.depthAttachment)
			{
				BindDepthAttachment(0, false);
				BindStencilAttachment(0, false);
				it->second.depthAttachment = 0;
			}
		}


#ifndef GL_ES_VERSION_2_0
		// �Ƿ���sRGBд
		if (m_bsRGBWriteSupport)
		{
			bool sRGBWrite = m_renderTargets[0] ? m_renderTargets[0]->GetParentTexture()->GetSRGB() : m_bSRGBWrite;
			if (sRGBWrite != m_bSRGBWrite) //�Ƿ���ڵ�ǰ��srgbд��־
			{
				if (sRGBWrite)
					glEnable(GL_FRAMEBUFFER_SRGB_EXT);
				else
					glDisable(GL_FRAMEBUFFER_SRGB_EXT);
			}
		}
#endif
	}

	void GLGraphicDriver::Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount)
	{
		if (!vertexCount)
			return;

		PrepareDraw();

		unsigned primitiveCount;  //ͼԪ��
		GLenum glPrimitiveType;   //ͼԪ����

		GetGLPrimitiveType(vertexCount, type, primitiveCount, glPrimitiveType);
		glDrawArrays(glPrimitiveType, vertexStart, vertexCount);

		m_uNumPrimitives += primitiveCount;
		++m_uNumBatches;
	}

	void GLGraphicDriver::Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount)
	{
		if (!indexCount || !m_indexBuffer || !m_indexBuffer->GetUID())
			return;

		PrepareDraw();

		unsigned indexSize = m_indexBuffer->GetIndexSize();  //���������С
		unsigned primitiveCount;
		GLenum glPrimitiveType;

		GetGLPrimitiveType(indexCount, type, primitiveCount, glPrimitiveType);
		GLenum indexType = indexSize == sizeof(unsigned short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		glDrawElements(glPrimitiveType, indexCount, indexType, reinterpret_cast<const GLvoid*>(indexStart * indexSize));  //������ƫ�Ƶ�ַ��indexStart����ʼ

		m_uNumPrimitives += primitiveCount;
		++m_uNumBatches;
	}

	void GLGraphicDriver::ResetCachedState()
	{
		m_indexBuffer = NULL;
		m_vertexShader = NULL;
		m_pixelShader = NULL;
		m_shaderProgram = NULL;
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

	void GLGraphicDriver::SetViewport(const IntRect& rect)
	{
		PrepareDraw();

		IntVector2 rtSize = GetRenderTargetDimensions();

		IntRect rectCopy = rect;

		if (rectCopy.right_ <= rectCopy.left_)
			rectCopy.right_ = rectCopy.left_ + 1;
		if (rectCopy.bottom_ <= rectCopy.top_)
			rectCopy.bottom_ = rectCopy.top_ + 1;
		rectCopy.left_ = MathHelper::Clamp(rectCopy.left_, 0, rtSize.x_);
		rectCopy.top_ = MathHelper::Clamp(rectCopy.top_, 0, rtSize.y_);
		rectCopy.right_ = MathHelper::Clamp(rectCopy.right_, 0, rtSize.x_);
		rectCopy.bottom_ = MathHelper::Clamp(rectCopy.bottom_, 0, rtSize.y_);

		//glScissor�����½�Ϊ����ԭ��(0,0)����ͨ������£�����ϵ����Ļ���Ͻ�Ϊ����ԭ��(0,0)����ˣ���Ҫת��һ��
		glViewport(rectCopy.left_, rtSize.y_ - rectCopy.bottom_, rectCopy.Width(), rectCopy.Height());
		m_viewport = rectCopy;

		// �رռ��ò��ԣ���Ҫ�û����´�
		SetScissorTest(false);

	}

	void GLGraphicDriver::SetColorWrite(bool enable)
	{
		if (enable != m_bColorWrite)
		{
			if (enable)
				glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			else
				glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

			m_bColorWrite = enable;
		}
	}

	void GLGraphicDriver::SetCullMode(CullMode mode)
	{
		if (mode != m_cullmode)
		{
			if (mode == CULL_NONE)
				glDisable(GL_CULL_FACE);
			else
			{
				// ��Dx �෴
				glEnable(GL_CULL_FACE);
				glCullFace(mode == CULL_CCW ? GL_FRONT : GL_BACK);
			}

			m_cullmode = mode;
		}
	}

	void GLGraphicDriver::SetFillMode(FillMode mode)
	{
#ifndef GL_ES_VERSION_2_0
		if (mode != mode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, glFillMode[mode]); //���ö�������ģʽ
			m_fillmode = mode;
		}
#endif
	}

	void GLGraphicDriver::SetDepthTest(CompareMode mode)
	{

	}

	void GLGraphicDriver::SetDepthWrite(bool enable)
	{
		if (enable != m_bDepthWrite)
		{
			glDepthMask(enable ? GL_TRUE : GL_FALSE);
			m_bDepthWrite = enable;
		}
	}

	void GLGraphicDriver::SetStencilTest(bool enable, CompareMode mode /*= CMP_ALWAYS*/, StencilOp pass /*= OP_KEEP*/, StencilOp fail /*= OP_KEEP*/, StencilOp zFail /*= OP_KEEP*/, unsigned stencilRef /*= 0*/, unsigned compareMask /*= M_MAX_UNSIGNED*/, unsigned writeMask /*= M_MAX_UNSIGNED*/)
	{
#ifndef GL_ES_VERSION_2_0
		//�ж��Ƿ������иı�
		if (enable != m_bStencilTest)
		{
			if (enable)
				glEnable(GL_STENCIL_TEST);
			else
				glDisable(GL_STENCIL_TEST);
			m_bStencilTest = enable;
		}

		if (enable)
		{
			//�ж�ģ����Ե�ģʽ���ο�ֵ���Ƚ������Ƿ���֮ǰ��ͬ
			if (mode != m_stencilTestMode || stencilRef != m_stencilRefVal || compareMask != m_stencilCompareMask)
			{
				//����ģ��ȽϺ���, �ο�ֵ�� �Ƚ�����
				glStencilFunc(glCmpFunc[mode], stencilRef, compareMask);
				//���浱ǰ������
				m_stencilTestMode = mode;
				m_stencilRefVal = stencilRef;
				m_stencilCompareMask = compareMask;
			}
			if (writeMask != m_stencilCompareMask)
			{
				glStencilMask(writeMask);
				m_stencilWriteMask = writeMask;
			}
			//  ����ģ�����ͨ����ʧ�ܵĲ���
			if (pass != m_stencilPassOp || fail != m_stencilFailOp || zFail != m_stencilZFailOp)
			{
				glStencilOp(glStencilOps[fail], glStencilOps[zFail], glStencilOps[pass]);
				m_stencilPassOp = pass;
				m_stencilFailOp = fail;
				m_stencilZFailOp = zFail;
			}
		}
#endif // GL_ES_VERSION_2_0

	}

	void GLGraphicDriver::SetBlendMode(BlendMode mode)
	{
		if (mode != m_blendMode)
		{
			if (mode == BLEND_REPLACE)
				glDisable(GL_BLEND);
			else
			{
				glEnable(GL_BLEND);
				glBlendFunc(glSrcBlend[mode], glDestBlend[mode]);
				glBlendEquation(glBlendOp[mode]);
			}

			m_blendMode = mode;
		}
	}

	void GLGraphicDriver::SetScissorTest(bool enable, const IntRect& rect)
	{
		//���ò����������ƻ������������ڵ����أ����������޸ġ�����������أ������ᱻ�޸�
		IntVector2 rtSize(GetRenderTargetDimensions()); //��ȾĿ���С
		IntVector2 viewPos(m_viewport.left_, m_viewport.top_);  //�ӿ�λ��

		if (enable)
		{
			IntRect intRect;
			intRect.left_ = MathHelper::Clamp(rect.left_ + viewPos.x_, 0, rtSize.x_ - 1);
			intRect.top_ = MathHelper::Clamp(rect.top_ + viewPos.y_, 0, rtSize.y_ - 1);
			intRect.right_ = MathHelper::Clamp(rect.right_ + viewPos.x_, 0, rtSize.x_);
			intRect.bottom_ = MathHelper::Clamp(rect.bottom_ + viewPos.y_, 0, rtSize.y_);

			if (intRect.right_ == intRect.left_)
				intRect.right_++;
			if (intRect.bottom_ == intRect.top_)
				intRect.bottom_++;

			if (intRect.right_ < intRect.left_ || intRect.bottom_ < intRect.top_)
				enable = false;

			if (enable && m_scissorRect != intRect)
			{
				//glScissor�����½�Ϊ����ԭ��(0,0)����ͨ������£�����ϵ����Ļ���Ͻ�Ϊ����ԭ��(0,0)����ˣ���Ҫת��һ��
				glScissor(intRect.left_, rtSize.y_ - intRect.bottom_, intRect.Width(), intRect.Height());
				m_scissorRect = intRect;
			}
		}
		else
			m_scissorRect = IntRect::ZERO;

		if (enable != m_bScissorTest)
		{
			if (enable)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
			m_bScissorTest = enable;
		}
	}

	void GLGraphicDriver::SetScissorTest(bool enable, const Rect& rect /*= Rect::FULL*/, bool borderInclusive /*= true*/)
	{
		//���ò����������ƻ������������ڵ����أ����������޸ġ�����������أ������ᱻ�޸�
		// ��ĳЩ��Ⱦѭ������ؿ���/�ر�һ������������
		// �رռ��ÿ��Լ���״̬�ı�

		if (rect.min_.x_ <= 0.0f && rect.min_.y_ <= 0.0f && rect.max_.x_ >= 1.0f && rect.max_.y_ >= 1.0f)
			enable = false;

		if (enable)
		{
			IntVector2 rtSize(GetRenderTargetDimensions());
			IntVector2 viewSize(m_viewport.Size());
			IntVector2 viewPos(m_viewport.left_, m_viewport.top_);
			IntRect intRect;
			int expand = borderInclusive ? 1 : 0;

			intRect.left_ = MathHelper::Clamp((int)((rect.min_.x_ + 1.0f) * 0.5f * viewSize.x_) + viewPos.x_, 0, rtSize.x_ - 1);
			intRect.top_ = MathHelper::Clamp((int)((-rect.max_.y_ + 1.0f) * 0.5f * viewSize.y_) + viewPos.y_, 0, rtSize.y_ - 1);
			intRect.right_ = MathHelper::Clamp((int)((rect.max_.x_ + 1.0f) * 0.5f * viewSize.x_) + viewPos.x_ + expand, 0, rtSize.x_);
			intRect.bottom_ = MathHelper::Clamp((int)((-rect.min_.y_ + 1.0f) * 0.5f * viewSize.y_) + viewPos.y_ + expand, 0, rtSize.y_);

			if (intRect.right_ == intRect.left_)
				intRect.right_++;
			if (intRect.bottom_ == intRect.top_)
				intRect.bottom_++;

			if (intRect.right_ < intRect.left_ || intRect.bottom_ < intRect.top_)
				enable = false;

			if (enable && m_scissorRect != intRect)
			{
				//glScissor�����½�Ϊ����ԭ��(0,0)����ͨ������£�����ϵ����Ļ���Ͻ�Ϊ����ԭ��(0,0)����ˣ���Ҫת��һ��
				glScissor(intRect.left_, rtSize.y_ - intRect.bottom_, intRect.Width(), intRect.Height());
				m_scissorRect = intRect;
			}
		}
		else
			m_scissorRect = IntRect::ZERO;

		if (enable != m_bScissorTest)
		{
			if (enable)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
			m_bScissorTest = enable;
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, float value)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location,sizeof(value), &value);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				glUniform1fv(info->m_location, 1, &value); //���õ�OpengGL Shader
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const float* data, unsigned count)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(float)*count, data);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				switch (info->m_type)
				{
				case GL_FLOAT:
					{
						glUniform1fv(info->m_location, count, data);
					}
					break;
				case GL_FLOAT_VEC2:
					glUniform2fv(info->m_location, count / 2, data);
					break;

				case GL_FLOAT_VEC3:
					glUniform3fv(info->m_location, count / 3, data);
					break;

				case GL_FLOAT_VEC4:
					glUniform4fv(info->m_location, count / 4, data);
					break;

				case GL_FLOAT_MAT3:
					glUniformMatrix3fv(info->m_location, count / 9, GL_FALSE, data);
					break;

				case GL_FLOAT_MAT4:
					glUniformMatrix4fv(info->m_location, count / 16, GL_FALSE, data);
					break;
				default:
					break;
				}
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Vector2& vector)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector2), &vector);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				switch (info->m_type)
				{
				case GL_FLOAT:
				{
					glUniform1fv(info->m_location, 1, vector.Data());
				}
				break;
				case GL_FLOAT_VEC2:
					glUniform2fv(info->m_location, 1, vector.Data());
					break;
				default:
					break;
				}
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Vector4& vector)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector4), &vector);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				switch (info->m_type)
				{
				case GL_FLOAT:
				{
					glUniform1fv(info->m_location, 1, vector.Data());
				}
				break;
				case GL_FLOAT_VEC2:
					glUniform2fv(info->m_location, 1, vector.Data());
					break;
				case GL_FLOAT_VEC3:
					glUniform3fv(info->m_location, 1, vector.Data());
					break;
				case GL_FLOAT_VEC4:
					glUniform4fv(info->m_location, 1, vector.Data());
					break;
				default:
					break;
				}
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Vector3& vector)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector3), &vector);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				switch (info->m_type)
				{
				case GL_FLOAT:
				{
					glUniform1fv(info->m_location, 1, vector.Data());
				}
				break;
				case GL_FLOAT_VEC2:
					glUniform2fv(info->m_location, 1, vector.Data());
					break;
				case GL_FLOAT_VEC3:
					glUniform3fv(info->m_location, 1, vector.Data());
					break;
				default:
					break;
				}
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Matrix3x3& matrix)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix3x3), &matrix);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				glUniformMatrix3fv(info->m_location, 1, GL_FALSE, matrix.Data());
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Matrix3x4& matrix)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				// ��չ��4x4����
				static Matrix4x4 fullMatrix;
				fullMatrix.m00_ = matrix.m00_;
				fullMatrix.m01_ = matrix.m01_;
				fullMatrix.m02_ = matrix.m02_;
				fullMatrix.m03_ = matrix.m03_;
				fullMatrix.m10_ = matrix.m10_;
				fullMatrix.m11_ = matrix.m11_;
				fullMatrix.m12_ = matrix.m12_;
				fullMatrix.m13_ = matrix.m13_;
				fullMatrix.m20_ = matrix.m20_;
				fullMatrix.m21_ = matrix.m21_;
				fullMatrix.m22_ = matrix.m22_;
				fullMatrix.m23_ = matrix.m23_;
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix3x4), &matrix);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				glUniformMatrix4fv(info->m_location, 1, GL_FALSE, matrix.Data());
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Matrix4x4& matrix)
	{
		if (m_shaderProgram)
		{
			const ShaderParameter* info = m_shaderProgram->GetParameter(param);
			if (info)
			{
				//����л����������õ���������
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix4x4), &matrix);//���Ƶ�Ӱ�ӻ�����
					return;
				}
				glUniformMatrix4fv(info->m_location, 1, GL_FALSE, matrix.Data());
			}
		}
	}

	void GLGraphicDriver::SetShaderParameter(StringHash param, const Color& color)
	{
		SetShaderParameter(param, color.Data(), 4);
	}


	bool GLGraphicDriver::HasShaderParameter(StringHash param)
	{
		return m_shaderProgram && m_shaderProgram->HasParameter(param);
	}

	bool GLGraphicDriver::NeedParameterUpdate(ShaderParameterGroup group, const void* source)
	{
		return m_shaderProgram ? m_shaderProgram->NeedParameterUpdate(group, source) : false;
	}

	void GLGraphicDriver::ClearParameterSources()
	{
		GLShaderProgram::ClearParameterSources();
	}

	void GLGraphicDriver::ClearTransformSources()
	{
		if (m_shaderProgram)
		{
			m_shaderProgram->ClearParameterSource(SP_CAMERA);
			m_shaderProgram->ClearParameterSource(SP_OBJECT);
		}

	}

	void GLGraphicDriver::ClearParameterSource(ShaderParameterGroup group)
	{
		if (m_shaderProgram)
		{
			m_shaderProgram->ClearGlobalParameterSource(group);
		}
	}

	Sapphire::ConstantBuffer* GLGraphicDriver::GetOrCreateConstantBuffer(unsigned bindingIndex, unsigned size)
	{
		unsigned key = (bindingIndex << 16) | size;
		std::unordered_map<unsigned, SharedPtr<ConstantBuffer> >::iterator i = m_constantsBuffers.find(key);
		if (i == m_constantsBuffers.end())
		{
			std::pair<unsigned, SharedPtr<ConstantBuffer>> pair = std::make_pair(key, SharedPtr<ConstantBuffer>(new ConstantBuffer(m_pCore, this)));
			m_constantsBuffers.insert(pair);
			pair.second->SetSize(size);
			return pair.second.Get();
		}
		else
		{
			return i->second.Get();
		}
	}

	void GLGraphicDriver::ResetRenderTargets()
	{
		//������ȾĿ�꣬ȫ������0
		for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			SetRenderTarget(i, (GLRenderSurface*)0);
		SetDepthStencil((GLRenderSurface*)0);
		IntVector2 size = m_displayContext->GetWindowSize();
		SetViewport(IntRect(0, 0, size.x_, size.y_)); //�����ӿ�
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
		return m_displayContext->IsTerminated();
	}

	void GLGraphicDriver::MarkFBODirty()
	{
		m_fboDirty = true;
	}

	uint GLGraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //��ѯ����ĸ�����������
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniLevel);
		return (uint)maxAniLevel;
	}

	uint GLGraphicDriver::GetMaxBones()
	{
#ifdef RPI
		return 32;
#else
		return GLGraphicDriver::GetGL3Support() ? 128 : 64;
#endif
	}

	Sapphire::IntVector2 GLGraphicDriver::GetRenderTargetDimensions() const
	{
		int width, height;

		if (m_renderTargets[0])
		{
			width = m_renderTargets[0]->GetWidth();
			height = m_renderTargets[0]->GetHeight();
		}
		else if (m_depthStencil)
		{
			width = m_depthStencil->GetWidth();
			height = m_depthStencil->GetHeight();
		}
		else
		{
			return m_displayContext->GetWindowSize(); //û�ҵ������ش��ڴ�С
		}

		return IntVector2(width, height);
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
		return m_curBoundFBO;
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

 

	Sapphire::IShaderVariation* GLGraphicDriver::GetShader(ShaderType type, const std::string& name, const std::string& defines /*= ""*/) const
	{
		return GetShader(type, name.c_str(), defines.c_str());
	}

	Sapphire::IShaderVariation* GLGraphicDriver::GetShader(ShaderType type, const char* name, const char* defines) const
	{
		//�@ȡshader��ԓ��ͬ�����d
		return m_pGLShaderMgr->GetShader(type, name, defines, false);
	}


	void GLGraphicDriver::SetShaders(IShaderVariation* vs, IShaderVariation* ps, IShaderVariation* gs /*= NULL*/, IShaderVariation* cs /*= NULL*/)
	{
		//�ж��Ƿ��ǵ�ǰʹ�õ�shader
		if (vs == m_vertexShader && ps == m_pixelShader)
			return;
		
		GLShaderVariation* glvs = (GLShaderVariation*)(vs);
		GLShaderVariation* glps = (GLShaderVariation*)(ps);
		//�Ƿ�������
		if (glvs && !glvs->GetGPUHandle())
		{
			if (glvs->GetCompilerOutput().empty())
			{
				//����
				bool bSuccess = glvs->Create();
				if (bSuccess)
				{
					SAPPHIRE_LOG(StringFormatA("Compiled vertex shader %s",glvs->GetFullName().c_str()));
				}
				else
				{
					SAPPHIRE_LOG(StringFormatA("Failed to compile vertex shader %s :\n ", glvs->GetFullName().c_str(),glvs->GetCompilerOutput().c_str()));
					glvs = NULL;
				}
			}

		}
		else
		{
			glvs = NULL;
		}
		if (glps && !glps->GetGPUHandle())
		{
			if (glps->GetCompilerOutput().empty())
			{
				//����
				bool bSuccess = glps->Create();
				if (bSuccess)
				{
					SAPPHIRE_LOG(StringFormatA("Compiled pixel shader %s", glps->GetFullName().c_str()));
				}
				else
				{
					SAPPHIRE_LOG(StringFormatA("Failed to compile pixel shader %s :\n ", glps->GetFullName().c_str(), glps->GetCompilerOutput().c_str()));
					glps = NULL;
				}
			}

		}
		else
		{
			glps = NULL;
		}

		if (!glvs || !glps)
		{
			glUseProgram(0);
			m_vertexShader = 0;
			m_pixelShader = 0;
			m_shaderProgram = 0;
		}
		else
		{
			m_vertexShader = glvs;
			m_pixelShader = glps;

			 
			std::string  key = GLShaderProgram::GetGLProgrameName(glvs, glps, NULL, NULL);
			
			std::unordered_map<std::string, SharedPtr<GLShaderProgram>>::iterator it = m_shaderProgramDict.find(key);
			if (it != m_shaderProgramDict.end())
			{
				if (it->second->GetGPUHandle())
				{
					glUseProgram(it->second->GetGPUHandle());
					m_shaderProgram = it->second;
				}
				else
				{
					glUseProgram(0);
					m_shaderProgram = NULL;
				}
			}
			else
			{
				 
				GLShaderProgram* newProgram = SharedPtr<GLShaderProgram>(new GLShaderProgram(m_pCore, this, glvs, glps));
				if (newProgram->Link())
				{
					SAPPHIRE_LOG(StringFormatA("Linked vertex Shader %s and pixel Shader %s", glvs->GetFullName().c_str(),glps->GetFullName().c_str()));
					m_shaderProgram = newProgram;
				}
				else
				{
					SAPPHIRE_LOG(StringFormatA("Linked vertex Shader %s and pixel Shader %s  :\n %s", glvs->GetFullName().c_str(), glps->GetFullName().c_str(), newProgram->GetLinkerOutput().c_str()));
					m_shaderProgram = 0;
				}
				m_shaderProgramDict[key] = newProgram;
			}

		}

#ifndef GL_ES_VERSION_2_0

		if (m_gl3Support && m_shaderProgram)
		{
			const SharedPtr<ConstantBuffer>* constantBuffers = m_shaderProgram->GetConstantBuffers();
			for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS * 2; ++i) //VertexShader��PixelShader��ռһ�Σ�����������
			{
				ConstantBuffer* buffer = constantBuffers[i].Get();
				if (buffer != m_currentConstantBuffers[i])
				{
					unsigned handle = buffer ? buffer->GetGPUHandle() : 0;
					glBindBufferBase(GL_UNIFORM_BUFFER, i, handle); //�󶨻���������
					m_curBoundUBO = handle; //��ǰ�󶨵�ubo
					m_currentConstantBuffers[i] = buffer;  //���õ�ǰ�󶨵�ubo����
					//����ȫ�ֲ���Դ
					GLShaderProgram::ClearGlobalParameterSource((ShaderParameterGroup)(i % MAX_SHADER_PARAMETER_GROUPS));
				}
				
			}
			//�Ƿ������Զ������ƽ��
			SetShaderParameter(VSP_CLIPPLANE, m_useCustomClipPlane ? m_customclipPlane : Vector4(0.0f, 0.0f, 0.0f, 1.0f));
		}

#endif
		//
		//if (m_shaderPrecache)
			//m_shaderPrecache->StoreShaders(glvs, glps);
		

	}


	Sapphire::IShaderVariation* GLGraphicDriver::GetVertexShader() const
	{
		return m_vertexShader;
	}

	Sapphire::IShaderVariation* GLGraphicDriver::GetPixelShader() const
	{
		return m_pixelShader;
	}

	void GLGraphicDriver::SetShaderPath(std::string path)
	{
		if (DirIsExistA(path.c_str()))
		{
			m_shaderResPath = path;
		}
		
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
		/*GLRenderSurface* renderTarget = 0;
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
		std::string ver = m_displayContext->GetApiVersion();
		if (!ver.empty() &&ver.size()>7)
		{
			if (ver[0] > '3')
			{
				m_gl3Support = true; //֧��OpenGL3.0����
			}
		}
		else
		{
			m_gl3Support = false;
		}
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
		return m_displayContext->GetWindow();
	}

	bool GLGraphicDriver::IsInitialized()
	{
		return m_bIsInitialized;
	}

	Sapphire::IDisplayContext* GLGraphicDriver::GetDisplayContext() const
	{
		return m_displayContext;
	}

	bool GLGraphicDriver::BeginFrame()
	{
		if (!IsInitialized() || IsDeviceLost())
			return false;

		//����������ȾĿ�꣬��Ȼ�����������ӿڣ���ʼ�����µ�һ֡
		ResetRenderTargets();

		//����ǰһ֡ʹ�õİ��������
		for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_pTextureMgr->SetTexture(0, (TextureUnit)i);
		}
		//������ɫ�����д��ģʽ
		SetColorWrite(true);
		SetDepthWrite(true);
		//�����¼�
		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_BEGINRENDERING, NULL);

		return true;
	}

	void GLGraphicDriver::EndFrame()
	{
		if (!IsInitialized())
			return;

		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_ENDRENDERING, NULL);
		//˫���彻��
		m_displayContext->SwapBuffers();
		//������ʱ�Ļ�����
		CleanScratchBuffers();
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

	GLenum GLGraphicDriver::GetWarpMode(TextureAddressMode mode)
	{
#ifndef GL_ES_VERSION_2_0
		return GLGraphicDriver::GetGL3Support() ? gl3WrapModes[mode] : glWrapModes[mode];
#else
		return glWrapModes[mode];
#endif
	}

	int GLGraphicDriver::GetHWAlphaFormat()
	{
#ifndef GL_ES_VERSION_2_0
		// Alpha format is deprecated on OpenGL 3+
		if (GLGraphicDriver::GetGL3Support())
			return GL_R8;
#endif
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

	void GLGraphicDriver::CleanFrameBuffers()
	{
		//���������е�fbo
		if (!IsDeviceLost())
		{
			//���°󶨻�ϵͳfbo
			BindFrameBuffer(m_sysFBO);//��fbo��ϵͳfbo
			m_curBoundFBO = m_sysFBO;
			m_fboDirty = true;

			for (std::map<ulonglong, FrameBufferObject>::iterator it = m_frameBuffers.begin(); it != m_frameBuffers.end(); ++it)
			{
				DeleteFrameBuffer(it->second.fbo);
			}
		}
		else
		{
			m_curBoundFBO = 0;
		}

		m_frameBuffers.clear();
	}

	void GLGraphicDriver::Clear(unsigned flags, const Color& color /*= Color(0.0f, 0.0f, 0.0f, 0.0f)*/, float depth /*= 1.0f*/, unsigned stencil /*= 0*/)
	{
		PrepareDraw();

#ifdef GL_ES_VERSION_2_0
		flags &= ~CLEAR_STENCIL;
#endif // !GL_ES_VERSION_2_0

		//�ȱ���ɵ�״̬
		bool oldColorWrite = m_bColorWrite;
		bool oldDepthWrite = m_bDepthWrite;
		
		if (flags & CLEAR_COLOR && !oldColorWrite)
			SetColorWrite(true);
		if (flags & CLEAR_DEPTH && !oldDepthWrite)
			SetDepthWrite(true);
		if (flags & CLEAR_STENCIL && m_stencilWriteMask != M_MAX_UNSIGNED)
			glStencilMask(M_MAX_UNSIGNED);

		unsigned glFlags = 0; //����clearֵ clear��־
		if (flags & CLEAR_COLOR)
		{
			glFlags |= GL_COLOR_BUFFER_BIT;
			glClearColor(color.r_, color.g_, color.b_, color.a_);
		}
		if (flags & CLEAR_DEPTH)
		{
			glFlags |= GL_DEPTH_BUFFER_BIT;
			glClearDepth(depth);
		}
		if (flags & CLEAR_STENCIL)
		{
			glFlags |= GL_STENCIL_BUFFER_BIT;
			glClearStencil(stencil);
		}

		// ����ӿڲ���ȫ��Ļ�����ü���������clear

		IntVector2 viewSize = GetRenderTargetDimensions(); //��ȡrt��Χ
		if (m_viewport.left_ != 0 || m_viewport.top_ != 0 || m_viewport.right_ != viewSize.x_ || m_viewport.bottom_ != viewSize.y_)
			SetScissorTest(true, IntRect(0, 0, m_viewport.Width(), m_viewport.Height()));
		else
			SetScissorTest(false);

		glClear(glFlags); //ִ��clear

		SetScissorTest(false);
		SetColorWrite(oldColorWrite);//�ָ���ǰ������
		SetDepthWrite(oldDepthWrite);
		if (flags & CLEAR_STENCIL && m_stencilWriteMask != M_MAX_UNSIGNED)
			glStencilMask(m_stencilWriteMask);

	}


	void GLGraphicDriver::AddGPUObject(GPUObject* gpuObj)
	{
		m_gpuObjects[gpuObj->GetUUID()] = gpuObj;
	}

	void GLGraphicDriver::RemoveGPUObject(GPUObject* gpuObj)
	{
		std::unordered_map<std::string, GPUObject*>::iterator it = m_gpuObjects.find(gpuObj->GetUUID());
		if (it != m_gpuObjects.end())
		{
			m_gpuObjects.erase(it);
		}	
	}

}