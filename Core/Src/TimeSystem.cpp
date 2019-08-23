#include "TimeSystem.h"
#include "Variant.h"


namespace Sapphire
{

	

	bool HiresTimer::supported(false);
	long long HiresTimer::frequency(1000);

	static unsigned Tick()
	{
#ifdef SAPPHIRE_WIN
		return (unsigned)timeGetTime();
#else
		struct timeval time;
		gettimeofday(&time, NULL);
		return (unsigned)(time.tv_sec * 1000 + time.tv_usec / 1000);
#endif
	}


	static long long HiresTick()
	{
#ifdef SAPPHIRE_WIN
		if (HiresTimer::IsSupported())
		{
			LARGE_INTEGER counter;
			QueryPerformanceCounter(&counter);
			return counter.QuadPart;
		}
		else
			return timeGetTime();
#else
		struct timeval time;
		gettimeofday(&time, NULL);
		return time.tv_sec * 1000000LL + time.tv_usec;
#endif
	}


	Timer::Timer()
	{
		Reset();
	}

	uint Timer::GetMSecs(bool reset)
	{
		unsigned currentTime = Tick();
		unsigned elapsedTime = currentTime - m_startTime;
		if (reset)
			m_startTime = currentTime;

		return elapsedTime;
	}

	void Timer::Reset()
	{
		m_startTime = Tick();
	}

	HiresTimer::HiresTimer()
	{
		Reset();
	}

	long long HiresTimer::GetUSecs(bool reset)
	{
		long long currentTime = HiresTick();
		long long elapsedTime = currentTime - startTime_;

		// 校正一下
		if (elapsedTime < 0)
			elapsedTime = 0;

		if (reset)
			startTime_ = currentTime;

		return (elapsedTime * 1000000LL) / frequency;
	}

	void HiresTimer::Reset()
	{
		startTime_ = HiresTick();
	}

	TimeSystem::TimeSystem(Core* pcore) :SubSystem(pcore),
		frameNumber_(0),
		timeStep_(0.0f),
		timerPeriod_(0)

	{
#ifdef SAPPHIRE_CLASS
		LARGE_INTEGER frequency;
		if (QueryPerformanceFrequency(&frequency))
		{
			HiresTimer::frequency = frequency.QuadPart;
			HiresTimer::supported = true;
		}
#else
		HiresTimer::frequency = 1000000;
		HiresTimer::supported = true;
#endif
	}

	TimeSystem::~TimeSystem()
	{
		SetTimerPeriod(0);
	}

	void TimeSystem::BeginFrame(float timeStep)
	{
		++frameNumber_;
		if (!frameNumber_)
			++frameNumber_;

		timeStep_ = timeStep;

		VariantVector eventData;
		eventData[0] = frameNumber_;
		eventData[1] = timeStep_;
		//发送开始一帧的事件
		FireEvent(ET_CORE_EVENT, EVENT_CORE_BEGINFRAME, (void*)&eventData);

	}

	void TimeSystem::EndFrame()
	{
		//发送结束一帧的事件
		FireEvent(ET_CORE_EVENT, EVENT_CORE_ENDFRAME);
	}

	void TimeSystem::SetTimerPeriod(unsigned mSec)
	{
#ifdef SAPPHIRE_WIN
		if (timerPeriod_ > 0)
			timeEndPeriod(timerPeriod_);

		timerPeriod_ = mSec;

		if (timerPeriod_ > 0)
			timeBeginPeriod(timerPeriod_);
#endif
	}

	float TimeSystem::GetElapsedTime()
	{
		return elapsedTime_.GetMSecs(false) / 1000.0f;
	}

	unsigned TimeSystem::GetSystemTime()
	{
		return Tick();
	}

	unsigned TimeSystem::GetTimeSinceEpoch()
	{
		return (unsigned)time(NULL);
	}

	Sapphire::String TimeSystem::GetTimeStamp()
	{
		time_t sysTime;
		time(&sysTime);
		const char* dateTime = ctime(&sysTime);
		String timeStr(dateTime);
		timeStr.ReplaceSubString("\n", "");
		return timeStr;
	}

	void TimeSystem::Sleep(unsigned mSec)
	{
#ifdef SAPPHIRE_WIN
		::Sleep(mSec);
#else
		usleep(mSec * 1000);
#endif
	}

}