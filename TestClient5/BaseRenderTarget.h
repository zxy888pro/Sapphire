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
	//��ȾĿ��
	class BaseRenderTarget
	{
	public:

		BaseRenderTarget();
		virtual ~BaseRenderTarget();

		void BindRT();
		void UnBindRT();
		
		ITexture* GetTexture(int index);
		void Initialize(int width = 800, int height = 600, int defaultAttchmentType = 0/*  0����1��Ⱦ������� */, ERenderTargetType = ERTT_READ_WRITE);
		void Release();

	protected:
			
		//һ��������֡������Ҫ�������µ�����
		//��������һ�����壨��ɫ����Ȼ�ģ�建�壩
		//������һ����ɫ����(Attachment)��
		//���еĸ����������������ģ��������ڴ棩
		//ÿ�����嶼Ӧ������ͬ����������
		GLuint m_fbo;  //fbo����
		GLuint m_rbo;  //rbo����
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