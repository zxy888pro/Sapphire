#pragma once


#include "Sapphire.h"
#include "Thread.h"


namespace Sapphire
{
	class SAPPHIRE_CLASS UIDCreator
	{
	public: 
		UIDCreator();
		UINT GetNewUID();

	private:

		MutexEx  m_lock;
		volatile UINT64 m_uSeqId;

	};


}