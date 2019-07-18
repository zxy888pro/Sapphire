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

			virtual void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, int m_multiSample, bool bVsync);

			//关闭并销毁opengl渲染窗口
			virtual void CloseWindow();

			virtual void SwapBuffers();
			//终止OpenGL显示环境
			virtual void Terminate();

			virtual void* GetWindow();

			virtual void GetCurrentDisplayMode(DisplayMode& mode) const;

			virtual void GetDisplayMode(int index, DisplayMode& mode) const;

			IntVector2 GetWindowSize() const;

			IntVector2 GetWindowPos() const;

			bool IsTerminated() const { return m_isTerminated; }

			//清理缓存区
			void  Clear(uint flags, Color& color, float depth, uint stencil);
			

			void* GetExternalWindow() const { return m_externalWindow; }
			void SetExternalWindow(void* val);
	private:

		void*          m_mainWindow;
		void*          m_externalWindow;   ////嵌入的外部主窗口句柄
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