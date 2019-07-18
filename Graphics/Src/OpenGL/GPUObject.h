#pragma once

#include <Graphics.h>



namespace Sapphire
{
	class GLGraphicDriver;

	//GPU���� texture/shader/Shaderprogram
	class GPUObject
	{
	public:

		GPUObject(GLGraphicDriver* pdriver);

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

		WeakPtr<GLGraphicDriver>  m_pGraphicDriver;
		uint m_uHwUID;
		//���ݶ�ʧ
		bool m_bDataLost;
		//���ݵȴ����µ�GPU
		bool m_bDataPending;
		std::string  m_uuid;
	};
}
