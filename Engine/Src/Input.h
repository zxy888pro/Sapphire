#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	//���������������
	class SAPPHIRE_CLASS Input : public BaseObject
	{
		SAPPHIRE_OBJECT(Input, BaseObject)
	public:
		Input(Core* pCore);
		virtual ~Input();


		 

		//��������Ϣ����ÿ֡��ʼǰ����
		void Update();


	};
}