#pragma once

#include <Graphics.h>



namespace Sapphire
{
	class IGraphicDriver;

	//GPU对象 texture/shader/Shaderprogram
	class GPUObject
	{
	public:

		GPUObject(IGraphicDriver* pdriver);

		virtual ~GPUObject(){};

		virtual void Release();

		uint GetGPUHandle(){ return m_uHwUID; }

		virtual void OnDeviceLost();

		virtual void OnDeviceReset() { }

		virtual void ClearDataLostFlag();

		bool IsDataLost() const { return m_bDataLost; }

		bool HasPendingData() const { return m_bDataPending; }
		
		const std::string&	GetUUID() const { return m_uuid; }

		

	protected:

		IGraphicDriver*  m_pGraphicDriver;
		uint m_uHwUID;
		//数据丢失
		bool m_bDataLost;
		//数据等待更新到GPU
		bool m_bDataPending;
		std::string  m_uuid;
	};
}
