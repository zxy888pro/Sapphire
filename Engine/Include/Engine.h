#pragma once
#include "Sapphire.h"
#include "TimeSystem.h"
#include "Variant.h"

namespace Sapphire
{

	class SAPPHIRE_CLASS Engine : public BaseObject
	{
		SAPPHIRE_OBJECT(Engine, BaseObject)

	public:

		Engine(Core* pCore);
		virtual ~Engine();

		bool Initialize(const VariantMap& parameters);

		//解析参数
		static VariantMap ParseParameters(const std::vector<std::string>& arguments);

		static bool HasParameter(const VariantMap& parameters, const String& parameter);

		static const Variant
			& GetParameter(const VariantMap& parameters, const String& parameter, const Variant& defaultValue = Variant::EMPTY);

		//运行一帧
		void RunFrame();

		//发送帧更新事件通知各子系统更新
		void Update();
		//帧更新以后渲染
		void Render();

		void Exit();

		bool IsExiting()  const { return m_bExiting; }

		bool IsInitialized() const { return m_bInitialized; }

		//应用帧数限制， 计算下一帧的时间步长,如果必要进行一定时间的休眠
		void ApplyFrameLimit();	 
		 
		unsigned GetMinFps() const { return m_uMinFps; }

		void SetMinFps(unsigned val) { m_uMinFps = val; }


		unsigned GetMaxFps() const { return m_uMaxFps; }

		void SetMaxFps(unsigned val) { m_uMaxFps = val; }

		unsigned GetMaxInactiveFps() const { return m_uMaxInactiveFps; }

		void SetMaxInactiveFps(unsigned val) { m_uMaxInactiveFps = val; }

		bool IsPauseMinimized() const { return m_bPauseMinimized; }

		void SetPauseMinimized(bool val) { m_bPauseMinimized = val; }

		//// 获得下一帧的时间步长。 由ApplyFrameLimit来更新
		float GetNextTimeStep() const { return m_uTimeStep; }
		///  设置下一帧的时间步长
		void SetNextTimeStep(float seconds);
			
	private:

		void DoExit();


		/// 帧更新timer
		HiresTimer m_frameTimer;
		/// 为了平滑保存之前的时间步长
		std::vector<float> m_lastTimeSteps;
		/// 下一帧的时间步长
		float m_uTimeStep;
		/// 用多少帧的平均值来平滑时间步长
		unsigned m_uTimeStepSmoothing;
		/// 每秒最小帧数
		unsigned m_uMinFps;
		/// 每大最小帧数
		unsigned m_uMaxFps;
		/// 当app没有取得输入焦点时的每秒最大帧数
		unsigned m_uMaxInactiveFps;
		/// 是否最小化的标志
		bool m_bPauseMinimized;
		//	初始化标志
		bool m_bInitialized;
		//  退出标志
		bool m_bExiting;


	};

}