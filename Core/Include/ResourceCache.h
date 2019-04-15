#pragma once


#include "SapphireDef.h"
#include <SubSystem.h>
#include "BaseResource.h"



namespace Sapphire
{

	
	//��Դ�������
	class ResourceCache : public SubSystem
	{
		SAPPHIRE_SUBSYSTEM(ESST_RESOURCECACHE)

	public:

		ResourceCache();
		~ResourceCache();


	protected:

		size_t			m_nCurrentUsedMemory;	//��ǰ�ڴ�ռ��
		size_t			m_nMaximumMemory;		//����ڴ�����
		bool			m_bResourceReserved;   //�Ƿ�Ԥ�����ڴ�
		ResMapItor		m_CurrentResource;     //��ǰ��Դ������
		 

	};



}