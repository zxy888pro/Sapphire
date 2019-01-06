#include <GraphicDriver.h>
#include <GraphicException.h>
#include "Texture2D.h"
#include "TextureMgr.h"
#include "ImageMgr.h"
#include "IIndexBuffer.h"
#include "IVertexBuffer.h"
#include "VertexBuffer.h"
#include "RenderSurface.h"

namespace Sapphire
{

	// ����OpenGLӳ�䶥������,������Ƥ����Ҫ�õ�,Ҫ����GLES2 ��Ƥbug��gles�豸ֻ֧�ֵ�8
	static const unsigned glVertexAttrIndex[] =
	{
		0, 1, 2, 3, 4, 8, 9, 5, 6, 7, 10, 11, 12
	};

	GraphicDriver::GraphicDriver()
	{
		m_pTextureMgr = new TextureMgr();
		m_pImageMgr = new ImageMgr();
		m_nTextureQuality = QUALITY_HIGH;
		
	}

	GraphicDriver::~GraphicDriver()
	{

	}

	void GraphicDriver::Init()
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
		CheckFeature();
	}

	void GraphicDriver::Release()
	{
		safeDelete(m_pImageMgr);	
		safeDelete(m_pTextureMgr);
		
	}

 

	void GraphicDriver::BindTexture(ITexture* pTexture, TextureUnit unit)
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

	void GraphicDriver::BindVBO(uint uHwUID)
	{
		if (m_curBindVBO != uHwUID)
		{
			if (uHwUID)
			{
				glBindBuffer(GL_ARRAY_BUFFER, uHwUID);
				m_curBindVBO = uHwUID;
			}
		}
	}

	void GraphicDriver::SetIndexBuffer(IIndexBuffer* pIndexBuffer)
	{
		if (m_indexBuffer == pIndexBuffer)
			return;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIndexBuffer ? pIndexBuffer->GetUID() : 0);  //������������
		m_indexBuffer = pIndexBuffer;
	}

	void GraphicDriver::ResetRenderTarget(uint index)
	{
		SetRenderTarget(index, (RenderSurface*)NULL);
	}

	Sapphire::RenderSurface* GraphicDriver::GetRenderTarget(uint index) const
	{
		return index < MAX_RENDERTARGETS ? m_renderTargets[index] : 0;
	}

	Sapphire::RenderSurface* GraphicDriver::GetDepthStencil() const
	{
		return m_depthStencil;
	}

	void GraphicDriver::ResetDepthStencil()
	{
		SetDepthStencil((RenderSurface*)0);
	}

	void GraphicDriver::CleanupRenderSurface(RenderSurface* surface)
	{

	}

	bool GraphicDriver::IsDeviceLost()
	{
		return false;
	}

	uint GraphicDriver::GetMaxAnisotropyLevels()
	{
		GLfloat maxAniLevel;    //��ѯ����ĸ�����������
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniLevel);
		return (uint)maxAniLevel;
	}

	PixelFormat GraphicDriver::GetPixelFormat(ImageType eImgType)
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

	PixelFormat GraphicDriver::GetPixelFormat(std::string szImageType)
	{
		if (m_imagetypeNames.find(szImageType) != m_imagetypeNames.end())
		{
			return GetPixelFormat(m_imagetypeNames[szImageType]);
		}
		return PF_UNDEFINED;
	}

	Sapphire::IVertexBuffer* GraphicDriver::GetVertexBuffer(uint index) const
	{
		return index < MAX_VERTEX_STREAMS ? m_vertexBuffers[index] : 0;
	}

	bool GraphicDriver::SetVertexBuffers(const std::vector<IVertexBuffer*>& buffers, const std::vector<uint>& elememtMasks, uint instOffset)
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

	void GraphicDriver::SetVertexBuffer(IVertexBuffer* vertexBuffer)
	{
		//ע�����ﲻ�Ƕ��̰߳�ȫ��
		static std::vector<IVertexBuffer*> vertexBuffers(1);
		static std::vector<unsigned> elementMasks(1);
		vertexBuffers[0] = vertexBuffer;
		elementMasks[0] = MASK_DEFAULT;

		SetVertexBuffers(vertexBuffers, elementMasks);
	}

	void GraphicDriver::SetRenderTarget(unsigned index, RenderSurface* renderTarget)
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

	void GraphicDriver::SetRenderTarget(unsigned index, Texture2D* texture)
	{
		/*RenderSurface* renderTarget = 0;
		if (texture)
		renderTarget = texture->GetRenderSurface();

		SetRenderTarget(index, renderTarget);*/
	}

	void GraphicDriver::SetDepthStencil(RenderSurface* depthStencil)
	{

	}

	void GraphicDriver::SetDepthStencil(Texture2D* texture)
	{

	}

	Sapphire::IIndexBuffer* GraphicDriver::GetIndexBuffer() const
	{
		return m_indexBuffer;
	}


	void* GraphicDriver::ReserveScratchBuffer(ulong size)
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

	void GraphicDriver::FreeScratchBuffer(void* buffer)
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

	void GraphicDriver::CleanScratchBuffers()
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

	int GraphicDriver::GetHWTextureWarpParam(TextureAddressMode mode)
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


	int GraphicDriver::GetHWTextureFormat(PixelFormat eFormat)
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

	int GraphicDriver::GetSWTextureFormat(PixelFormat eFormat)
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


	uint GraphicDriver::GetHWTextureDataType(PixelFormat ePformat)
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

	int GraphicDriver::GetHWAlphaFormat()
	{
		return GL_ALPHA;
	}

	int GraphicDriver::GetHWLuminanceFormat()
	{
		return GL_LUMINANCE;
	}

	int GraphicDriver::GetHWLuminanceAlphaFormat()
	{
		return GL_LUMINANCE_ALPHA;
	}

	int GraphicDriver::GetHWRGBFormat()
	{
		return GL_RGB;
	}

	int GraphicDriver::GetHWRGBAFormat()
	{
		return GL_RGBA;
	}

	void GraphicDriver::CheckFeature()
	{
		m_bAnisotropySupport = GLEW_EXT_texture_filter_anisotropic != 0;
		m_nMaxTextureUnits = GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS;
	}

}