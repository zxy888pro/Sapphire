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

			virtual void CreateRenderWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, int multiSample, bool bResizable, bool bVsync);

			virtual void CreateRenderWindow(int x, int y, int width, int height, bool bFullScreen, int multiSample, bool bResizable, bool bVsync);

			//关闭并销毁opengl渲染窗口
			virtual void CloseWindow();

			virtual void SwapBuffers();
			//终止OpenGL显示环境
			virtual void Terminate();

			virtual void* GetWindow();

			virtual void GetCurrentDisplayMode(DisplayMode& mode) const;

			virtual void GetDisplayMode(int index, DisplayMode& mode) const;

			virtual IntVector2 GetWindowSize() const;

			virtual IntVector2 GetWindowPos() const;

			virtual bool IsTerminated() const { return m_isTerminated; }
			
			virtual void* GetExternalWindow() const { return m_externalWindow; }

			virtual void SetExternalWindow(void* val);

			virtual void SetWindowTitle(const char* wndName);

			virtual const std::string& GetWindowTitle() const;

			virtual const std::string& GetDeviceName() const { return m_deviceName; }

			virtual const std::string& GetProviderName() const { return m_providerName; }

			virtual const std::string& GetApiName() const { return m_ApiName; }

			virtual const std::string& GetApiVersion() const { return m_ApiVer; }

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
		bool		   m_bResizable;
		std::string    m_windowName;
		std::string    m_providerName;
		std::string    m_deviceName;
		std::string    m_ApiVer;
		std::string    m_ApiName;
	};

}