#pragma once

#include <cstdint>
#include <arm64_neon.h>

/*
* ����: ���� ���� ��Ű��ó p344
*/
class FClock
{
	uint64_t TimeCycles;
	float32_t TimeScale;
	bool bPaused;

	static float32_t CyclesPerSecond;

	static inline uint64_t SecondsToCycles(float32_t TimeSeconds)
	{
		return static_cast<uint64_t>(TimeSeconds * CyclesPerSecond);
	}

	// ����: ª�� �ð��� �ʷ� ��ȯ�� ���� ���.
	static inline float32_t CyclesToSeconds(uint64_t TimeCycles)
	{
		return (float32_t)TimeCycles / CyclesPerSecond;
	}

public:
	static void Init();

	explicit FClock(float32_t StartTImeSeconds = 0.0f);

	// ���� �ð��� �ֱ������ ����.
	uint64_t GetTimeCycles() const;

	// �� Ŭ�ϰ� �ٸ� Ŭ���� ���� �ð� ���̸� ���� �� �� ������ ����.
	float32_t CalcDeltaSeconds(const FClock& Other);

	// ������ �� �� ���� �ҷ��� ��.
	// ������ ������ ������ �ð��� �ʴ����� ����.
	void Update(float32_t DeltaRealSeconds);

	void SetPaused(bool WantPaused);

	bool IsPaused()const;

	void SetTimeScale(float32_t Scale);

	float32_t GetTimeScale() const;

	void SingleStep();
};

