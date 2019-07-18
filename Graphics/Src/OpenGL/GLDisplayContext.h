#pragma once
#include "Sapphire.h"
#include "Graphics.h"
#include "IDisplayContext.h"


namespace Sapphire
{

	//�ײ�OpenGL��ʾ����
	class GLDisplayContext : public IDisplayContext
	{
	public:
		GLDisplayContext();
		virtual ~GLDisplayContext();
		

			virtual void Initialize();

			virtual void SetContextAttribute(int attr, int val);

			virtual void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, int m_multiSample, bool bVsync);

			//�رղ�����opengl��Ⱦ����
			virtual void CloseWindow();

			virtual void SwapBuffers();
			//��ֹOpenGL��ʾ����
			virtual void Terminate();

			virtual void* GetWindow();

			virtual void GetCurrentDisplayMode(DisplayMode& mode) const;

			virtual void GetDisplayMode(int index, DisplayMode& mode) const;

			IntVector2 GetWindowSize() const;

			IntVector2 GetWindowPos() const;

			bool IsTerminated() const { return m_isTerminated; }

			//��������
			void  Clear(uint flags, Color& color, float depth, uint stencil);
			

			void* GetExternalWindow() const { return m_externalWindow; }
			void SetExternalWindow(void* val);
	private:

		void*          m_mainWindow;
		void*          m_externalWindow;   ////Ƕ����ⲿ�����ھ��
		bool           m_isTerminated;
		int            m_width;
		int            m_height;
		int            m_posX;
		int            m_posY;
		bool		   m_bFullScreen;
		bool		   m_bVsync;
		int            m_multiSample;
		std::string    m_windowName;
	};

}