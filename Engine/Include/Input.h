#pragma once

#include "Sapphire.h"
#include <unordered_set>
#include "Math/Vector2.h"


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

		void Release();

		bool HasFocus() { return m_inputFocusFlag; }



		int GetKeyFromName(const String& name) const;

		int GetKeyFromScancode(int scancode) const;

		String GetKeyName(int key) const;

		int GetScancodeFromKey(int key) const;

		int GetScancodeFromName(const String& name) const;

		String GetScancodeName(int scancode) const;

		bool GetKeyDown(int key) const;

		bool GetKeyPress(int key) const;

		bool GetScancodeDown(int scancode) const;

		bool GetScancodePress(int scancode) const;

		bool GetMouseButtonDown(int button) const;

		bool GetMouseButtonPress(int button) const;

		IntVector2 GetMousePosition() const;

		/// 返回至上一帧以来，鼠标的移动
		const IntVector2& GetMouseMove() const { return m_mouseMove; }

		int GetMouseMoveX() const { return m_mouseMove.x_; }

		int GetMouseMoveY() const { return m_mouseMove.y_; }

		int GetMouseMoveWheel() const { return m_mouseWheelMove; }

		void SetMouseVisible(bool enable, bool suppressEvent = false);

		void ResetMouseVisible();

		bool IsMouseVisible() const { return m_bMouseVisible; }

		//处理窗口消息，在每帧开始前调用
		void Update();


		virtual void Invoke(ushort eEventType, ushort eEvent, EventContext* src, void* eventData = NULL) override;


	private:

		//获取输入焦点
		void GainFocus();
		//失去输入焦点
		void LoseFocus();

		//重置状态
		void ResetState();

		//处理鼠标按键状态改变
		void SetMouseButton(int button, bool newState);

		//处理Key改变
		void SetKey(int key, int scancode, unsigned raw, bool newState);

		void SetMouseWheel(int delta);

		void SetMousePosition(const IntVector2& position);

		std::unordered_set<int>						m_keyDowns;
		std::unordered_set<int>					    m_keyPress;
		std::unordered_set<int>					    m_scancodePress;
		std::unordered_set<int>					    m_scancodeDowns;

		IntVector2                                  m_mouseMove;
		int                                         m_mouseWheelMove;
		bool										m_bMouseVisible;

		bool										m_inputFocusFlag;
		IGraphicDriver*								m_pGraphicDriver;

	};
}