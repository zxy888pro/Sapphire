#pragma once
#include "Sapphire.h"

namespace Sapphire
{
	//���ڲ���ϵͳ�ĵ;���timer
	class SAPPHIRE_CLASS Timer
	{
	public: 

		Timer();
		///��ȡ���ϴ������������ŵĺ�����
		uint   GetMSecs(bool reset);
		//���ü�ʱ��
		void   Reset();

	private:

		uint   m_startTime;

	};


	/// ����profiling�ĸ߾���timer
	class SAPPHIRE_CLASS HiresTimer
	{
		friend class TimeSystem;

	public:
		 
		HiresTimer();

		///��ȡ���ϴ������������ŵ�΢����
		long long GetUSecs(bool reset);
		 
		void Reset();

		/// �Ƿ�֧�ָ߾��ȼ�ʱ��
		static bool IsSupported() { return supported; }

		/// ���֧�֣����ظ߾���timer��Ƶ��
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
		/// ���õ;���timer�����ڣ�0λĬ������
		void SetTimerPeriod(unsigned mSec);

	 
		unsigned GetFrameNumber() const { return frameNumber_; }

	 
		float GetTimeStep() const { return timeStep_; }

	 
		unsigned GetTimerPeriod() const { return timerPeriod_; }

		 
		float GetElapsedTime();


		static unsigned GetSystemTime();
		/// ��ȡ��1970.1.1������ϵͳʱ��
		static unsigned GetTimeSinceEpoch();
		/// ���ػ�ȡ����/ʱ���ַ���
		static String GetTimeStamp();
		
		static void Sleep(unsigned mSec);


	private:
		/// �Գ��������������ŵ�ʱ��
		Timer elapsedTime_;
		/// ֡��
		unsigned frameNumber_;
		///ʱ�䲽��
		float timeStep_;
		/// �;�������
		unsigned timerPeriod_;

	};

}