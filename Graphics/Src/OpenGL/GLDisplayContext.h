#pragma once
#include "Sapphire.h"
#include "Graphics.h"


namespace Sapphire
{
	//�ײ�OpenGL��ʾ����
	class GLDisplayContext
	{
	public:
		GLDisplayContext();
		virtual ~GLDisplayContext();
		

		void Init();

		void SetContextAttribute(int attr, int val);

		void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, bool bVsync);

		void SwapBuffers();

		void Terminate();

	private:

		void* m_mainWindow;

	};

}