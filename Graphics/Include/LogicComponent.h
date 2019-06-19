#pragma once
#include "Sapphire.h"
#include "Component.h"


namespace Sapphire
{
	class SAPPHIRE_CLASS LogicComponent : public Component
	{
		SAPPHIRE_OBJECT(LogicComponent, Component)

	public:

		LogicComponent(Core* pCore);
		virtual ~LogicComponent();

		//һ�ҵ�Nodeʱ��ִ��
		virtual void Start(){};
		//�״�Updateǰִ��һ��
		virtual void DelayedStart() { }

		//ÿִ֡��
		virtual void Update(float deltaTime) {};
		virtual void UpdatePost(float deltaTime) {};
		//�̶�ʱ��ִ��
		virtual void FixedUpdate(float deltaTime) {};
		virtual void FixedPostUpdate(float deltaTime) {};

		virtual void Stop() {};

		bool IsDelayedStartCalled() const { return m_isDelayedStartCalled; }

	protected:

		bool m_isDelayedStartCalled;
	};
		 
}