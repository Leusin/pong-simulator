#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer()
	: DeltaTime{ -1.0 }
	, StartTime{ Clock::now() }
	, BaseTime{ TimePoint() }
	, PausedTime{ TimePoint() }
	, StopTime{ TimePoint() }
	, PrevTime{ TimePoint() }
	, CurrentTime{ TimePoint() }
	, bStopped{ false }
{
}

float GameTimer::GetDeltaTime() const
{
	return static_cast<float>(DeltaTime);
}

float GameTimer::GetRunTime() const
{
	return std::chrono::duration<float>
		(Clock::now() - StartTime).count();
}

// 일시정지 외에 흐른 시간을 반환한다.
float GameTimer::GetGameTime() const
{
	if (bStopped)
	{
		auto SincePause =
			StopTime - PausedTime;
		auto SinceEpoch =
			std::chrono::duration_cast<std::chrono::nanoseconds>
			(BaseTime.time_since_epoch());
		return std::chrono::duration<float>
			(SincePause - SinceEpoch).count();
	}
	else
	{
		auto SinceStart =
			CurrentTime - PausedTime;
		auto LunchTime =
			std::chrono::duration_cast<std::chrono::nanoseconds>
			(BaseTime.time_since_epoch());
		return std::chrono::duration<float>
			(SinceStart - LunchTime).count();
	}
}

void GameTimer::Reset()
{
	auto now = Clock::now();
	BaseTime = now;
	PrevTime = now;
	StopTime = TimePoint();
	bStopped = false;
}

void GameTimer::Start()
{
	if (bStopped)
	{
		auto now = Clock::now();
		PausedTime += (now - StopTime); // Adjust paused duration
		PrevTime = now;
		StopTime = TimePoint();
		bStopped = false;
	}
}

void GameTimer::Stop()
{
	if (!bStopped)
	{
		StopTime = Clock::now();
		bStopped = true;
	}
}

void GameTimer::Tick()
{
	if (bStopped)
	{
		DeltaTime = 0.0;
		return;
	}

	CurrentTime = Clock::now();
	DeltaTime = std::chrono::duration<double>
		(CurrentTime - PrevTime).count();
	PrevTime = CurrentTime;

	// DeltaTime이 음수일 경우 0으로 처리
	// (프로세서 절전 모드 등으로 인한 문제 방지)
	if (DeltaTime < 0.0)
	{
		DeltaTime = 0.0;
	}
}