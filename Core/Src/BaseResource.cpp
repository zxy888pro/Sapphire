#include "BaseResource.h"

bool Sapphire::BaseResource::operator < (BaseResource& container)
{
	//�Ƚ���Դ���ȼ�
	if (GetPriority() < container.GetPriority())
		return true;
	else if (GetPriority() > container.GetPriority())
		return false;
	else
	{
		//�ٱȽ���Դ������
		if (m_LastAccess < container.GetLastAccess())
			return true;
		else if (m_LastAccess > container.GetLastAccess())
			return false;
		else
		{
			//���Ƚϴ�С
			if (GetSize() < container.GetSize())
				return true;
			else
				return false;
		}
	}
	return false;
}
