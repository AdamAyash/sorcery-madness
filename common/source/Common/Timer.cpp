
#include "Timer.h"
#include "Common.h"

#define ONE_THOUSAND_OF_A_SECOND 1000.0f

namespace common
{
	Timer::Timer()
	{
	}

	Timer::~Timer()
	{
		Restart();
	}

	void Timer::Restart()
	{
		m_oStartInterval = common::Clock::now();
	}

	float Timer::Elapsed()
	{
		return static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(common::Clock::now() - m_oStartInterval).count());
	}

	float Timer::ElapsedMilliseconds()
	{
		return Elapsed() / ONE_THOUSAND_OF_A_SECOND;
	}
}