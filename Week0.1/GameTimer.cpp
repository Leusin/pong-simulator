#include "GameTimer.h"
#include <windows.h>

GameTimer::GameTimer()
	: SecondsPerCount(0.0)
	, DeltaTime(-1.0)
	, BaseTime(0)
	, PausedTime(0)
	, PrevTime(0)
	, CurrentTime(0)
	, bStopped(false)
{
	LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
	SecondsPerCount = 1.0 / static_cast<double>(frequency.QuadPart);
}

float GameTimer::GetGameTime() const
{
	if (bStopped) 
	{
		return static_cast<float>(PausedTime - BaseTime);
	}
	else 
	{
		return static_cast<float>(CurrentTime - BaseTime);
	}
}

float GameTimer::GetDeltaTime() const
{
	return static_cast<float>(DeltaTime);
}

// 일시정지 외에 흐른 시간을 반환한다.
float GameTimer::GetTotalTime() const
{
	if (bStopped)
	{
		return static_cast<float>(((
			StopTime - PausedTime) - BaseTime) * 
			SecondsPerCount);
	}
	else
	{
		return static_cast<float>(((
			CurrentTime - PausedTime) - BaseTime) * 
			SecondsPerCount
			);
	}
}

void GameTimer::Reset()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);

	BaseTime = static_cast<int64_t>(CurrentTime.QuadPart);
	PrevTime = BaseTime;
	StopTime = 0;
	bStopped = false;
}

void GameTimer::Start()
{
	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	int64_t StartTime = static_cast<int64_t>(CurrentTime.QuadPart);

	if (bStopped)
	{
		PausedTime += (StartTime - StopTime);
		PrevTime = StartTime;
		StopTime = 0;
		bStopped = false;
	}
}

void GameTimer::Stop()
{
	if (bStopped)
	{
		return;
	}

	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	StopTime = static_cast<int64_t>(CurrentTime.QuadPart);
	bStopped = true;
}

void GameTimer::Tick()
{
	if (bStopped)
	{
		DeltaTime = 0.0;
		return;
	}

	LARGE_INTEGER CurrentTime;
	QueryPerformanceCounter(&CurrentTime);
	this->CurrentTime = static_cast<int64_t>(CurrentTime.QuadPart);

	DeltaTime = (this->CurrentTime - PrevTime) * SecondsPerCount;

	PrevTime = this->CurrentTime;

	// 프로세서가 절전 모드로 들어가거나
	// 다른 프로세서와 엉키는 경우
	// DeltaTime 이 음수가 될 수 있다.
	if (DeltaTime < 0.0)
	{
		DeltaTime = 0.0;
	}
}