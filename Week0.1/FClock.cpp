#include "FClock.h"
#include <profileapi.h>

void FClock::Init()
{
	LARGE_INTEGER Frequency;
	QueryPerformanceFrequency(&Frequency);
	CyclesPerSecond = static_cast<float32_t>(Frequency.HighPart);
}

FClock::FClock(float32_t StartTImeSeconds)
	: TimeCycles(SecondsToCycles(StartTImeSeconds))
	, TimeScale(1.0f)
	, bPaused(false)
{
}

uint64_t FClock::GetTimeCycles() const
{
	return TimeCycles;
}

float32_t FClock::CalcDeltaSeconds(const FClock& Other)
{
	uint64_t DeltaTime = TimeCycles - Other.TimeCycles;
	return CyclesToSeconds(DeltaTime);
}

void FClock::Update(float32_t DeltaTImeRealSeconds)
{
	if (!bPaused)
	{
		uint64_t dtScaledCycles = 
			SecondsToCycles(DeltaTImeRealSeconds * TimeScale);
		TimeCycles += dtScaledCycles;
	}
}

void FClock::SetPaused(bool wantPaused)
{
	bPaused = wantPaused;
}

bool FClock::IsPaused() const
{
	return bPaused;
}

void FClock::SetTimeScale(float32_t scale)
{
	TimeScale = scale;
}

float32_t FClock::GetTimeScale() const
{
	return TimeScale;
}

void FClock::SingleStep()
{
	if (bPaused)
	{
		uint64_t dtScaledCycles = 
			SecondsToCycles((1.0f / 30.0f * TimeScale));
		TimeCycles += dtScaledCycles;
	}
}
