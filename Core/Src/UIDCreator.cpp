#include "UIDCreator.h"
#include <stdint.h>
#include "Atom.h"

namespace Sapphire
{

	UIDCreator::UIDCreator()
	{
		m_uSeqId = 0;
	}

	UINT UIDCreator::GetNewUID()
	{
		m_lock.Lock();
		++m_uSeqId;
		m_lock.Unlock();
		return m_uSeqId;
	}

	 
}