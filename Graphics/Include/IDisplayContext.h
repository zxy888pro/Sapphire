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

		virtual void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, bool bVsync) = 0;

		virtual void SwapBuffers() = 0;

		virtual void Terminate() = 0;

		virtual void* GetWindow() = 0;
	};


}