#pragma once
#include "Sapphire.h"


namespace Sapphire
{

	struct DisplayMode
	{
		int		width;
		int		height;
		short	redBits;
		int		greenBits;
		int		blueBits;
		int		refreshRate;
	};

	struct IDisplayContext
	{
		virtual void Initialize() = 0;

		virtual void SetContextAttribute(int attr, int val) = 0;

		virtual void CreateRenderWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, int multiSample, bool bResizable, bool bVsync) = 0;

		virtual void CreateRenderWindow(int x, int y, int width, int height, bool bFullScreen, int multiSample, bool bResizable, bool bVsync) = 0;

		virtual void GetCurrentDisplayMode(DisplayMode& mode) const = 0;

		virtual void GetDisplayMode(int index, DisplayMode& mode) const = 0;

		virtual void SwapBuffers() = 0;

		virtual void Terminate() = 0;

		virtual void* GetWindow() = 0;

		//关闭并销毁opengl渲染窗口
		virtual void CloseWindow() = 0;

		virtual IntVector2 GetWindowSize() const = 0;

		virtual IntVector2 GetWindowPos() const = 0;

		virtual void* GetExternalWindow() const = 0;

		virtual void SetExternalWindow(void* val) = 0;

		virtual void SetWindowTitle(const char* wndName) = 0;

		virtual const std::string& GetWindowTitle() const = 0;

		virtual const std::string& GetDeviceName() const = 0;

		virtual const std::string& GetProviderName() const = 0;

		virtual const std::string& GetApiName() const = 0;

		virtual const std::string& GetApiVersion() const = 0;
	};


}