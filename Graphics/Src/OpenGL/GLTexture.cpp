#include "GLTexture.h"
#include "IGraphicDriver.h"
#include "GLGraphicDriver.h"


namespace Sapphire
{

	GLTexture::GLTexture(Core* pCore, IGraphicDriver* pDriver, const char* resName /*= ""*/):
		BaseResource(pCore),
		GPUObject(pDriver),
		m_uWidth(0),
		m_uHeight(0),
		m_uDepth(0),
		m_bIsCompress(false),
		m_requestLevel(0),
		m_maxMipLevel(0),
		m_uNumMipmaps(1),
		m_bIsDisposed(true),
		m_uAnisotropyLevel(8),
		m_ePixelFormat(PixelFormat::PF_UNDEFINED),
		m_glTexFormat(0),
		m_eFilterMode(TextureFilterMode::FILTER_NEAREST),
		m_eUsage(TextureUsage::TEXTURE_STATIC),
		m_szName(""),
		m_bSRGB(false),
		m_glTexTarget(0),
		m_eType(ResoureType_Texture)
	{
		for (int i = 0; i < MAX_COORDS; ++i)
			m_eAddressMode_[i] = ADDRESS_REPEAT; m_eAddressMode_;
		for (int i = 0; i < MAX_TEXTURE_QUALITY_LEVELS; ++i)
			m_skipMips[i] = (unsigned)(MAX_TEXTURE_QUALITY_LEVELS - 1 - i);

	}

	GLTexture::~GLTexture()
	{

	}

	void GLTexture::Release()
	{

	}

	bool GLTexture::Recreate()
	{
		return false;
	}

	void GLTexture::Dispose()
	{

	}

	bool GLTexture::IsDisposed()
	{
		return true;
	}

	size_t GLTexture::GetSize()
	{
		return m_uSize;
	}

	uint GLTexture::GetDataSize() const
	{
		if (IsCompressed())
		{
			if (m_glTexFormat == COMPRESSED_RGB_PVRTC_4BPPV1_IMG || m_glTexFormat == COMPRESSED_RGBA_PVRTC_4BPPV1_IMG)
				return (unsigned)((MAX(m_uWidth, 8) * MAX(m_uHeight, 8) * 4 + 7) >> 3);
			else if (m_glTexFormat == COMPRESSED_RGB_PVRTC_2BPPV1_IMG || m_glTexFormat == COMPRESSED_RGBA_PVRTC_2BPPV1_IMG)
				return (unsigned)((MAX(m_uWidth, 16) * MAX(m_uHeight, 8) * 2 + 7) >> 3);
			else
				return GetRowSize() * ((m_uHeight + 3) >> 2);
		}
		else
			return GetRowSize() * m_uHeight;
	}

	uint GLTexture::GetRowSize() const
	{
		switch (m_glTexFormat)
		{
		case GL_ALPHA:
		case GL_LUMINANCE:
			return (unsigned)m_uWidth;

		case GL_LUMINANCE_ALPHA:
			return (unsigned)(m_uWidth * 2);

		case GL_RGB:
			return (unsigned)(m_uWidth * 3);

		case GL_RGBA:
#ifndef GL_ES_VERSION_2_0
		case GL_DEPTH24_STENCIL8_EXT:
		case GL_RG16:
		case GL_RG16F:
		case GL_R32F:
#endif
			return (unsigned)(m_uWidth * 4);

#ifndef GL_ES_VERSION_2_0
		case GL_R8:
			return (unsigned)m_uWidth;

		case GL_RG8:
		case GL_R16F:
			return (unsigned)(m_uWidth * 2);

		case GL_RGBA16:
		case GL_RGBA16F_ARB:
			return (unsigned)(m_uWidth * 8);

		case GL_RGBA32F_ARB:
			return (unsigned)(m_uWidth * 16);
#endif

		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			return (unsigned)(((m_uWidth + 3) >> 2) * 8);

		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			return (unsigned)(((m_uWidth + 3) >> 2) * 16);

		case GL_ETC1_RGB8_OES:
			return (unsigned)(((m_uWidth + 3) >> 2) * 8);

		case COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
		case COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
			return (unsigned)((m_uWidth * 4 + 7) >> 3);

		case COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
		case COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
			return (unsigned)((m_uWidth * 2 + 7) >> 3);

		default:
			return 0;
		}
	}

	void GLTexture::ReRequest()
	{

	}

	void GLTexture::OnDeviceLost()
	{

	}

	void GLTexture::OnDeviceReset()
	{

	}

	bool GLTexture::Create()
	{
		return false;
	}

	uint GLTexture::getLevelWidth(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uWidth >> level, 1);
	}

	uint GLTexture::getLevelHeight(uint level) const
	{
		if (level > m_uNumMipmaps)
			return 0;
		return MAX(m_uHeight >> level, 1);
	}

	Sapphire::ITexture* GLTexture::getBackupTexture() const
	{
		return m_backupTex;
	}

	uint GLTexture::getRequestMipLevel() const
	{
		return m_requestLevel;
	}

	void GLTexture::setRequestMipLevel(uint level)
	{
		m_requestLevel = level;
	}

	void GLTexture::setBackupTexture(ITexture* tex)
	{
		m_backupTex = dynamic_cast<GLTexture*>(tex);
	}

	bool GLTexture::IsCompressed() const
	{
		return m_glTexFormat == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || m_glTexFormat == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT ||
			m_glTexFormat == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT || m_glTexFormat == GL_ETC1_RGB8_OES ||
			m_glTexFormat == COMPRESSED_RGB_PVRTC_4BPPV1_IMG || m_glTexFormat == COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ||
			m_glTexFormat == COMPRESSED_RGB_PVRTC_2BPPV1_IMG || m_glTexFormat == COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
	}

	void GLTexture::SetSRGB(bool enable)
	{
		GLGraphicDriver* pDriver = dynamic_cast<GLGraphicDriver*>(m_pGraphicDriver);
		if (pDriver)
			enable &= pDriver->GetSRGBSupport();
		if (enable != m_bSRGB)
		{
			m_bSRGB = enable;
			// 如果纹理已经创建了，必须用sRGB纹理重新创建
			if (m_uHwUID)
				Create();
		
			// 如果帧缓存对象再用这个纹理，标记为脏
			if (pDriver && pDriver->GetRenderTarget(0) && pDriver->GetRenderTarget(0)->GetParentTexture() == this)
				pDriver->MarkFBODirty();
		}
	}

	bool GLTexture::GetSRGB() const
	{
		return m_bSRGB;
	}

	uint GLTexture::GetHWFormat() const
	{
		return m_glTexFormat;
	}

	void GLTexture::UpdateParameters()
	{
		if (!m_uHwUID || !m_pGraphicDriver)
			return;

		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_S, GLGraphicDriver::GetWarpMode(m_eAddressMode_[COORD_U]));
		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_T, GLGraphicDriver::GetWarpMode(m_eAddressMode_[COORD_V]));
#ifndef GL_ES_VERSION_2_0
		glTexParameteri(m_glTexTarget, GL_TEXTURE_WRAP_R, GLGraphicDriver::GetWarpMode(m_eAddressMode_[COORD_W]));
#endif

		TextureFilterMode filterMode = m_eFilterMode;
		if (filterMode == FILTER_DEFAULT)
			filterMode = m_pGraphicDriver->GetDefaultTextureFilterMode();

		switch (filterMode)
		{
		case FILTER_NEAREST:
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case FILTER_BILINEAR:
			if (m_uAnisotropyLevel < 2)
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		case FILTER_ANISOTROPIC:
		case FILTER_TRILINEAR:
			if (m_uAnisotropyLevel < 2)
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else
				glTexParameteri(m_glTexTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		default:
			break;
		}

#ifndef GL_ES_VERSION_2_0
		if (m_pGraphicDriver->GetAnisotropySupport())
		{
			glTexParameterf(m_glTexTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT,
				filterMode == FILTER_ANISOTROPIC ? (float)m_pGraphicDriver->GetTextureAnisotropy() : 1.0f);
		}

		// 阴影比较
		if (m_bShadowCompare)
		{
			glTexParameteri(m_glTexTarget, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(m_glTexTarget, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}
		else
			glTexParameteri(m_glTexTarget, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glTexParameterfv(m_glTexTarget, GL_TEXTURE_BORDER_COLOR, m_borderColor.Data());
#endif

		m_bParametersDirty = false;
	}

	void GLTexture::SetParameters(XMLFile* xmlFile)
	{

	}

	void GLTexture::SetParameter(const XMLElement& xmlElem)
	{

	}

	bool GLTexture::GetParametersDirty() const
	{
		return m_bParametersDirty;
	}


	void GLTexture::setPixelFormat(PixelFormat val)
	{
		 m_ePixelFormat = val; 
		 m_bParametersDirty = true;
	}

	void GLTexture::setFilterMode(Sapphire::TextureFilterMode val)
	{
		 m_eFilterMode = val; 
		 m_bParametersDirty = true;
	}

	void GLTexture::setAddressMode(TextureCoordinate crood, TextureAddressMode mode)
	{
		 m_eAddressMode_[crood] = mode; 
		 m_bParametersDirty = true;
	}

	uint GLTexture::getUID() const
	{
		return 0;
	}

	void GLTexture::RenderSurfaceUpdate()
	{

	}

	void GLTexture::SetBorderColor(const Color& color)
	{
		m_borderColor = color;
		m_bParametersDirty = true;
	}

	void GLTexture::SetShadowCompare(bool enable)
	{
		m_bShadowCompare = enable;
		m_bParametersDirty = true;
	}

	unsigned GLTexture::GetSRGBFormat(unsigned format)
	{
#ifndef GL_ES_VERSION_2_0
		if (!m_pGraphicDriver || !m_pGraphicDriver->GetSRGBSupport())
			return format;

		switch (format)
		{
		case GL_RGB:
			return GL_SRGB_EXT;
		case GL_RGBA:
			return GL_SRGB_ALPHA_EXT;
		case GL_LUMINANCE:
			return GL_SLUMINANCE_EXT;
		case GL_LUMINANCE_ALPHA:
			return GL_SLUMINANCE_ALPHA_EXT;
		case GL_COMPRESSED_RGBA_S3TC_DXT1_EXT:
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT;
		case GL_COMPRESSED_RGBA_S3TC_DXT3_EXT:
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT;
		case GL_COMPRESSED_RGBA_S3TC_DXT5_EXT:
			return GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT;
		default:
			return format;
		}
#else
		return format;
#endif
	}

}