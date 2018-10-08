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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	}

	void BaseRenderTarget::UnBindRT()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	 

	Sapphire::ITexture* BaseRenderTarget::GetTexture(int index)
	{
		if (m_texMap.find(index) != m_texMap.end())
		{
			return m_texMap[index];
		}
		return NULL;
	}

	void BaseRenderTarget::Initialize(int width/*=800*/, int height/*=600*/, int defaultAttchmentType /*= 0/* 0����1��Ⱦ������� */, ERenderTargetType eRttType)
	{

		Release();
		m_width = width;
		m_height = height;
		m_eRttType = eRttType;
		CreateFBO(&m_fbo, defaultAttchmentType);
	}

	void BaseRenderTarget::Release()
	{
		bool bRet = glIsFramebuffer(m_fbo);
		if (bRet)
		{
			glDeleteFramebuffers(1, &m_fbo);
		}
		GraphicDriver* pGd = GraphicDriver::GetSingletonPtr();
		m_assert(pGd);
		ITextureMgr* pTexMgr = pGd->getTextureMgr();
		m_assert(pTexMgr);
		for (std::map<uint, ITexture*>::iterator it = m_texMap.begin(); it != m_texMap.end(); ++it)
		{
			if (glIsTexture(it->second->getUID()))
			{
				pTexMgr->DiscardTexture(it->second);
			}
		}
		m_texMap.clear();
	}


	bool BaseRenderTarget::CreateFBO(GLuint* fbo, int defaultAttchmentType)
	{
		glGenFramebuffers(1, fbo);

		
		//FBO����ֻ��/ֻд/��д GL_READ_FRAMEBUFFER/GL_DRAW_FRAMEBUFFER/GL_FRAMEBUFFER
		switch (m_eRttType)
		{
		case Sapphire::ERTT_READ:
			glBindFramebuffer(GL_READ_FRAMEBUFFER, *fbo);
			break;
		case Sapphire::ERTT_WRITE:
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, *fbo);
			break;
		case Sapphire::ERTT_READ_WRITE:
			glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
			break;
		default:
			glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
			break;
		}
		CreateTexAttachment(0);
		CreateRBOAttachment();
		//���������
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{ 
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return true;
		}
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

		if (m_texMap.find(index) != m_texMap.end() && glIsTexture(m_texMap[index]->getUID()))
		{
			pTexMgr->DiscardTexture(m_texMap[index]);
			m_texMap[index] = 0;
		}
		ITexture* pTex = pTexMgr->CreateEmptyTexture2D(m_width, m_height,PF_A8R8G8B8);
		pTexMgr->SetTexture(pTex, TU_POSTEFFECT);
		m_texMap[index] = pTex;
		
		//֡�����������  
		//target��֡�����Ŀ�꣨���ơ���ȡ�������߽���GL_READ_FRAMEBUFFER/GL_DRAW_FRAMEBUFFER/GL_FRAMEBUFFER�� 
		//attachment��������Ҫ���ӵĸ������͡�
		//textarget����ϣ�����ӵ���������
		//texture��Ҫ���ӵ��������
		//level��mipmap�ȼ�
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, pTex->getUID(), 0); //����������ӵ�FBO��
	}

	bool BaseRenderTarget::CreateRBOAttachment()
	{
		/*
		��Ⱦ�������(Renderbuffer Object)��������֮�����뵽OpenGL�У���Ϊһ�����õ�֡���帽�����͵ģ������ڹ�ȥ������Ψһ���õĸ�����
		������ͼ��һ������Ⱦ���������һ�������Ļ��壬��һϵ�е��ֽڡ����������صȡ���Ⱦ������󸽼ӵĺô��ǣ����Ὣ���ݴ���ΪOpenGLԭ������Ⱦ��ʽ������Ϊ������Ⱦ��֡�����Ż����ġ�
		��Ⱦ�������ֱ�ӽ����е���Ⱦ���ݴ��浽���Ļ����У��������κ���������ʽ��ת����������Ϊһ������Ŀ�д������ʡ�
		��Ȼ����Ȼ�����ܹ�ʹ��glReadPixels����ȡ�������ӵ�ǰ�󶨵�֡���壬�����Ǹ��������з����ض����������.
		��Ϊ���������Ѿ���ԭ���ĸ�ʽ�ˣ���д����߸����������ݵ�����������ʱ�Ƿǳ���ġ����ԣ��������������Ĳ�����ʹ����Ⱦ�������ʱ��ǳ��졣������ÿ����Ⱦ�������ʹ�õ�glfwSwapBuffers��
		Ҳ����ͨ����Ⱦ�������ʵ�֣�ֻ��Ҫд��һ����Ⱦ����ͼ�񣬲�����󽻻�������һ����Ⱦ����Ϳ����ˡ���Ⱦ�����������ֲ����ǳ�������
		������Ⱦ�������ͨ������ֻд�ģ����ǻᾭ��������Ⱥ�ģ�帽������Ϊ�󲿷�ʱ�����Ƕ�����Ҫ����Ⱥ�ģ�建���ж�ȡֵ��ֻ������Ⱥ�ģ����ԡ�
		*/
		if (glIsRenderbuffer(m_rbo))
		{
			glDeleteRenderbuffers(1, &m_rbo);
		}
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		//���ô洢��ʽ
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		return true;
	}

}