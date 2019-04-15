#pragma once


#include "SapphireDef.h"
#include <SubSystem.h>
#include "BaseResource.h"



namespace Sapphire
{

	
	//资源缓存管理
	class ResourceCache : public SubSystem
	{
		SAPPHIRE_SUBSYSTEM(ESST_RESOURCECACHE)

	public:

		ResourceCache();
		~ResourceCache();


	protected:

		size_t			m_nCurrentUsedMemory;	//当前内存占用
		size_t			m_nMaximumMemory;		//最大内存上限
		bool			m_bResourceReserved;   //是否预分配内存
		ResMapItor		m_CurrentResource;     //当前资源迭代器
		 

	};



}