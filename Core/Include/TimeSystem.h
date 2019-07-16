#pragma once
#include "Sapphire.h"

namespace Sapphire
{
	//基于操作系统的低精度timer
	class SAPPHIRE_CLASS Timer
	{
	public: 

		Timer();
		///获取至上次启动以来流逝的毫秒数
		uint   GetMSecs(bool reset);
		//重置计时器
		void   Reset();

	private:

		uint   m_startTime;

	};


	/// 用于profiling的高精度timer
	class SAPPHIRE_CLASS HiresTimer
	{
		friend class TimeSystem;

	public:
		 
		HiresTimer();

		///获取至上次启动以来流逝的微秒数
		long long GetUSecs(bool reset);
		 
		void Reset();

		/// 是否支持高精度计时器
		static bool IsSupported() { return supported; }

		/// 如果支持，返回高精度timer的频率
		static long long GetFrequency() { return frequency; }

	private:
		 
		long long startTime_;
 
		static bool supported;
		 
		static long long frequency;
	};


	class SAPPHIRE_CLASS TimeSystem : public SubSystem
	{
		SAPPHIRE_OBJECT(TimeSystem, SubSystem)

	public:

		TimeSystem(Core* pcore);
		virtual ~TimeSystem();

		 
		void BeginFrame(float timeStep);
	 
		void EndFrame();
		/// 设置低精度timer的周期，0位默认周期
		void SetTimerPeriod(unsigned mSec);

	 
		unsigned GetFrameNumber() const { return frameNumber_; }

	 
		float GetTimeStep() const { return timeStep_; }

	 
		unsigned GetTimerPeriod() const { return timerPeriod_; }

		 
		float GetElapsedTime();


		static unsigned GetSystemTime();
		/// 获取自1970.1.1以来的系统时间
		static unsigned GetTimeSinceEpoch();
		/// 返回获取日期/时间字符串
		static String GetTimeStamp();
		
		static void Sleep(unsigned mSec);


	private:
		/// 自程序启动以来流逝的时间
		Timer elapsedTime_;
		/// 帧号
		unsigned frameNumber_;
		///时间步长
		float timeStep_;
		/// 低精度周期
		unsigned timerPeriod_;

	};

}