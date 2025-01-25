#pragma once

#include <cstdint>

/*
* 참고: 게임 엔진 아키텍처 p344
*/
class FClock
{
	uint64_t TimeCycles;
	float TimeScale;
	bool bPaused;

	static float CyclesPerSecond;

	static inline uint64_t SecondsToCycles(float TimeSeconds)
	{
		return static_cast<uint64_t>(TimeSeconds * CyclesPerSecond);
	}

	// 주의: 짧은 시간을 초로 변환할 떄만 사용.
	static inline float CyclesToSeconds(uint64_t TimeCycles)
	{
		return (float)TimeCycles / CyclesPerSecond;
	}

public:
	static void Init();

	explicit FClock(float StartTImeSeconds = 0.0f);

	// 현재 시각을 주기단위로 리턴.
	uint64_t GetTimeCycles() const;

	// 이 클록과 다른 클록의 절대 시간 차이를 구한 후 초 단위로 리턴.
	float CalcDeltaSeconds(const FClock& Other);

	// 프레임 당 한 번씩 불러야 함.
	// 실제로 측정한 프레임 시간을 초단위로 받음.
	void Update(float DeltaRealSeconds);

	void SetPaused(bool WantPaused);

	bool IsPaused()const;

	void SetTimeScale(float Scale);

	float GetTimeScale() const;

	void SingleStep();
};

