#include "GPUObject.h"


namespace Sapphire
{

	void GPUObject::Release()
	{

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