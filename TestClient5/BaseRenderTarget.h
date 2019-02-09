#pragma once

#include <SapphireDef.h>
#include "stdafx.h"

namespace Sapphire
{
	enum  ERenderTargetType
	{
		ERTT_READ,
		ERTT_WRITE,
		ERTT_READ_WRITE
	};

	class ITexture;
	class IGraphicDriver;
	//渲染目标
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void BindRT();
		void UnBindRT();
		
		ITexture* GetTexture(int index);
		void Initialize(int width = 800, int height = 600, int defaultAttchmentType = 0/*  0纹理1渲染缓冲对象 */, ERenderTargetType = ERTT_READ_WRITE);
		void Release();

	protected:
			
		//一个完整的帧缓冲需要满足以下的条件
		//附加至少一个缓冲（颜色、深度或模板缓冲）
		//至少有一个颜色附件(Attachment)。
		//所有的附件都必须是完整的（保留了内存）
		//每个缓冲都应该有相同的样本数。
		GLuint m_fbo;  //fbo对象
		GLuint m_rbo;  //rbo对象
		int    m_width;
		int    m_height;
		bool CreateFBO(GLuint*  fbo, int defaultAttchmentType);

		bool CreateTexAttachment(uint index);
		bool CreateRBOAttachment();
		ERenderTargetType m_eRttType;
		std::map<uint, ITexture*> m_texMap;
		IGraphicDriver* m_pDriver;
		

	};
}