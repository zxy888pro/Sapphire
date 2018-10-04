#pragma once

#include <SapphireDef.h>
#include "stdafx.h"

namespace Sapphire
{


	//��ȾĿ��
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void BindRT();
		void UnBindRT();
		

		void Initialize(int width=800, int height=600, int defaultAttchmentType = 0/*  0����1��Ⱦ������� */);
		void Release();

	protected:
		
		void AddAttachement(int attachmentType);		
		//һ��������֡������Ҫ�������µ�����
		//��������һ�����壨��ɫ����Ȼ�ģ�建�壩
		//������һ����ɫ����(Attachment)��
		//���еĸ����������������ģ��������ڴ棩
		//ÿ�����嶼Ӧ������ͬ����������
		GLuint m_fbo;  //fbo����
		int    m_width;
		int    m_height;
		bool CreateFBO(GLuint*  fbo, int defaultAttchmentType);

		bool CreateTexAttachment(uint index);
		bool CreateRBOAttachment();

		std::map<uint, uint> m_texMap;
		

	};
}