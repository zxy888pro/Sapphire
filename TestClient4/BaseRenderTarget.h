#pragma once

#include <SapphireDef.h>
#include "stdafx.h"

namespace Sapphire
{


	//渲染目标
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void BindRT();
		void UnBindRT();
		

		void Initialize(int width=800, int height=600, int defaultAttchmentType = 0/*  0纹理1渲染缓冲对象 */);
		void Release();

	protected:
		
		void AddAttachement(int attachmentType);		
		//一个完整的帧缓冲需要满足以下的条件
		//附加至少一个缓冲（颜色、深度或模板缓冲）
		//至少有一个颜色附件(Attachment)。
		//所有的附件都必须是完整的（保留了内存）
		//每个缓冲都应该有相同的样本数。
		GLuint m_fbo;  //fbo对象
		int    m_width;
		int    m_height;
		bool CreateFBO(GLuint*  fbo, int defaultAttchmentType);

		bool CreateTexAttachment(uint index);
		bool CreateRBOAttachment();

		std::map<uint, uint> m_texMap;
		

	};
}