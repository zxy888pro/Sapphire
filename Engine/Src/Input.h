#pragma once

#include "Sapphire.h"
#include <unordered_set>


namespace Sapphire
{

	class IGraphicDriver;

	//处理输入输出的类
	class SAPPHIRE_CLASS Input : public SubSystem
	{
		SAPPHIRE_OBJECT(Input, SubSystem)
	public:
		Input(Core* pCore);
		virtual ~Input();

		//初始化输入控制子系统,要用到窗口对象，所以必须在图形驱动初始化之后进行
		void Initialize();
		 

		//处理窗口消息，在每帧开始前调用
		void Update();

	private:

		std::unordered_set<int>						m_keyDowns;
		std::unordered_set<int>					    m_keyPress;
		IGraphicDriver*								m_pGraphicDriver;

	};
}