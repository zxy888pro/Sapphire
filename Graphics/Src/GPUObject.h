#pragma once

#include <Graphics.h>



namespace Sapphire
{
	class GPUObject
	{
	public:
		GPUObject();
		virtual ~GPUObject();

		virtual void Release();

		virtual void GPUObjectInit();

		uint GetGPUHandle(){ return m_uHwUID; }

		virtual void OnDeviceLost() { }

		virtual void OnDeviceReset() { }
		

	protected:

		uint m_uHwUID;
		
	};
}
