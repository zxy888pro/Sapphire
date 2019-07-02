#pragma once

#include "Sapphire.h"


namespace Sapphire
{
	//处理输入输出的类
	class SAPPHIRE_CLASS Input : public BaseObject
	{
		SAPPHIRE_OBJECT(Input, BaseObject)
	public:
		Input(Core* pCore);
		virtual ~Input();


		 

		//处理窗口消息，在每帧开始前调用
		void Update();


	};
}