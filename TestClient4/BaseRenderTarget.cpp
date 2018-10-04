#include "BaseRenderTarget.h"
#include <Sapphire.h>
#include <logUtil.h>
#include <GraphicDriver.h>
#include <ITextureMgr.h>

namespace Sapphire
{

	BaseRenderTarget::BaseRenderTarget()
	{

	}

	BaseRenderTarget::~BaseRenderTarget()
	{

	}

	void BaseRenderTarget::BindRT()
	{

	}

	void BaseRenderTarget::UnBindRT()
	{

	}

	 

	void BaseRenderTarget::Initialize(int width/*=800*/, int height/*=600*/, int defaultAttchmentType /*= 0/* 0纹理1渲染缓冲对象 */)
	{

		Release();
		m_width = width;
		m_height = height;
		CreateFBO(&m_fbo, defaultAttchmentType);
	}

	void BaseRenderTarget::Release()
	{
		bool bRet = glIsFramebuffer(m_fbo);
		if (bRet)
		{
			glDeleteFramebuffers(1, &m_fbo);
		}
	}

	void BaseRenderTarget::AddAttachement(int attachmentType)
	{

	}

	bool BaseRenderTarget::CreateFBO(GLuint* fbo, int defaultAttchmentType)
	{
		glGenFramebuffers(1, fbo);

		//检查完整性
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
			return true;
		else
		{
			LogUtil::LogMsgLn("Check FBO Completion failed !");
			return false;
		}
			
	}

	bool BaseRenderTarget::CreateTexAttachment(uint index)
	{
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		m_assert(pGd);
		ITextureMgr* pTexMgr = pGd->getTextureMgr();
		m_assert(pTexMgr);
		ITexture* pTex = pTexMgr->CreateEmptyTexture2D(m_width, m_height,PF_R8G8B8);
		m_texMap[index] = pTex->getUID();
		
	}

}