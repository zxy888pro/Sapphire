#pragma once

#include "Sapphire.h"
#include <unordered_set>


namespace Sapphire
{

	class IGraphicDriver;

	//���������������
	class SAPPHIRE_CLASS Input : public SubSystem
	{
		SAPPHIRE_OBJECT(Input, SubSystem)
	public:
		Input(Core* pCore);
		virtual ~Input();

		//��ʼ�����������ϵͳ,Ҫ�õ����ڶ������Ա�����ͼ��������ʼ��֮�����
		void Initialize();
		 

		//��������Ϣ����ÿ֡��ʼǰ����
		void Update();

	private:

		std::unordered_set<int>						m_keyDowns;
		std::unordered_set<int>					    m_keyPress;
		IGraphicDriver*								m_pGraphicDriver;

	};
}