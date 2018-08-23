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
		virtual void GPUObjectInit(void* pData);
		virtual void SetData(void* pData);
		virtual void Activate();
		virtual void Deactivate();
		virtual void Update(void* pData);
		uint GetGPUHandle(){ return m_uHwUID; }
		

	protected:

		uint m_uHwUID;

	};
}
