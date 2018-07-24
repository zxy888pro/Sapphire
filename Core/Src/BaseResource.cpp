#include "BaseResource.h"

bool Sapphire::BaseResource::operator < (BaseResource& container)
{
	//比较资源优先级
	if (GetPriority() < container.GetPriority())
		return true;
	else if (GetPriority() > container.GetPriority())
		return false;
	else
	{
		//再比较资源访问数
		if (m_LastAccess < container.GetLastAccess())
			return true;
		else if (m_LastAccess > container.GetLastAccess())
			return false;
		else
		{
			//最后比较大小
			if (GetSize() < container.GetSize())
				return true;
			else
				return false;
		}
	}
	return false;
}
