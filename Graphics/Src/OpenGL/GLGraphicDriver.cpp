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
	// 重新OpenGL映射顶点属性,经常蒙皮修正要用到,要避免GLES2 蒙皮bug，gles设备只支持到8
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


	//取的对应OpenGL的图元数和图元类型
	void GetGLPrimitiveType(unsigned elementCount, PrimitiveType type, unsigned& primitiveCount, GLenum& glPrimitiveType)
	{
		switch (type)
		{
		case TRIANGLE_LIST:
			primitiveCount = elementCount / 3;   //三角形 每三个顶点一个三角形
			glPrimitiveType = GL_TRIANGLES;
			break;

		case LINE_LIST:
			primitiveCount = elementCount / 2;     //线段  每2个顶点一条线段
			glPrimitiveType = GL_LINES;
			break;

		case POINT_LIST:
			primitiveCount = elementCount;    //点   每个顶点一个点
			glPrimitiveType = GL_POINTS;
			break;

		case TRIANGLE_STRIP:                 //三角形带     三角形数=顶点数-2
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
		m_displayContext = new GLDisplayContext(); //创建OpenGL窗口显示环境
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
		m_displayContext->Initialize(); //初始化OpenGL窗口显示环境
		CheckFeature();
		//初始化渲染系統接口
		IRenderSystem* pRenderSys = new GLRenderSystem(m_pCore,this);
		pRenderSys->Initialize();
		m_pCore->RegisterSubSystem<IRenderSystem>(pRenderSys, ESST_RENDERSYSTEM);

		//创建UI渲染器
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
				//释放所有的存在的GPU对象，
				//ShaderProgram也作为一个GPU对象，首先清理它们避免迭代的时候列表被修改
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
				//要重新创建环境，使得GpuObject都丢失
				std::unordered_map<std::string, GPUObject*>::iterator it = m_gpuObjects.begin();
				for (; it != m_gpuObjects.end(); ++it)
				{
					it->second->OnDeviceLost();
				}
				//最后清理shaderProgram
				m_shaderProgramDict.clear();
				

				FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_DEVICELOST, NULL);
			}
			
		}

		CleanFrameBuffers();
		m_depthTextures.clear();

		if (m_displayContext)
		{
			//终止显示环境
			m_displayContext->Terminate();
		}
		//销毁窗口
		if (closeWindow)
		{
			m_displayContext->CloseWindow();
		}
		
	}

	bool GLGraphicDriver::m_gl3Support;


	bool GLGraphicDriver::SetDisplayMode(int x, int y, int width, int height, bool bFullScreen, bool bVsync, int multiSample, bool tripleBuffer, bool resizable)
	{

		if (!width || !height) //长宽设置有非法
		{
			if (bFullScreen) //全屏幕的话，先取得当前显示器的分辨率
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
		//把采样次数限制到1~16之间
		multiSample = MathHelper::Clamp(multiSample, 1, 16);

		{
			//作为嵌入窗口时不能关闭
			if (!m_displayContext->GetExternalWindow())
			{
				//DisplayContext关闭存在的窗口，并标记GpuObject Lost
				Release(false, true);
				//重新再创建窗口
				m_displayContext->CreateRenderWindow(x, y, width, height, bFullScreen, multiSample,false, bVsync);
				 
			}
			if (!m_displayContext->GetWindow())
				return false;
		}
		
		ResetRenderTargets();
		//m_displayContext->Clear()


		Restore();//恢复GPU Objects并设置初始状态
		Clear(CLEAR_COLOR);
		m_displayContext->SwapBuffers(); //交换缓冲区


		CheckFeatureSupport();
		//发送设置显示模式的事件
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
		//回复GPUObjects的状态
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

		//創建一個一直使用的VAO對象
		if(glIsBuffer(m_VAO))
		{
			glBindVertexArray(m_VAO);
			glDeleteVertexArrays(1, &m_VAO);
		}
		glGenVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

#else
		//OPENGLES 2.0
		//OpenGL Es2不支持VAO VBO

#endif

		// 设置纹理读写1字节对齐, 在上传任何纹理数据前是非常重要
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
		//先检查有没有fbo对象正在使用
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
			//重新绑定回系统fbo
			if (m_curBoundFBO != m_sysFBO)
			{
				BindFrameBuffer(m_sysFBO);
				m_curBoundFBO = m_sysFBO;
			}
#ifndef GL_ES_VERSION_2_0
			// 是否开启sRGB写
			if (m_bsRGBWriteSupport)
			{
				bool sRGBWrite = m_renderTargets[0] ? m_renderTargets[0]->GetParentTexture()->GetSRGB(): m_bSRGBWrite;
				if (sRGBWrite != m_bSRGBWrite) //是否等于当前的srgb写标志
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
		// 搜寻一个基于这个格式和大小的新的帧缓冲区，或者创建一个新的
		IntVector2 rtSize = GetRenderTargetDimensions();
		unsigned texFormat = 0;
		if (m_renderTargets[0])
			texFormat = m_renderTargets[0]->GetParentTexture()->GetHWFormat();
		else if (m_depthStencil)
			texFormat = m_depthStencil->GetParentTexture()->GetHWFormat();

		//framebuffer表的key和分辨率相关  格式|x|y
		unsigned long long fboKey = (rtSize.x_ << 16 | rtSize.y_) | (((unsigned long long)texFormat) << 32);
		std::map<ulonglong, FrameBufferObject>::iterator it = m_frameBuffers.find(fboKey);

		if(it == m_frameBuffers.end()) //没有找到
		{
			FrameBufferObject newFbo; //重新创建一个
			newFbo.fbo = CreateFramebuffer();
			std::pair <std::map<ulonglong, FrameBufferObject>::iterator, bool> pair = m_frameBuffers.insert(std::make_pair(fboKey, newFbo));
			it = pair.first;
		}

		//綁定fbo到當前找到匹配的FBO
		if (m_curBoundFBO != it->second.fbo)
		{
			BindFrameBuffer(it->second.fbo);
			m_curBoundFBO = it->second.fbo;
		}

		

		/////帧缓存对象的MRT多重渲染目标的设置
#ifndef GL_ES_VERSION_2_0
		// 如果需要的話,設置讀取和繪製緩存區
		if (it->second.readBuffers != GL_NONE)
		{
			glReadBuffer(GL_NONE); 
			it->second.readBuffers = GL_NONE;
		}
		//检查当前有哪些renderTarget在用，合并到newDrawBuffers中
		unsigned newDrawBuffers = 0;
		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (m_renderTargets[j])
				newDrawBuffers |= 1 << j;
		}

		//当前用到renderTargets和FBO的drawBuffers对不上,更新当前FBO的drawBuffer
		if (newDrawBuffers != it->second.drawBuffers)
		{
			// 检查非颜色渲染目标，（只有深度渲染）
			if (!newDrawBuffers)
				glDrawBuffer(GL_NONE); //复位
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
				glDrawBuffers(drawBufferCount, (const GLenum*)drawBufferIds); //设置好drawBuffer的颜色缓存区
			}
			it->second.drawBuffers = newDrawBuffers;
		}
#else

#endif
		 //更新要附加到fbo的renderTarget的纹理对象
		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (m_renderTargets[j])
			{
				ITexture* texture = m_renderTargets[j]->GetParentTexture();

				// 如果紋理參數髒了，那麼在附加到緩存區對象前期，更新它
				if (texture->GetParametersDirty())
				{
					m_pTextureMgr->SetTextureForUpdate(texture);//设置好要更新的纹理
					texture->UpdateParameters();
					BindTexture(0, TextureUnit::TU_DIFFUSE);//更新完了，记得恢复纹理默认值
				}
				//更新完了，位FBO添加颜色附件
				if (it->second.colorAttachments[j] != m_renderTargets[j])
				{
					BindColorAttachment(j, m_renderTargets[j]->GetTarget(), texture->getUID());
					it->second.colorAttachments[j] = m_renderTargets[j];
				}
			}
			else
			{
				//没有renderTarget，置空
				if (it->second.colorAttachments[j])
				{
					BindColorAttachment(j, GL_TEXTURE_2D, 0);
					it->second.colorAttachments[j] = 0;
				}
			}
		}


		//更新深度模板缓存区
		if (m_depthStencil)
		{
			ITexture* texture = m_depthStencil->GetParentTexture();

#ifndef GL_ES_VERSION_2_0
			bool hasStencil = texture->GetHWFormat() == GL_DEPTH24_STENCIL8_EXT;
#else
			bool hasStencil = texture->GetHWFormat() == GL_DEPTH24_STENCIL8_OES;
#endif
			//获取renderBuffer对象
			unsigned renderBufferID = m_depthStencil->GetRenderBuffer();
			if (!renderBufferID)
			{
				//没有renderBuffer对象,用纹理代替renderBuffer
				if (texture->GetParametersDirty())
				{
					m_pTextureMgr->SetTextureForUpdate(texture);//设置好要更新的纹理
					texture->UpdateParameters();
					BindTexture(0, TextureUnit::TU_DIFFUSE);//更新完了，记得恢复纹理默认值
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
			//没有模板深度缓存区
			if (it->second.depthAttachment)
			{
				BindDepthAttachment(0, false);
				BindStencilAttachment(0, false);
				it->second.depthAttachment = 0;
			}
		}


#ifndef GL_ES_VERSION_2_0
		// 是否开启sRGB写
		if (m_bsRGBWriteSupport)
		{
			bool sRGBWrite = m_renderTargets[0] ? m_renderTargets[0]->GetParentTexture()->GetSRGB() : m_bSRGBWrite;
			if (sRGBWrite != m_bSRGBWrite) //是否等于当前的srgb写标志
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

		unsigned primitiveCount;  //图元数
		GLenum glPrimitiveType;   //图元类型

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

		unsigned indexSize = m_indexBuffer->GetIndexSize();  //获得索引大小
		unsigned primitiveCount;
		GLenum glPrimitiveType;

		GetGLPrimitiveType(indexCount, type, primitiveCount, glPrimitiveType);
		GLenum indexType = indexSize == sizeof(unsigned short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
		glDrawElements(glPrimitiveType, indexCount, indexType, reinterpret_cast<const GLvoid*>(indexStart * indexSize));  //索引的偏移地址从indexStart处开始

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
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer ? pIndexBuffer->GetUID() : 0);  //绑定索引缓冲区
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

		//glScissor以左下角为坐标原点(0,0)，而通常情况下，坐标系以屏幕左上角为坐标原点(0,0)。因此，需要转换一下
		glViewport(rectCopy.left_, rtSize.y_ - rectCopy.bottom_, rectCopy.Width(), rectCopy.Height());
		m_viewport = rectCopy;

		// 关闭剪裁测试，需要用户重新打开
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
				// 和Dx 相反
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
			glPolygonMode(GL_FRONT_AND_BACK, glFillMode[mode]); //设置多边形填充模式
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
		//判断是否设置有改变
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
			//判断模板测试的模式、参考值、比较掩码是否与之前相同
			if (mode != m_stencilTestMode || stencilRef != m_stencilRefVal || compareMask != m_stencilCompareMask)
			{
				//设置模板比较函数, 参考值， 比较掩码
				glStencilFunc(glCmpFunc[mode], stencilRef, compareMask);
				//保存当前的设置
				m_stencilTestMode = mode;
				m_stencilRefVal = stencilRef;
				m_stencilCompareMask = compareMask;
			}
			if (writeMask != m_stencilCompareMask)
			{
				glStencilMask(writeMask);
				m_stencilWriteMask = writeMask;
			}
			//  设置模板测试通过，失败的操作
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
		//剪裁测试用于限制绘制区域。区域内的像素，将被绘制修改。区域外的像素，将不会被修改
		IntVector2 rtSize(GetRenderTargetDimensions()); //渲染目标大小
		IntVector2 viewPos(m_viewport.left_, m_viewport.top_);  //视口位置

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
				//glScissor以左下角为坐标原点(0,0)，而通常情况下，坐标系以屏幕左上角为坐标原点(0,0)。因此，需要转换一下
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
		//剪裁测试用于限制绘制区域。区域内的像素，将被绘制修改。区域外的像素，将不会被修改
		// 在某些渲染循环里，返回开启/关闭一个完整的区域
		// 关闭剪裁可以减少状态改变

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
				//glScissor以左下角为坐标原点(0,0)，而通常情况下，坐标系以屏幕左上角为坐标原点(0,0)。因此，需要转换一下
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location,sizeof(value), &value);//复制到影子缓冲区
					return;
				}
				glUniform1fv(info->m_location, 1, &value); //设置到OpengGL Shader
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(float)*count, data);//复制到影子缓冲区
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector2), &vector);//复制到影子缓冲区
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector4), &vector);//复制到影子缓冲区
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Vector3), &vector);//复制到影子缓冲区
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix3x3), &matrix);//复制到影子缓冲区
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
				// 扩展成4x4矩阵
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix3x4), &matrix);//复制到影子缓冲区
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
				//如果有缓冲区，设置到缓存区中
				if (info->m_bufferPtr)
				{
					ConstantBuffer* buffer = info->m_bufferPtr;
					if (!buffer->IsDirty())
					{
						m_dirtyConstantBuffers.push_back(buffer);
					}
					buffer->SetParameter((uint)info->m_location, sizeof(Matrix4x4), &matrix);//复制到影子缓冲区
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
		//遍历渲染目标，全部重置0
		for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
			SetRenderTarget(i, (GLRenderSurface*)0);
		SetDepthStencil((GLRenderSurface*)0);
		IntVector2 size = m_displayContext->GetWindowSize();
		SetViewport(IntRect(0, 0, size.x_, size.y_)); //重置视口
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
		return m_displayContext->IsTerminated();
	}

	void GLGraphicDriver::MarkFBODirty()
	{
		m_fboDirty = true;
	}

	uint GLGraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //查询允许的各向异性数量
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
			return m_displayContext->GetWindowSize(); //没找到，返回窗口大小
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
		//獲取shader應該用同步加載
		return m_pGLShaderMgr->GetShader(type, name, defines, false);
	}


	void GLGraphicDriver::SetShaders(IShaderVariation* vs, IShaderVariation* ps, IShaderVariation* gs /*= NULL*/, IShaderVariation* cs /*= NULL*/)
	{
		//判断是否是当前使用的shader
		if (vs == m_vertexShader && ps == m_pixelShader)
			return;
		
		GLShaderVariation* glvs = (GLShaderVariation*)(vs);
		GLShaderVariation* glps = (GLShaderVariation*)(ps);
		//是否编译完成
		if (glvs && !glvs->GetGPUHandle())
		{
			if (glvs->GetCompilerOutput().empty())
			{
				//编译
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
				//编译
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
			for (unsigned i = 0; i < MAX_SHADER_PARAMETER_GROUPS * 2; ++i) //VertexShader和PixelShader各占一段，所以是两倍
			{
				ConstantBuffer* buffer = constantBuffers[i].Get();
				if (buffer != m_currentConstantBuffers[i])
				{
					unsigned handle = buffer ? buffer->GetGPUHandle() : 0;
					glBindBufferBase(GL_UNIFORM_BUFFER, i, handle); //绑定缓冲区对象
					m_curBoundUBO = handle; //当前绑定的ubo
					m_currentConstantBuffers[i] = buffer;  //设置当前绑定的ubo对象
					//清理全局参数源
					GLShaderProgram::ClearGlobalParameterSource((ShaderParameterGroup)(i % MAX_SHADER_PARAMETER_GROUPS));
				}
				
			}
			//是否设置自定义剪裁平面
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
		std::string ver = m_displayContext->GetApiVersion();
		if (!ver.empty() &&ver.size()>7)
		{
			if (ver[0] > '3')
			{
				m_gl3Support = true; //支持OpenGL3.0以上
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

		//重置所有渲染目标，深度缓冲区表面和视口，开始绘制新的一帧
		ResetRenderTargets();

		//清理前一帧使用的绑定纹理对象
		for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
		{
			m_pTextureMgr->SetTexture(0, (TextureUnit)i);
		}
		//开启颜色和深度写入模式
		SetColorWrite(true);
		SetDepthWrite(true);
		//发送事件
		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_BEGINRENDERING, NULL);

		return true;
	}

	void GLGraphicDriver::EndFrame()
	{
		if (!IsInitialized())
			return;

		FireEvent(ET_GRAPHIC_EVENT, EVENT_GRAPHICS_ENDRENDERING, NULL);
		//双缓冲交换
		m_displayContext->SwapBuffers();
		//清理临时的缓冲区
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

	void GLGraphicDriver::CleanFrameBuffers()
	{
		//先清理所有的fbo
		if (!IsDeviceLost())
		{
			//重新绑定回系统fbo
			BindFrameBuffer(m_sysFBO);//绑定fbo到系统fbo
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

		//先保存旧的状态
		bool oldColorWrite = m_bColorWrite;
		bool oldDepthWrite = m_bDepthWrite;
		
		if (flags & CLEAR_COLOR && !oldColorWrite)
			SetColorWrite(true);
		if (flags & CLEAR_DEPTH && !oldDepthWrite)
			SetDepthWrite(true);
		if (flags & CLEAR_STENCIL && m_stencilWriteMask != M_MAX_UNSIGNED)
			glStencilMask(M_MAX_UNSIGNED);

		unsigned glFlags = 0; //设置clear值 clear标志
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

		// 如果视口不是全屏幕，设置剪裁器限制clear

		IntVector2 viewSize = GetRenderTargetDimensions(); //获取rt范围
		if (m_viewport.left_ != 0 || m_viewport.top_ != 0 || m_viewport.right_ != viewSize.x_ || m_viewport.bottom_ != viewSize.y_)
			SetScissorTest(true, IntRect(0, 0, m_viewport.Width(), m_viewport.Height()));
		else
			SetScissorTest(false);

		glClear(glFlags); //执行clear

		SetScissorTest(false);
		SetColorWrite(oldColorWrite);//恢复先前的设置
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