#pragma once
#include "Sapphire.h"
#include "Graphics.h"
#include "IDisplayContext.h"


namespace Sapphire
{
	//µ◊≤„OpenGLœ‘ æª∑æ≥
	class GLDisplayContext : public IDisplayContext
	{
	public:
		GLDisplayContext();
		virtual ~GLDisplayContext();
		

		virtual void Initialize();

			virtual void SetContextAttribute(int attr, int val);

			virtual void CreateNativeWindow(const char* wndName, int x, int y, int width, int height, bool bFullScreen, bool bVsync);

			virtual void SwapBuffers();

			virtual void Terminate();

			virtual void* GetWindow();

	private:

		void* m_mainWindow;

	};

}