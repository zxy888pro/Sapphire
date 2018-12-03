#pragma once

#include <Graphics.h>



namespace Sapphire
{
	class GPUObject
	{
	public:
		GPUObject(){};
		virtual ~GPUObject(){};

		virtual void Release();

		uint GetGPUHandle(){ return m_uHwUID; }

		virtual void OnDeviceLost();

		virtual void OnDeviceReset() { }

		virtual void ClearDataLostFlag();

		bool IsDataLost() const { return m_bDataLost; }

		bool HasPendingData() const { return m_bDataPending; }
		

	protected:

		uint m_uHwUID;
		//���ݶ�ʧ
		bool m_bDataLost;
		//���ݵȴ����µ�GPU
		bool m_bDataPending;
		
	};
}
