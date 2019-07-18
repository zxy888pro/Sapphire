#include "CommonApi.h"
#include "GPUObject.h"
#include "GLGraphicDriver.h"

namespace Sapphire
{

	GPUObject::GPUObject(GLGraphicDriver* pdriver) :m_pGraphicDriver(pdriver)
	{
		m_uuid = GetUUIDStr();
		if (pdriver)
			m_pGraphicDriver->AddGPUObject(this);
	}

	void GPUObject::Release()
	{
		if (m_pGraphicDriver)
			m_pGraphicDriver->RemoveGPUObject(this);
	}

	void GPUObject::OnDeviceLost()
	{
		m_uHwUID = 0;
	}

	void GPUObject::ClearDataLostFlag()
	{
		m_bDataLost = false;
	}

}