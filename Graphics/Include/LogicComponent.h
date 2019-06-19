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

		//一挂到Node时就执行
		virtual void Start(){};
		//首次Update前执行一次
		virtual void DelayedStart() { }

		//每帧执行
		virtual void Update(float deltaTime) {};
		virtual void UpdatePost(float deltaTime) {};
		//固定时间执行
		virtual void FixedUpdate(float deltaTime) {};
		virtual void FixedPostUpdate(float deltaTime) {};

		virtual void Stop() {};

		bool IsDelayedStartCalled() const { return m_isDelayedStartCalled; }

	protected:

		bool m_isDelayedStartCalled;
	};
		 
}