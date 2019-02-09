#include "BaseRenderTarget.h"
#include <Sapphire.h>
#include <logUtil.h>
#include <IGraphicDriver.h>
#include <ITextureMgr.h>

namespace Sapphire
{

	BaseRenderTarget::BaseRenderTarget()
	{
		m_pDriver = dynamic_cast<IGraphicDriver*>(Core::GetSingletonPtr()->GetSubSystemWithType(Sapphire::ESST_GRAPHICDRIVER));
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

	void BaseRenderTarget::Initialize(int width/*=800*/, int height/*=600*/, int defaultAttchmentType /*= 0/* 0纹理1渲染缓冲对象 */, ERenderTargetType eRttType)
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
		m_assert(m_pDriver);
		ITextureMgr* pTexMgr = m_pDriver->getTextureMgr();
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

		
		//FBO可以只读/只写/读写 GL_READ_FRAMEBUFFER/GL_DRAW_FRAMEBUFFER/GL_FRAMEBUFFER
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
		//检查完整性
		int ret = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (ret == GL_FRAMEBUFFER_COMPLETE)
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
		m_assert(m_pDriver);
		ITextureMgr* pTexMgr = m_pDriver->getTextureMgr();
		m_assert(pTexMgr);

		if (m_texMap.find(index) != m_texMap.end() && glIsTexture(m_texMap[index]->getUID()))
		{
			pTexMgr->DiscardTexture(m_texMap[index]);
			m_texMap[index] = 0;
		}
		ITexture* pTex = pTexMgr->CreateEmptyTexture2D(m_width, m_height,PF_A8R8G8B8);
		pTexMgr->SetTexture(pTex, TU_POSTEFFECT);
		m_texMap[index] = pTex;

		bool b = glIsTexture(pTex->getUID());

		
		//帧缓冲的纹理附件  
		//target：帧缓冲的目标（绘制、读取或者两者皆有GL_READ_FRAMEBUFFER/GL_DRAW_FRAMEBUFFER/GL_FRAMEBUFFER） 
		//attachment：我们想要附加的附件类型。
		//textarget：你希望附加的纹理类型
		//texture：要附加的纹理对象
		//level：mipmap等级
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+index, GL_TEXTURE_2D, pTex->getUID(), 0); //将纹理附件添加到FBO上
		GLenum errorcode = glGetError();
		if (errorcode != 0)
		{
			SAPPHIRE_LOGERROR("Create TexAttachment Failed!");
		}
	}

	bool BaseRenderTarget::CreateRBOAttachment()
	{
		/*
		渲染缓冲对象(Renderbuffer Object)是在纹理之后引入到OpenGL中，作为一个可用的帧缓冲附件类型的，所以在过去纹理是唯一可用的附件。
		和纹理图像一样，渲染缓冲对象是一个真正的缓冲，即一系列的字节、整数、像素等。渲染缓冲对象附加的好处是，它会将数据储存为OpenGL原生的渲染格式，它是为离屏渲染到帧缓冲优化过的。
		渲染缓冲对象直接将所有的渲染数据储存到它的缓冲中，不会做任何针对纹理格式的转换，让它变为一个更快的可写储存介质。
		当然你仍然还是能够使用glReadPixels来读取它，这会从当前绑定的帧缓冲，而不是附件本身，中返回特定区域的像素.
		因为它的数据已经是原生的格式了，当写入或者复制它的数据到其它缓冲中时是非常快的。所以，交换缓冲这样的操作在使用渲染缓冲对象时会非常快。我们在每个渲染迭代最后使用的glfwSwapBuffers，
		也可以通过渲染缓冲对象实现：只需要写入一个渲染缓冲图像，并在最后交换到另外一个渲染缓冲就可以了。渲染缓冲对象对这种操作非常完美。
		由于渲染缓冲对象通常都是只写的，它们会经常用于深度和模板附件，因为大部分时间我们都不需要从深度和模板缓冲中读取值，只关心深度和模板测试。
		*/
		if (glIsRenderbuffer(m_rbo))
		{
			glDeleteRenderbuffers(1, &m_rbo);
		}
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		//设置存储格式
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
		return true;
	}

}