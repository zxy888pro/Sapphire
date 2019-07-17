#pragma once
#include "Sapphire.h"
#include "Graphics.h"
#include "IDisplayContext.h"


namespace Sapphire
{

	//底层OpenGL显示环境
	class GLDisplayContext : public IDisplayContext
	{
	public:
		GLDisplayContext();
		virtual ~GLDisplayContext();
		

			virtual void Initialize();

			virtual void SetContextAttribute(int attr, int val);

			virtual void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, bool bVsync);

			virtual void CloseWindow();

			virtual void SwapBuffers();

			virtual void Terminate();

			virtual void* GetWindow();

			virtual void GetCurrentDisplayMode(DisplayMode& mode) const;

			virtual void GetDisplayMode(int index, DisplayMode& mode) const;

			IntVector2 GetWindowSize() const;

			IntVector2 GetWindowPos() const;

			bool IsTerminated() const { return m_isTerminated; }
			

			void* GetExternalWindow() const { return m_externalWindow; }
			void SetExternalWindow(void* val);
	private:

		void*          m_mainWindow;
		void*          m_externalWindow;   //外部窗口句柄
		bool           m_isTerminated;
		int            m_width;
		int            m_height;
		int            m_posX;
		int            m_posY;
		bool		   m_bFullScreen;
		bool		   m_bVsync;
		std::string    m_windowName;
	};

}